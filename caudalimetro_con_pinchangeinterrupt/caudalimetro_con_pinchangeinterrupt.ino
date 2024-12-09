#include <PinChangeInterrupt.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
volatile float waterFlow; // Cambiado a float
const int sensorPin = 50;


void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("MAXING");
  delay(600);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Litros: ");
  waterFlow = 0.0; // Cambiado a 0.0
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(sensorPin), pulse, RISING);

  Serial.begin(9600);
}

void loop() {
  lcd.setCursor(0, 0);
  Serial.print("Litros: ");
  lcd.print("Litros: ");
  lcd.setCursor(0, 1);
  lcd.print(waterFlow, 1); // Mostrar solo un decimal
  Serial.println(waterFlow, 1); 
  lcd.setCursor(7, 1);
  lcd.print("litros ");
  delay(200);
}

void pulse() {
  waterFlow += 1.0 / 450.0; // Incremento ajustado
}
