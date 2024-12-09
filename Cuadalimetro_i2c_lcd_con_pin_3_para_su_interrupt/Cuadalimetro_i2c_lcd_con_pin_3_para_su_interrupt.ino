#include <LiquidCrystal_I2C.h> // Debe descargar la Libreria que controla el I2C

#include<Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
volatile double waterFlow;
const int sensorPin=50;

void setup() 
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("MAXING");
  delay(600);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Litros: ");
  waterFlow = 0;
  attachInterrupt(digitalPinToInterrupt(sensorPin), pulse, RISING);  //DIGITAL Pin 2: Interrupt 0
  Serial.begin(9600);
}
void loop() 
{
   lcd.setCursor(0,0);
   Serial.print("Litros: ");
   lcd.print("Litros: ");
   lcd.setCursor(0,1);
   lcd.print(waterFlow);
   Serial.println(waterFlow); 
   lcd.setCursor(7,1);
   lcd.print("litros ");
   delay(200);
}
void pulse()   //measure the quantity of square wave
{
  waterFlow += 1.0 / 450.0;
}
