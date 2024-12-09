/****************************************
 * Incluir Librerias
 ****************************************/
#define ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <UbidotsEsp32Mqtt.h>

 
/****************************************
 * Definir pines chars y funciones
 ****************************************/
const char* ssid = "WIFI1E62";
const char* password = "uP337xNoz3";

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

int trig3 = 13;     //Transmisor
int eco3 = 12;      //Receptor
int duracion3;     //Variable para duracion del pulso
int distancia3;    //Variable para hallar la distancia
int nivel3;

int trig4 = 14;     //Transmisor
int eco4 = 27;      //Receptor
int duracion4;     //Variable para duracion del pulso
int distancia4;    //Variable para hallar la distancia
int nivel4;

int trig5 = 2;     //Transmisor
int eco5 = 4;      //Receptor
int duracion5;     //Variable para duracion del pulso
int distancia5;    //Variable para hallar la distancia
int nivel5;

int trig6 = 5;     //Transmisor
int eco6 = 18;      //Receptor
int duracion6;     //Variable para duracion del pulso
int distancia6;    //Variable para hallar la distancia
int nivel6;



/****************************************
 * Ubidots
 ****************************************/  
const char *UBIDOTS_TOKEN = "BBUS-WCZoqU3dRcV6S22mx6BBb7rtSgjk3V";
const char *DEVICE_LABEL = "max";
const char *VARIABLE_LABEL_1 = "nivel1"; 
const char *VARIABLE_LABEL_2 = "nivel2"; 
const char *VARIABLE_LABEL_3 = "nivel3"; 
const char *VARIABLE_LABEL_4 = "nivel4"; 
const char *VARIABLE_LABEL_5 = "nivel5"; 
const char *VARIABLE_LABEL_6 = "nivel6"; 
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
  pinMode(trig3, OUTPUT);
  pinMode(eco3, INPUT);
  pinMode(trig4, OUTPUT);
  pinMode(eco4, INPUT);
  pinMode(trig5, OUTPUT);
  pinMode(eco5, INPUT);
  pinMode(trig6, OUTPUT);
  pinMode(eco6, INPUT);

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
   delay(1);

   digitalWrite(trig3, HIGH);
   delay(1);
   digitalWrite(trig3, LOW);
   duracion3 = pulseIn(eco3, HIGH);  //Recibe el puslo
   distancia3 = duracion3 / 58.2;    //Calculo para hallar la distancia en cm
   nivel3=map(distancia3,40,0,0,100);
   delay(1);

   digitalWrite(trig4, HIGH);
   delay(1);
   digitalWrite(trig4, LOW);
   duracion4 = pulseIn(eco4, HIGH);  //Recibe el puslo
   distancia4 = duracion4 / 58.2;    //Calculo para hallar la distancia en cm
   nivel4=map(distancia4,40,0,0,100);
   delay(1);

   digitalWrite(trig5, HIGH);
   delay(1);
   digitalWrite(trig5, LOW);
   duracion5 = pulseIn(eco5, HIGH);  //Recibe el puslo
   distancia5 = duracion5 / 58.2;    //Calculo para hallar la distancia en cm
   nivel5=map(distancia5,40,0,0,100);
   delay(1);

   digitalWrite(trig6, HIGH);
   delay(1);
   digitalWrite(trig6, LOW);
   duracion6 = pulseIn(eco6, HIGH);  //Recibe el puslo
   distancia6 = duracion6 / 58.2;    //Calculo para hallar la distancia en cm
   nivel6=map(distancia6,40,0,0,100);


    ubidots.add(VARIABLE_LABEL_1, nivel1);
    ubidots.add(VARIABLE_LABEL_2, nivel2);
    ubidots.add(VARIABLE_LABEL_3, nivel3);
    ubidots.add(VARIABLE_LABEL_4, nivel4);
    ubidots.add(VARIABLE_LABEL_5, nivel5);
    ubidots.add(VARIABLE_LABEL_6, nivel6);
    ubidots.publish(DEVICE_LABEL);
    Serial.println("Enviando los datos a Ubidots: ");
    Serial.println("Nivel1: " + String(nivel1));
    Serial.println("Nivel2: " + String(nivel2));
    Serial.println("Nivel3: " + String(nivel3));
    Serial.println("Nivel4: " + String(nivel4));
    Serial.println("Nivel5: " + String(nivel5));
    Serial.println("Nivel6: " + String(nivel6));
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
