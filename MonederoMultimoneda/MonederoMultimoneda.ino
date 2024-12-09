#include <LiquidCrystal_I2C.h>

//Constantes para los pines.
const int SignalPin = 2;
const int bcancelar = 13;
const int bcloro = 12; //precio 10
const int bfabuloso = 11; // precio 15
const int bsalvo = 10; //precio 12
const int bmrmusculo = 9; // precio 16
const int bpinol = 8;// precio 14
const int bajax = 7; //precio 13

//estados para realizar acciones
unsigned long botones;
int bestadoc = 0; 

int bmcloro;
int bmfabuloso;
int bmsalvo;
int bmmrmusculo;
int bmpinol;
int bmajax;


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
const int sensorPin = 3;
const int measureInterval = 200;
volatile int pulseConter;
const float factorK = 7.5;
float volume = 0;
long t0 = 0;
void ISRCountPulse()
{
   pulseConter++;
}

LiquidCrystal_I2C lcd(0x27,16,2);


void setup() {

// Inicializamos la comunicacion serial, para ver los resultados en el monitor.
Serial.begin(9600);

//Agregamos la interrupcion con el pin indicado.
attachInterrupt(digitalPinToInterrupt(SignalPin), coinInterrupt, RISING);

  pinMode (13,INPUT);
  pinMode (12, INPUT);
  pinMode (11, INPUT);
  pinMode (10, INPUT);
  pinMode (9, INPUT);
  pinMode (8, INPUT);
  pinMode (7, INPUT);
  botones=millis();

  lcd.init();
  lcd.backlight();
  lcd.clear();

}

// Interrupcion.
void coinInterrupt(){
 
  // Cada vez que insertamos una moneda valida, incrementamos el contador de monedas y encendemos la variable de control,
  pulso++;
  MillisUltPulso = millis();
  Serial.println("Interrupcion");
   
}

void loop() {

//Calculamos los milisegundos de la ultima ejecusion menos el ultimo tiempo que se genero un pulso.
unsigned long lastTime = millis() - MillisUltPulso;

//Validamos si hay algun puslo, si es asi tambien se valida que el ultimo tiempo asignado sea mayor a la cantidad de milisegundos establecidos.
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

//Validamos la moneda depositada.
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


  lcd.setCursor(0,0);
  lcd.print("Credito:"); 
  lcd.setCursor (10,0);
  lcd.print(CreditoAcum);

  
////compras
  if(millis()-botones >= 200) 
{

if (digitalRead(bcloro) == HIGH && bestadoc == 0 && CreditoAcum==10) //INICIO DE COMPRA DEL CLORO
{
CreditoAcum = CreditoAcum - 10;
if(CreditoAcum < 0)
{
  CreditoAcum = 0; 
}
Serial.print("Credito Total: $ ");
Serial.print(CreditoAcum);
Serial.println(".00");
botones=millis();
bestadoc=1;
lcd.clear();
}
if (bestadoc==1 && digitalRead(bcloro)==LOW){bestadoc=0;} //FIN DE COMPRA DEL CLORO


if (digitalRead(bfabuloso) == HIGH && bestadoc == 0 && CreditoAcum== 15) //INICIO DE COMPRA DEL FABULOSO
{
CreditoAcum = CreditoAcum - 15;
if(CreditoAcum < 0)
{
  CreditoAcum = 0; 
}
Serial.print("Credito Total: $ ");
Serial.print(CreditoAcum);
Serial.println(".00");
botones=millis();
bestadoc=1;
lcd.clear();
}
if (bestadoc==1 && digitalRead(bfabuloso)==LOW){bestadoc=0;} //FIN DE COMPRA DEL FABULOSO


if (digitalRead(bsalvo) == HIGH && bestadoc == 0 && CreditoAcum== 12) //INICIO DE COMPRA DEL SALVO
{
CreditoAcum = CreditoAcum - 12;
if(CreditoAcum < 0)
{
  CreditoAcum = 0; 
}
Serial.print("Credito Total: $ ");
Serial.print(CreditoAcum);
Serial.println(".00");
botones=millis();
bestadoc=1;
lcd.clear();
}
if (bestadoc==1 && digitalRead(bsalvo)==LOW){bestadoc=0;} //FIN DE COMPRA DEL SALVO

if (digitalRead(bmrmusculo) == HIGH && bestadoc == 0 && CreditoAcum==16) //INICIO DE COMPRA DEL MRMUSCULO
{
CreditoAcum = CreditoAcum - 16;
if(CreditoAcum < 0)
{
  CreditoAcum = 0; 
}
Serial.print("Credito Total: $ ");
Serial.print(CreditoAcum);
Serial.println(".00");
botones=millis();
bestadoc=1;
lcd.clear();
}
if (bestadoc==1 && digitalRead(bmrmusculo)==LOW){bestadoc=0;} //FIN DE COMPRA DEL MRMUSCULO

if (digitalRead(bpinol) == HIGH && bestadoc == 0 && CreditoAcum==14 ) //INICIO DE COMPRA DEL PINOL
{
CreditoAcum = CreditoAcum - 14;
if(CreditoAcum < 0)
{
  CreditoAcum = 0; 
}
Serial.print("Credito Total: $ ");
Serial.print(CreditoAcum);
Serial.println(".00");
botones=millis();
bestadoc=1;
lcd.clear();
}
if (bestadoc==1 && digitalRead(bpinol)==LOW){bestadoc=0;} //FIN DE COMPRA DEL PINOL


if (digitalRead(bajax) == HIGH && bestadoc == 0 && CreditoAcum==13) //INICIO DE COMPRA DEL AJAX
{
CreditoAcum = CreditoAcum - 13;
if(CreditoAcum < 0)
{
  CreditoAcum = 0; 
}
Serial.print("Credito Total: $ ");
Serial.print(CreditoAcum);
Serial.println(".00");
botones=millis();
bestadoc=1;
lcd.clear();
}
if (bestadoc==1 && digitalRead(bajax)==LOW){bestadoc=0;} //FIN DE COMPRA DEL AJAX

if(digitalRead(bcancelar) == HIGH && bestadoc == 0 && CreditoAcum >0)
{
  CreditoAcum = 0;
  Serial.print("Credito Total: $ ");
  Serial.print (CreditoAcum);
  Serial.println(".00");
  botones=millis();
  bestadoc=1;
  lcd.clear();
}
if (bestadoc==1 && digitalRead(bcancelar)==LOW){bestadoc=0;} //cancelamiento de compra


}

//despacho del producto

}
