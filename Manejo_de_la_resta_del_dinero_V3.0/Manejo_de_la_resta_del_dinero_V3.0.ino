#include <Servo.h>

// Definición de los pines de los LEDs
const int pinLED_10 = 13; // LED para moneda de 10 pesos
const int pinLED_5 = 12;  // LED para moneda de 5 pesos
const int pinLED_2 = 11;   // LED para moneda de 2 pesos
const int pinLED_1 = 10;   // LED para moneda de 1 peso

// Definición de los pines de los servomotores
const int pinServo_10 = 24; // Servomotor para moneda de 10 pesos
const int pinServo_5 = 25;  // Servomotor para moneda de 5 pesos
const int pinServo_2 = 26;  // Servomotor para moneda de 2 pesos
const int pinServo_1 = 27;  // Servomotor para moneda de 1 peso

Servo servo_10;
Servo servo_5;
Servo servo_2;
Servo servo_1;

void setup() {
  // Configuración de los pines de los LEDs como salidas
  pinMode(pinLED_10, OUTPUT);
  pinMode(pinLED_5, OUTPUT);
  pinMode(pinLED_2, OUTPUT);
  pinMode(pinLED_1, OUTPUT);

  // Configuración de los pines de los servomotores
  servo_10.attach(pinServo_10);
  servo_5.attach(pinServo_5);
  servo_2.attach(pinServo_2);
  servo_1.attach(pinServo_1);

  // Inicialización de la comunicación serial para depuración
  Serial.begin(9600);
}

void loop() {
  // Array de monedas disponibles
  int monedas[] = {10, 5, 2, 1};
  int num_monedas = sizeof(monedas) / sizeof(monedas[0]); // Calcula el número de monedas disponibles

  int credito_restante = 23; // Crédito restante (debe ser igualado al crédito acumulado)

  // Iteración sobre las monedas disponibles
  for (int i = 0; i < num_monedas; i++) {
    // Calcula cuántas veces se puede restar la moneda actual del crédito restante
    int veces = credito_restante / monedas[i];
    // Actualiza el crédito restante restando la cantidad de monedas utilizadas
    credito_restante -= veces * monedas[i];

    // Muestra información sobre el descuento realizado
    Serial.print("Descuento de ");
    Serial.print(veces);
    Serial.print(" moneda(s) de ");
    Serial.print(monedas[i]);
    Serial.println(" pesos");

    // Enciende y apaga el LED correspondiente al descuento
    switch (monedas[i]) {
      case 10:
        flashLED(pinLED_10, veces);
        moveServo(servo_10, veces); // Mueve el servomotor de 10 pesos el número de veces
        break;
      case 5:
        flashLED(pinLED_5, veces);
        moveServo(servo_5, veces); // Mueve el servomotor de 5 pesos el número de veces
        break;
      case 2:
        flashLED(pinLED_2, veces);
        moveServo(servo_2, veces); // Mueve el servomotor de 2 pesos el número de veces
        break;
      case 1:
        flashLED(pinLED_1, veces);
        moveServo(servo_1, veces); // Mueve el servomotor de 1 peso el número de veces
        break;
    }

    // Si ya no queda crédito por descontar, salir del bucle
    if (credito_restante == 0) {
      break;
    }
  }

  // Actualizar el crédito acumulado con el crédito restante después del descuento
  actualizarCreditoAcumulado(credito_restante);
}

// Función para encender y apagar un LED cierta cantidad de veces
void flashLED(int pinLED, int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pinLED, HIGH); // Enciende el LED
    delay(500);                  // Espera 0.5 segundos
    digitalWrite(pinLED, LOW);   // Apaga el LED
    delay(500);                  // Espera 0.5 segundos
  }
}

// Función para mover un servomotor un cierto número de veces entre 0 y 180 grados
void moveServo(Servo servo, int times) {
  for (int i = 0; i < times; i++) {
    servo.write(180); // Mueve el servomotor a 180 grados
    delay(500);       // Espera 0.5 segundos
    servo.write(0);   // Devuelve el servomotor a 0 grados
    delay(500);       // Espera 0.5 segundos
  }
}

// Función para actualizar el crédito acumulado
void actualizarCreditoAcumulado(int credito_restante) {
  // Tu lógica para actualizar el crédito acumulado
}
