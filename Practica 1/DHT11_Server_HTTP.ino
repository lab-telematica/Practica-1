#include <ESP8266WiFi.h>   // Biblioteca para la conexión Wi-Fi en ESP8266
#include <ESP8266HTTPClient.h> // Biblioteca para realizar solicitudes HTTP
#include <DHT.h>           // Biblioteca para el sensor DHT

#define DHTPIN 4           // Pin digital conectado al sensor DHT
#define DHTTYPE DHT11      // Tipo de sensor DHT (DHT11 o DHT22)
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "nombre_de_tu_red";          // SSID de la red Wi-Fi
const char* password = "contraseña_de_tu_red";  // Contraseña de la red Wi-Fi

const char* serverName = "http://example.com/data";  // Dirección del servidor HTTP

void setup() {
  Serial.begin(115200);  // Inicializar el puerto serie
  dht.begin();           // Inicializar el sensor DHT

  // Conectar a la red Wi-Fi
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");
}

void loop() {
  // Leer los datos del sensor DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Verificar si hubo un error al leer el sensor
  if (isnan(h) || isnan(t)) {
    Serial.println("Error al leer del sensor DHT!");
    return;
  }

  // Mostrar los datos en el monitor serie
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" *C");

  // Verificar que la conexión Wi-Fi sigue activa antes de enviar los datos
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;  // Crear un cliente Wi-Fi
    HTTPClient http;
    http.begin(client, serverName);  // Iniciar conexión con el servidor

    // Crear el payload JSON con los datos del sensor
    String postData = "{\"temperatura\":" + String(t) + ", \"humedad\":" + String(h) + "}";
    http.addHeader("Content-Type", "application/json");  // Añadir el encabezado HTTP

    // Realizar la solicitud POST al servidor
    int httpResponseCode = http.POST(postData);

    // Verificar si la solicitud fue exitosa
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Respuesta del servidor: " + response);
    } else {
      Serial.print("Error en la solicitud HTTP, Código: ");
      Serial.println(httpResponseCode);
    }

    http.end();  // Finalizar la conexión HTTP
  } else {
    Serial.println("Conexión Wi-Fi perdida. Intentando reconectar...");
    WiFi.reconnect();  // Intentar reconectar a Wi-Fi
  }

  // Pausa de 10 segundos entre cada envío de datos
  delay(10000);
}
