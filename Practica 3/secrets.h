#include <pgmspace.h>

const char WIFI_SSID[] = "replaceWithSSDI";               
const char WIFI_PASSWORD[] = "replaceWithPassword";     

#define THINGNAME "ESP8266"

int8_t TIME_ZONE = -5; 

const char MQTT_HOST[] = "replace_with_aws_mqtt_endpoint";


static const char cacert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
jANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxM...
-----END CERTIFICATE-----
)EOF";


// Copy contents from XXXXXXXX-certificate.pem.crt here ▼
static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
IpxjFFiycgLqnPRMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMzEwMTIxNTU5
MjhaFw00OTEyMzEyMz...
-----END CERTIFICATE-----
)KEY";


// Copy contents from  XXXXXXXX-private.pem.key here ▼
static const char privkey[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
PwDYJXO5/VHPsimEz1Zl4Re/90+jUDAxHLbaiLTI68UoI/uGLlLXKRT6tXJOWHPa
iEhqEQSLfvU0TJ2I8y3eqOX2qAHbk0/i4k7XisWe8WczFOh3FeOzPiMs3CHk23WM
OqUlAVG0PyX/u2AUV36QiN....
-----END RSA PRIVATE KEY-----
)KEY";