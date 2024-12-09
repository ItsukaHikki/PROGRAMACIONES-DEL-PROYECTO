#include <LiquidCrystal_I2C.h>
#include <PinChangeInterrupt.h>
#include <Wire.h>
#include <Servo.h>

// Variables para controlar el movimiento del servomotor
unsigned long lastMoveTime = 0;
unsigned long moveInterval = 150; // Intervalo entre movimientos en milisegundos
int currentStep = 0; // Paso actual del movimiento

//Constantes para la compra y el monedero.
const int SignalPin = 2;
const int bcancelar = 13;
const int bcloro = 12; //precio 10

//Estados
unsigned long botones;
unsigned long almacenamientolcd=0;
const unsigned long tiempolcd = 100;
int bestadoc = 0; 
bool agua= false;
int bmcloro;

//Constantes para las monedas.
const int UnPeso = 2;
const int DosPesos = 4;
const int CincoPesos = 6;
const int DiezPesos = 8;

//Variables.
volatile int pulso = 0;
//volatile unsigned long MillisUltPulso = 0;
unsigned long MillisUltPulso = 0;
int PulsosAcum = 0;
int CreditoAcum = 0;
int MaxTimePulse = 200;


//caudalimetros y bombas
volatile float waterFlow; 
const int sensorPin = 10;
int r1=8;

// Definición de los pines de los servomotores
const int pinServo_10 = 22 ; 
const int pinServo_5 = 23;  
const int pinServo_2 = 24;  
const int pinServo_1 = 25;  
Servo servo_10;
Servo servo_5;
Servo servo_2;
Servo servo_1;

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() 
{
 
Serial.begin(9600);

digitalWrite(r1,HIGH);//Relevador en HIGH por estado invertido

attachInterrupt(digitalPinToInterrupt(SignalPin), coinInterrupt, RISING);// Interrupcion con el pin del sensor de monedas.

attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(sensorPin), pulse, RISING);// Interrupción con el pin del sensor del caudalimetro usando interrupción digital.

  pinMode (13,INPUT);
  pinMode (12,INPUT);
  pinMode (r1,OUTPUT);
  
// Configuración  de los servomotores
  servo_10.attach(pinServo_10);
  servo_5.attach(pinServo_5);
  servo_2.attach(pinServo_2);
  servo_1.attach(pinServo_1);

//Uso simultaneo de programas (hacer que el lcd no se detenga en desarrollo)
  botones=millis();
  almacenamientolcd=millis();
  
//Inicio de la secuencia de encendido del LCD.
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("MAXING");
  lcd.setCursor(2,1);
  lcd.print("SomeTimes");
  delay(600);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Credito:");
  lcd.setCursor(0, 1);
  lcd.print("Litros: ");
  waterFlow = 0.0; //se cambia para solo mostrar un decimal
}

void loop() 

{
    if (millis() - almacenamientolcd >= tiempolcd) 
    {
        pantalla();
        almacenamientolcd = millis();
    }

    monedas();
    
    if(millis()-botones >= 50)
    { 
        compras();
        botones = millis();
    }
}

//Interrupción Caudalimetro
void pulse() 
{
  if(agua==true)
  {
  waterFlow += 1.0 / 450.0; // Incremento ajustado
  }
}

// Interrupcion del sensor de monedas.
void coinInterrupt()
{
  // Cada vez que insertamos una moneda valida, incrementamos el contador de monedas y encendemos la variable de control,
  pulso++;
  MillisUltPulso = millis();
  Serial.println("Interrupcion");  
}


void pantalla()
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

void monedas()
{
//Calculo de los milisegundos de la ultima ejecusion menos el ultimo tiempo que se genero un pulso.
unsigned long lastTime = millis() - MillisUltPulso;

//Validación de algun puslo,y si es asi tambien se valida que el ultimo tiempo asignado sea mayor a la cantidad de milisegundos establecidos.
if((pulso > 0) && (lastTime >= MaxTimePulse)){

    //La cantidad de creditos es el contador y acumulador de pulsos, hasta que se cumpla alguna condicion.
    PulsosAcum = pulso;
    pulso = 0;
    Serial.print("Pulses: ");
    Serial.print(PulsosAcum);
    Serial.print(" LastTime: ");
    Serial.print(lastTime);
    Serial.print(" LastPulse: ");
    Serial.println(MillisUltPulso);    
}

//Validación de la moneda depositada.
switch (PulsosAcum){

  case UnPeso:
    PulsosAcum = 0;
    Serial.println("Moneda depositada de $ 1.00");
    CreditoAcum += 1;
    Serial.print("Credito Total: $ ");
    Serial.print(CreditoAcum);
    Serial.println(".00");
    break;

  case DosPesos:
    PulsosAcum = 0;
    Serial.println("Moneda depositada de $ 2.00");
    CreditoAcum += 2;
    Serial.print("Credito Total: $ ");
    Serial.print(CreditoAcum);
    Serial.println(".00");
    break;
    
  case CincoPesos:

    PulsosAcum = 0;
    Serial.println("Moneda depositada de $ 5.00");
    CreditoAcum += 5;
    Serial.print("Credito Total: $ ");
    Serial.print(CreditoAcum);
    Serial.println(".00");
    break;

  case DiezPesos:

    PulsosAcum = 0;
    Serial.println("Moneda depositada de $ 10.00");
    CreditoAcum += 10;
    Serial.print("Credito Total: $ ");
    Serial.print(CreditoAcum);
    Serial.println(".00");
    break;
  
  }
}

void compras()
{
//Función de compra del cloro
if (digitalRead(bcloro) == HIGH && bestadoc == 0 && CreditoAcum>=10) 
{
CreditoAcum = CreditoAcum - 10;
if(CreditoAcum < 0)
{
  CreditoAcum = 0; 
}
digitalWrite(r1,LOW);
Serial.print("Credito Total: $ ");
Serial.print(CreditoAcum);
Serial.println(".00");
bestadoc=1;
lcd.clear();
agua=true;
cambio();
}
//Función para evitar el rebote del boton
if (bestadoc==1 && digitalRead(bcloro)==LOW){bestadoc=0;}
//Función de cancelamiento de compra, para devolver cambio
if(digitalRead(bcancelar) == HIGH && bestadoc == 0 && CreditoAcum >0)
{
  cambio();
  Serial.print("Credito Total: $ ");
  Serial.print (CreditoAcum);
  Serial.println(".00");
  bestadoc=1;
  lcd.clear();
}
//Función antirrebote del botón 
if (bestadoc==1 && digitalRead(bcancelar)==LOW){bestadoc=0;} //cancelamiento de compra

//Función para poner Fin al despacho del producto
 if(waterFlow>=1)
  {
    waterFlow=0;
    digitalWrite(r1,HIGH);
    agua=false;
    CreditoAcum=0;
  }
  
}

void cambio() 
{
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
  // Verificar si es hora de realizar el próximo movimiento
  if (millis() - lastMoveTime >= moveInterval) {
    // Realizar el próximo paso del movimiento
    if (currentStep % 2 == 0) {
      servo.write(180); // Mueve a 180 grados en pasos pares
    } else {
      servo.write(0); // Mueve a 0 grados en pasos impares
    }

    // Actualizar el tiempo del último movimiento
    lastMoveTime = millis();

    // Incrementar el paso actual del movimiento
    currentStep++;

    // Si se han completado todos los movimientos necesarios, reiniciar el contador de pasos
    if (currentStep >= times * 2) {
      currentStep = 0;  
    }
  }
}

// Función para actualizar el crédito acumulado
void actualizarCreditoAcumulado(int credito_restante) {
  CreditoAcum = 0;
}
