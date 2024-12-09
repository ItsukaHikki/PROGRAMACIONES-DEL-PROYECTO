#include <LiquidCrystal_I2C.h>
#include <PinChangeInterrupt.h>
#include <Wire.h>
#include <Servo.h>

// Constantes para los pines.
const int SignalPin = 2;
const int bcancelar = 13;
const int bcloro = 12; // Precio 10

// Estados para realizar acciones
unsigned long botones;
int bestadoc = 0; 
bool agua = false; // Cambiado a bool para representar estado de agua
int bmcloro;

// Constantes para las monedas.
const int UnPeso = 2;
const int DosPesos = 4;
const int CincoPesos = 6;
const int DiezPesos = 8;

// Variables.
volatile int pulso = 0;
unsigned long MillisUltPulso = 0;
int CreditoAcum = 0;
int MaxTimePulse = 200;

// Caudalímetro y bombas
volatile float waterFlow = 0.0; // Cambiado a float
const int sensorPin = 10;
int r1 = 8;

// Definición de los pines de los servomotores
Servo servo_10;
Servo servo_5;
Servo servo_2;
Servo servo_1;
const int pinServo_10 = 3; // Servomotor para moneda de 10 pesos
const int pinServo_5 = 5;  // Servomotor para moneda de 5 pesos
const int pinServo_2 = 6;  // Servomotor para moneda de 2 pesos
const int pinServo_1 = 9;  // Servomotor para moneda de 1 peso

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Inicialización de la comunicación serial
  Serial.begin(9600);
  // Iniciar relevador con HIGH
  pinMode(r1, OUTPUT);
  digitalWrite(r1, HIGH);
  // Interrupción con el pin del sensor de monedas
  attachInterrupt(digitalPinToInterrupt(SignalPin), coinInterrupt, RISING);
  // Interrupción con el pin del sensor del caudalímetro usando interrupción digital
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(sensorPin), pulse, RISING);
  // Declaración de modo de pines
  pinMode(bcancelar, INPUT);
  pinMode(bcloro, INPUT);
  // Configuración de los pines de los servomotores
  servo_10.attach(pinServo_10);
  servo_5.attach(pinServo_5);
  servo_2.attach(pinServo_2);
  servo_1.attach(pinServo_1);
  // Iniciar el LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("MAXING");
  lcd.setCursor(2, 1);
  lcd.print("SomeTimes");
  delay(600);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Credito:");
  lcd.setCursor(0, 1);
  lcd.print("Litros: ");
}

// Interrupción del caudalímetro
void pulse() {
  if (agua) {
    waterFlow += 1.0 / 450.0; // Incremento ajustado
  }
}

// Interrupción del sensor de monedas
void coinInterrupt() {
  pulso++;
  MillisUltPulso = millis();
}

void loop() {
  // Actualizar el LCD
  actualizarLCD();
  // Procesar monedas depositadas
  procesarMonedas();
  // Procesar compras
  procesarCompras();
}

// Función para actualizar el contenido del LCD
void actualizarLCD() 
{
  lcd.setCursor(0,0);
  lcd.print("Credito:");  
  lcd.setCursor (10,0);
  lcd.print(CreditoAcum);
  lcd.setCursor(0,1);
  lcd.print("Litros");
  lcd.setCursor(10,1);
  lcd.print(waterFlow,1);
}

// Función para procesar monedas depositadas
void procesarMonedas() {
  unsigned long lastTime = millis() - MillisUltPulso;
  if (pulso > 0 && lastTime >= MaxTimePulse) {
    switch (pulso) {
      case UnPeso:
        CreditoAcum += 1;
        break;
      case DosPesos:
        CreditoAcum += 2;
        break;
      case CincoPesos:
        CreditoAcum += 5;
        break;
      case DiezPesos:
        CreditoAcum += 10;
        break;
    }
    pulso = 0;
  }
}

// Función para procesar compras
void procesarCompras() {
  if (millis() - botones >= 200) {
    if (digitalRead(bcloro) == HIGH && bestadoc == 0 && CreditoAcum >= 10) {
      // Compra de cloro
      CreditoAcum -= 10;
      digitalWrite(r1, LOW);
      botones = millis();
      bestadoc = 1;
      agua = true;
      cambio();
      lcd.clear();
    }
    if (bestadoc == 1 && digitalRead(bcloro) == LOW) {
      bestadoc = 0;
    }
    if (digitalRead(bcancelar) == HIGH && bestadoc == 0 && CreditoAcum > 0) {
      // Cancelar compra
      cambio();
      botones = millis();
      bestadoc = 1;
      lcd.clear();
    }
    if (bestadoc == 1 && digitalRead(bcancelar) == LOW) {
      bestadoc = 0;
    }
  }

  // Finalizar despacho de producto
  if (waterFlow >= 1) {
    waterFlow = 0;
    digitalWrite(r1, HIGH);
    agua = false;
   CreditoAcum=0;
  }
}

// Función para devolver cambio
void cambio() {
  int monedas[] = {10, 5, 2, 1};
  int num_monedas = sizeof(monedas) / sizeof(monedas[0]);
  int credito_restante = CreditoAcum;
  for (int i = 0; i < num_monedas; i++) {
    int veces = credito_restante / monedas[i];
    credito_restante -= veces * monedas[i];
    switch (monedas[i]) {
      case 10:
        moveServo(servo_10, veces);
        break;
      case 5:
        moveServo(servo_5, veces);
        break;
      case 2:
        moveServo(servo_2, veces);
        break;
      case 1:
        moveServo(servo_1, veces);
        break;
    }
    if (credito_restante == 0) {
      break;
    }
  }
  actualizarCreditoAcumulado(credito_restante);
}

// Función para mover un servomotor un cierto número de veces entre 0 y 180 grados
void moveServo(Servo servo, int times) {
  for (int i = 0; i < times; i++) {
    servo.write(180);
    delay(200);
    servo.write(0);
    delay(200);
  }
}

// Función para actualizar el crédito acumulado
void actualizarCreditoAcumulado(int credito_restante) {
  CreditoAcum = 0;
}
