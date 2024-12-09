#define RXp2 16
#define TXp2 17
int x = 0;

void setup() {
  Serial.begin(9600);   // Inicializa el puerto serie de la ESP32
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);  // Inicializa el puerto serie del Arduino
}

void loop() {
  if (Serial2.available() > 0) {  // Verifica si hay datos disponibles en el puerto serie del Arduino
    String Data = Serial2.readStringUntil('\n'); // Lee la cadena recibida hasta que encuentra un salto de línea
    Data.trim(); // Elimina los espacios en blanco al principio y al final de la cadena
    Serial.println("Mensaje recibido desde Arduino: " + Data); // Imprime el mensaje recibido desde Arduino

    // Verifica si la cadena recibida es igual a la palabra específica ("Hello Boss")
    if (Data.equals("Guenhacestusmomos enelmonitorserial")) {
      x = x + 1;
      Serial.println("Se recibió la palabra 'Guenhacestusmomos enelmonitorserial'. Ejecutando la acción...");
      Serial.println("Contador actual: " + String(x));
    } else {
      Serial.println("Mensaje recibido no coincide con 'Guenhacestusmomos enelmonitorserial'.");
    }
  }
}
