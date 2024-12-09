/****************************************
 * Incluir Librerias
 ****************************************/
#define ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <UbidotsEsp32Mqtt.h>

 
/****************************************
 * Definir pines chars y funciones
 ****************************************/
const char* ssid = "INFINITUM24BB";
const char* password = "MyhfugXRt7";

int trig1 = 26;     //Transmisor
int eco1 = 25;      //Receptor
int duracion1;     //Variable para duracion del pulso
int distancia1;    //Variable para hallar la distancia
int nivel1;

int trig2 = 23;     //Transmisor
int eco2 = 22;      //Receptor
int duracion2;     //Variable para duracion del pulso
int distancia2;    //Variable para hallar la distancia
int nivel2;

/****************************************
 * Ubidots
 ****************************************/  
const char *UBIDOTS_TOKEN = "BBUS-IBaf9noF3aAocm5xAF704o4O7NvSbA";
const char *DEVICE_LABEL = "maxing";
const char *VARIABLE_LABEL_1 = "nivel1"; 
const char *VARIABLE_LABEL_2 = "nivel2"; 
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
  
  pinMode(trig1, OUTPUT);
  pinMode(eco1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(eco2, INPUT);

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
   digitalWrite(trig1, HIGH);
   delay(1);
   digitalWrite(trig1, LOW);
   duracion1 = pulseIn(eco1, HIGH);  //Recibe el puslo
   distancia1 = duracion1 / 58.2;    //Calculo para hallar la distancia en cm
   nivel1=map(distancia1,40,0,0,100);
   delay(1);

   digitalWrite(trig2, HIGH);
   delay(1);
   digitalWrite(trig2, LOW);
   duracion2 = pulseIn(eco2, HIGH);  //Recibe el puslo
   distancia2 = duracion2 / 58.2;    //Calculo para hallar la distancia en cm
   nivel2=map(distancia2,40,0,0,100);


    ubidots.add(VARIABLE_LABEL_1, nivel1);
    ubidots.add(VARIABLE_LABEL_2, nivel2);
    ubidots.publish(DEVICE_LABEL);
    Serial.println("Enviando los datos a Ubidots: ");
    Serial.println("Nivel: " + String(nivel1));
    Serial.println("Nivel: " + String(nivel2));
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
