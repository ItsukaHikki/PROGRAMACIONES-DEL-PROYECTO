#include <HardwareSerial.h>

void setup() {
  Serial.begin(9600); // Inicializa el puerto serie para la comunicación con el monitor serial
  Serial2.begin(9600); // Inicializa el puerto serie para la comunicación con el Arduino Uno
}

void loop() {
  // Envía datos al Arduino Uno
  Serial2.println("Hola desde ESP32");

  // Espera recibir datos del Arduino Uno
  if (Serial2.available()) {
    String dataFromArduino = Serial2.readStringUntil('\n');
    Serial.println("Mensaje recibido desde Arduino Uno: " + dataFromArduino);
  }

  // Espera recibir datos desde el usuario para enviar al Arduino Uno
  if (Serial.available()) {
    String dataToSend = Serial.readStringUntil('\n');
    Serial2.println(dataToSend);
  }

  // Aquí puedes agregar cualquier otra lógica que necesites ejecutar en la ESP32
}
