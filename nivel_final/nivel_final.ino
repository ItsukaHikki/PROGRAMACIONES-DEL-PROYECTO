/****************************************
 * Incluir Librerías
 ****************************************/
#include <WiFi.h>
#include <UbidotsEsp32Mqtt.h>

/****************************************
 * Definir constantes y parámetros
 ****************************************/
// Credenciales de WiFi
const char* ssid = "WIFI1E62";  
const char* password = "uP337xNoz3"; 

// Token y parámetros de Ubidots
const char *UBIDOTS_TOKEN = "BBUS-WCZoqU3dRcV6S22mx6BBb7rtSgjk3V";  // Asegúrate de usar el token correcto de tu cuenta Ubidots
const char *DEVICE_LABEL = "max";  // Nombre del dispositivo (asegúrate de que exista en Ubidots)

// Variables de Ubidots
const char *VARIABLE_LABELS[6] = {"nivel1", "nivel2", "nivel3", "nivel4", "nivel5", "nivel6"};

// Pinos de los sensores ultrasónicos
#define TRIG1 26
#define ECHO1 25
#define TRIG2 23
#define ECHO2 22
#define TRIG3 13
#define ECHO3 12
#define TRIG4 14
#define ECHO4 27
#define TRIG5 2
#define ECHO5 4
#define TRIG6 5
#define ECHO6 18

/****************************************
 * Variables globales
 ****************************************/
unsigned long timer;
const int PUBLISH_FREQUENCY = 5000; // Frecuencia para publicar en Ubidots (ms)
Ubidots ubidots(UBIDOTS_TOKEN); // Inicialización del cliente Ubidots

/****************************************
 * Configuración inicial (setup)
 ****************************************/
void setup() {
  Serial.begin(115200);

  // Conectar a la red WiFi
  ConectarWifi();

  // Configurar los pines de los sensores ultrasónicos
  configurarPines();

  // Configurar Ubidots
  ubidots.setDebug(true); // Activa la depuración
  ubidots.setCallback(callback); // Configura la función de retorno de mensajes
  ubidots.setup();
  ubidots.reconnect(); // Intenta la reconexión a Ubidots

  timer = millis(); // Inicia el temporizador
}

/****************************************
 * Bucle principal (loop)
 ****************************************/
void loop() {
  // Verifica la conexión con Ubidots y WiFi
  if (!ubidots.connected()) {
    Serial.println("Intentando reconectar a Ubidots...");
    ubidots.reconnect();
  }

  if (WiFi.status() != WL_CONNECTED) {
    ConectarWifi();
  }

  // Publicar los niveles cada 5 segundos
  if (millis() - timer >= PUBLISH_FREQUENCY) {
    publicarNiveles();
    timer = millis();
  }

  // Ejecutar el cliente de Ubidots
  ubidots.loop();
  delay(1000);
}

/****************************************
 * Conectar a la red WiFi
 ****************************************/
void ConectarWifi() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("Conectando a WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nConectado a WiFi");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
  }
}

/****************************************
 * Configurar pines de los sensores ultrasónicos
 ****************************************/
void configurarPines() {
  int pines_trig[] = {TRIG1, TRIG2, TRIG3, TRIG4, TRIG5, TRIG6};
  int pines_echo[] = {ECHO1, ECHO2, ECHO3, ECHO4, ECHO5, ECHO6};

  for (int i = 0; i < 6; i++) {
    pinMode(pines_trig[i], OUTPUT);
    pinMode(pines_echo[i], INPUT);
  }
}

/****************************************
 * Publicar los niveles de los sensores en Ubidots
 ****************************************/
void publicarNiveles() {
  int pines_trig[] = {TRIG1, TRIG2, TRIG3, TRIG4, TRIG5, TRIG6};
  int pines_echo[] = {ECHO1, ECHO2, ECHO3, ECHO4, ECHO5, ECHO6};

  for (int i = 0; i < 6; i++) {
    int nivel = medirNivel(pines_trig[i], pines_echo[i]);
    if (nivel != -1) {
      ubidots.add(VARIABLE_LABELS[i], nivel);
      Serial.print("Nivel ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(nivel);
    }
  }

  ubidots.publish(DEVICE_LABEL);
  Serial.println("Datos publicados en Ubidots");
}

/****************************************
 * Medir nivel de un sensor ultrasónico
 ****************************************/
int medirNivel(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duracion = pulseIn(echo, HIGH, 50000); // 50ms de espera máxima para recibir eco
  if (duracion == 0) {
    Serial.println("Error: No se recibió señal de eco.");
    return -1; // Si no se detecta señal, retorna error
  }

  int distancia = duracion / 58.2; // Conversión a cm
  int nivel = map(distancia, 40, 0, 0, 100); // Mapea de 0-40cm a 0-100%
  
  nivel = constrain(nivel, 0, 100); // Asegura que el nivel esté entre 0 y 100
  return nivel;
}

/****************************************
 * Función de retorno para mensajes MQTT
 ****************************************/
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Mensaje recibido de Ubidots [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
