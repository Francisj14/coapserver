/*
ESP-COAP Server
*/

#include <ESP8266WiFi.h>
#include <coap_server.h>
#include "DHT.h"
#define DHTTYPE DHT11 

float temp,dist,hum;

////////////HCSR04-Distancia////////////////////
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
const int trigPin = 14;  //D4
const int echoPin = 12;  //D3
long duration;
/////////DHT11////////////////
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];
const int DHTPin = 5;  //Comunicación de datos en el pin 5 (GPIO 5 -- D1)
// Iniciando sensor
DHT dht(DHTPin, DHTTYPE);
//////////////////////////////////
// CoAP server endpoint url callback
void callback_temp(coapPacket &packet, IPAddress ip, int port, int obs);
void callback_hum(coapPacket &packet, IPAddress ip, int port, int obs);
void callback_dist(coapPacket &packet, IPAddress ip, int port, int obs);
coapServer coap;

//WiFi connection info
const char* ssid = "---"; //Nombre ed red
const char* password = "--";  //Contraseña de red

// LED STATE
bool LEDSTATE;

// CoAP server endpoint URL 1
void callback_temp(coapPacket *packet, IPAddress ip, int port,int obs) {
  Serial.println("temperatura");

  // send response
  char p[packet->payloadlen + 1];
  memcpy(p, packet->payload, packet->payloadlen);
  p[packet->payloadlen] = NULL;
  Serial.println(p);
  String message(p);
  char buff[5];
  sprintf(buff,"%3.2f",temp);
  Serial.println(buff);
  coap.sendResponse(ip, port,buff);
  //delay(1000);
 
}

// CoAP server endpoint URL 2
void callback_hum(coapPacket *packet, IPAddress ip, int port,int obs) {
  Serial.println("distancia");

  // send response
  char p[packet->payloadlen + 1];
  memcpy(p, packet->payload, packet->payloadlen);
  p[packet->payloadlen] = NULL;
  Serial.println(p);
  String message(p);
  char buff[5];
  sprintf(buff,"%3.2f",hum);
  Serial.println(buff);
  coap.sendResponse(ip, port,buff);
  //delay(1000);
 
}

// CoAP server endpoint URL 2
void callback_dist(coapPacket *packet, IPAddress ip, int port,int obs) {
  Serial.println("humedad");

  // send response
  char p[packet->payloadlen + 1];
  memcpy(p, packet->payload, packet->payloadlen);
  p[packet->payloadlen] = NULL;
  Serial.println(p);
  String message(p);
  char buff[5];
  sprintf(buff,"%3.2f",dist);
  Serial.println(buff);
  coap.sendResponse(ip, port,buff);
  //delay(1000);
 
}

void setup() {
  yield();
    //serial begin
    Serial.begin(115200);
    dht.begin();
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    WiFi.begin(ssid, password);
    Serial.println(" ");

  // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    
  while (WiFi.status() != WL_CONNECTED) {
    //delay(500);
    yield();
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
    // Print the IP address
    Serial.println(WiFi.localIP());

  //Aplicacion del sensor
           hum = dht.readHumidity();
          // Leyendo temperatura en Celsius (es la unidad de medición por defecto)
           temp = dht.readTemperature();
          // Leyendo temperatura en Fahrenheit (Si es "true" la leerá en esa unidad)
          float f = dht.readTemperature(true);
          // Revisión de fallas en la lectura
          if (isnan(hum) || isnan(temp) || isnan(f)) {
            Serial.println("Fallos al leer el sensor DHT");
            strcpy(celsiusTemp, "Fallido");
            strcpy(fahrenheitTemp, "Fallido");
            strcpy(humidityTemp, "Fallido");
          }

          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
    dist= duration*0.034/2;
  
  // add server url endpoints.
  // can add multiple endpoint urls.
       coap.server(callback_temp, "temperatura");
       coap.server(callback_hum, "humedad");
       coap.server(callback_dist, "distancia");
    // coap.server(callback_text,"text");

  // start coap server/client
  coap.start();
  // coap.start(5683);
}

void loop() {
  coap.loop();
  delay(1000);

}
