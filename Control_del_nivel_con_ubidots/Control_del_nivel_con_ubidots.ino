/****************************************
 * Incluir Librerias
 ****************************************/
#define ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <UbidotsEsp32Mqtt.h>
#include "DHT.h"
 
/****************************************
 * Definir pines chars y funciones
 ****************************************/
const char* ssid = "INFINITUM24BB";
const char* password = "MyhfugXRt7";

int trig = 26;     //Transmisor
int eco = 25;      //Receptor
int duracion;     //Variable para duracion del pulso
int distancia;    //Variable para hallar la distancia
int nivel;

/****************************************
 * Ubidots
 ****************************************/  
const char *UBIDOTS_TOKEN = "BBUS-IBaf9noF3aAocm5xAF704o4O7NvSbA";
const char *DEVICE_LABEL = "maxing";
const char *VARIABLE_LABEL_1 = "distancia"; 
const char *VARIABLE_LABEL_2 = "nivel"; 
const int PUBLISH_FREQUENCY = 5000; 
unsigned long timer;
unsigned long reinicio;
uint8_t analogPin = 34; 
Ubidots ubidots(UBIDOTS_TOKEN);

/*********************************************
 * Función importante de la bomba en ubidots
 *********************************************/ 
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() 
{
  Serial.begin(115200);
  ConectarWifi();
  
  pinMode(trig, OUTPUT);
  pinMode(eco, INPUT);

  timer = millis();
  reinicio=millis();
  
  ubidots.connectToWifi(ssid, password);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  
}
 
 void loop() 
{

 /************************************************************
 * Reconección a ubidots por si se cae y variable de la bomba
 *************************************************************/ 
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }


 /************************************************************
 * Lectura de los sensores y publicación en ubidots
 *************************************************************/ 

  if (millis() - timer >= PUBLISH_FREQUENCY) 
  {
   digitalWrite(trig, HIGH);
   delay(1);
   digitalWrite(trig, LOW);
   duracion = pulseIn(eco, HIGH);  //Recibe el puslo
   distancia = duracion / 58.2;    //Calculo para hallar la distancia en cm
   nivel=map(distancia,40,0,0,100);


    ubidots.add(VARIABLE_LABEL_1, distancia);
    ubidots.add(VARIABLE_LABEL_2, nivel);
    ubidots.publish(DEVICE_LABEL);
    Serial.println("Enviando los datos a Ubidots: ");
    Serial.println("Distancia: " + String(distancia));
    Serial.println("Nivel: " + String(nivel));
    Serial.println("-----------------------------------------");
    timer = millis();
  }
 /******************************
 * Loop de ubidots 
 *******************************/ 
  if (millis()-reinicio>=10000)
  {
  ubidots.loop();
  reinicio=millis();
  }
}

 /******************************
 * Conexión a Wifi 
 *******************************/ 

void ConectarWifi() 
{
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");
    Serial.println("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(2, HIGH);
      delay(200);
      digitalWrite(2, LOW);
      delay(200);
      Serial.print(".");
    }
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}
