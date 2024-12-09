#include <PinChangeInterrupt.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
volatile float waterFlow; // Cambiado a float
const int sensorPin = 10;
const int sensorPin2=3;
int r1=8;
const int b1=9;
int estado=0;

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
  attachInterrupt(digitalPinToInterrupt(sensorPin2), pulse, RISING);
  pinMode(r1,OUTPUT);
  pinMode(b1,INPUT);
  digitalWrite(r1,HIGH);
  Serial.begin(9600);
}

void loop() 
{
  if(digitalRead(b1)==HIGH && estado==0) 
  {
    digitalWrite(r1,LOW);
    estado=1; 
  }
  if (estado==1 && digitalRead(b1)==LOW){estado=0;}

  
  lcd.setCursor(0, 0);
  Serial.print("Litros: ");
  lcd.print("Litros: ");
  lcd.setCursor(0, 1);
  lcd.print(waterFlow, 1); // Mostrar solo un decimal
  Serial.println(waterFlow, 1); 
  lcd.setCursor(7, 1);
  lcd.print("litros ");

  if(waterFlow>=1)
  {
    waterFlow=0;
    digitalWrite(r1,HIGH);
  }
  delay(200);
  
}

void pulse() {
  waterFlow += 1.0 / 450.0; // Incremento ajustado
}
