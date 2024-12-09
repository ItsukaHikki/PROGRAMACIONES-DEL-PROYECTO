
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600); // Inicializa el puerto serie para la comunicación con el monitor serial
  mySerial.begin(9600); // Inicializa el puerto serie virtual en los pines 10 (RX) y 11 (TX)
}

void loop() {
  // Espera recibir datos de la ESP32
  if (mySerial.available()) {
    String dataFromESP32 = mySerial.readStringUntil('\n');
    Serial.println("Mensaje recibido desde ESP32: " + dataFromESP32);
  }

  // Espera recibir datos desde el usuario para enviar a la ESP32
  if (Serial.available()) {
    String dataToSend = Serial.readStringUntil('\n');
    mySerial.println(dataToSend);
  }

  // Aquí puedes agregar cualquier otra lógica que necesites ejecutar en el Arduino Uno
}
