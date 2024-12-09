#include <LiquidCrystal_I2C.h>
#include <PinChangeInterrupt.h>
#include <Wire.h>

//Constantes para los pines.
const int SignalPin = 2;
const int bcancelar = 13;
const int bcloro = 12; //precio 10

//estados para realizar acciones
unsigned long botones;
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
int CreditoAcum = 100;
int MaxTimePulse = 200;


//caudalimetros y bombas
volatile float waterFlow; // Cambiado a float
const int sensorPin = 10;
int r1=8;

bool bug=false;

LiquidCrystal_I2C lcd(0x27,16,2);
 

void setup() {

// Inicializamos la comunicacion serial, para ver los resultados en el monitor.
Serial.begin(9600);
//Se incia el relevador con HIGH, dado que sus estados son invertidos
digitalWrite(r1,HIGH);
// Interrupcion con el pin del sensor de monedas.
attachInterrupt(digitalPinToInterrupt(SignalPin), coinInterrupt, RISING);
// Interrupción con el pin del sensor del caudalimetro usando interrupción digital.
attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(sensorPin), pulse, RISING);
// Declaración de modo de pines
  pinMode (13,INPUT);
  pinMode (12, INPUT);
  pinMode(r1,OUTPUT);
//Uso simultaneo de programas  
  botones=millis();
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
  
  pulso++;
  MillisUltPulso = millis();
  Serial.println("Interrupcion");
   
}

void loop() 
{

//Loop del lcd
  lcd.setCursor(0,0);
  lcd.print("Credito:");  
  lcd.setCursor (10,0);
  lcd.print(CreditoAcum);
  lcd.setCursor(0,1);
  lcd.print("Litros");
  lcd.setCursor(10,1);
  lcd.print(waterFlow,1);


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
Serial.println("entraste a la compra de cloro");
}
//Función para evitar el rebote del boton
if (bestadoc==1 && digitalRead(bcloro)==LOW){bestadoc=0;}
//Función de cancelamiento de compra, para devolver cambio
if(digitalRead(bcancelar) == HIGH && bestadoc == 0 && CreditoAcum >0)
{
  CreditoAcum = 0;
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
    bug=true;
  }

 // if(bug==true && CreditoAcum>0)
 // {
  //  CreditoAcum=0;
   // bug=false;
  //}

}
