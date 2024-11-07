//Instalar la placa ESP8266
/* Archivo->Preferencias
 * Agregar la ruta https://arduino.esp8266.com/stable/package_esp8266com_index.json
 * 
 * Herramientas->Placa->Gestor de tarjetas
 * Instalar ESP8266 community
 */

// Instalar librería DHT
/*
 * Programa->Incluir Librería->Administrar Biblioteca
 * Instalar DHT sensor library de Adafruit
 */

// Instalar librería EasyBuzzer
/*
 * Programa->Incluir Librería->Administrar Biblioteca
 * Instalar EasyBuzzer de Evert Arias
 */

// Instalar librería PubSubClient
/*
 * Programa->Incluir Librería->Administrar Biblioteca
 * Instalar PubSubClient de Nick O'Leary
 */

 // Instalar librería ArduinoJson
/*
 * Programa->Incluir Librería->Administrar Biblioteca
 * Instalar ArduinoJson de Benoit
 */
 
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"
#include "DHT.h"
#include "EasyBuzzer.h"

#define DHTPIN 4        // (D2) Pin digital conectado al sensor DHT
#define BUZZPIN 5       // (D1) Pin digital conectado al buzzer
#define DHTTYPE DHT11   // DHT 11
#define LDRPIN A0

DHT dht(DHTPIN, DHTTYPE);
 
float h ;
float t;
int l;
unsigned long lastMillis = 0;
unsigned long previousMillis = 0;
const long interval = 5000;
 
#define AWS_IOT_PUBLISH_TOPIC   "esp8266/1/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp8266/1/sub"
 
WiFiClientSecure net;
 
BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);
 
PubSubClient client(net);
 
time_t now;
time_t nowish = 1672531200; // 01/01/2023 00:00:00 (fecha base)
 
 
void NTPConnect(void)
{
  Serial.print("Configurando fecha/hora mediante SNTP");
  //configTime(int timezone_sec, int daylightOffset_sec, const char* server1, const char* server2);
  configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("\nConfigurado!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Fecha y hora actual: ");
  Serial.println(asctime(&timeinfo));
}
 
 
void messageReceived(char *topic, byte *payload, unsigned int length)
{
  EasyBuzzer.beep(1000, 3);
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
 
 
void connectAWS()
{
  delay(3000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println(String("\nIntentando conectar a la red: ") + String(WIFI_SSID));
 
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(String("\nConectado a la red: ") + String(WIFI_SSID));

  NTPConnect();
 
  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);
 
  client.setServer(MQTT_HOST, 8883);
  client.setCallback(messageReceived);
 
 
  Serial.println("\nConectando a AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(1000);
  }
 
  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("\nConectado a AWS IoT!");
}
 
 
void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["time"] = now;
  doc["humidity"] = h;
  doc["temperature"] = t;
  doc["light"] = l;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
 
 
void setup()
{
  Serial.begin(115200);
  connectAWS();
  dht.begin();
  EasyBuzzer.setPin(BUZZPIN);
}
 
 
void loop()
{
  if (millis() - lastMillis > interval)
  {
    lastMillis = millis();
    if (!client.connected())
    {
      connectAWS();
    }
    else
    {
      client.loop();
      
      h = dht.readHumidity();
      t = dht.readTemperature();
      l = analogRead(LDRPIN);
    
      if (isnan(h) || isnan(t) )
      {
        Serial.println(F("Error al leer el sensor DHT!"));
        return;
      }
     
      Serial.print(F("Humedad: "));
      Serial.print(h);
      Serial.print(F("%  Temperatura: "));
      Serial.print(t);
      Serial.print(F("°C Intensidad: "));
      Serial.print(l);
      Serial.println(F(" %"));
     
      now = time(nullptr);
      publishMessage();
    }
  }
  EasyBuzzer.update();
}
