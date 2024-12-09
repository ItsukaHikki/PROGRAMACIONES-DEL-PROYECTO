#include <LiquidCrystal_I2C.h>
#include <PinChangeInterrupt.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,16,2);
//BOTONES DE PRODUCTO

const int boton1= 13; //PRODUCTO 1
const int boton2= 12; //PRODUCTO 2
const int boton3= 11; //PRODUCTO 3
const int boton4= 9;  //PRODUCTO 4
const int boton5= 7;  //PRODUCTO 5
const int boton6= 5;  //FUNCIÓN CAMBIO

//VARIABLES PARA DEBOUNCE

const unsigned long debounceDelay = 50;  // Tiempo de debounce en ms

unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
unsigned long lastDebounceTime4 = 0;
unsigned long lastDebounceTime5 = 0;
unsigned long lastDebounceTime6 = 0;

bool boton1Presionado = false;
bool boton2Presionado = false;
bool boton3Presionado = false;
bool boton4Presionado = false;
bool boton5Presionado = false;
bool boton6Presionado = false;

//COIN DETECTOR

const int coin = 2; // Sensor de monedas

//CONSTANTES PARA LAS MONEDAS

const int UnPeso = 2;
const int DosPesos = 4;
const int CincoPesos = 6;
const int DiezPesos = 8;

//VARIABLES DEL COIN DETECTOR

volatile int pulso = 0;
//volatile unsigned long MillisUltPulso = 0;
unsigned long MillisUltPulso = 0;
int PulsosAcum = 0;
int CreditoAcum = 0;
int creditosPrev = -1;
int MaxTimePulse = 200;

//CAUDALIMETROS

const int caudal1 = 65;
const int caudal2 = 50;
const int caudal3 = 51;
const int caudal4 = 52;
const int caudal5 = 53;

//BOMBAS

const int rele1=26;
const int rele2=27;
const int rele3=28;
const int rele4=29;
const int rele5=30;

// VARIABLES DE TIEMPO PARA ACTUALIZAR EL LCD
unsigned long lastUpdateLCD = 0;
const unsigned long lcdUpdateDelay = 200;


//VARIABLES VARIAS

bool agua=false;
bool bug1=false;
bool bug2=false;
bool activarAntibugs = false;
bool voidcompras=false;
unsigned long reseteo = 0;
unsigned long when=0;
volatile float waterFlow; // Cambiado a float

//VARIABLES PARA BARRA DE CARGA
bool dispensado=false;
const unsigned long tiempoBarra = 100;
unsigned long actualiza = 0;
int progreso = 0; 
bool dispensando = false;
bool actualizandoCreditos = true;

//Variables PARA ACTIVIAR EL COIN DETECTOR
const int ondec = 45;


//FUNCIÓN DE MOSTRAR CREDITOS EN EL LCD

void mostrarCreditos() 
{
  lcd.setCursor(0,0);
  lcd.print("Creditos:");
  lcd.setCursor(0,1);
  lcd.print("               ");
  lcd.setCursor(0, 1);
  lcd.print(CreditoAcum);
}

void creditosinsuficientes() 
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Creditos");
  lcd.setCursor(0,1);
  lcd.print("Insuficientes");
  delay(500);
  mostrarCreditos();
}

//SENSOR DE MONEDAS
void coinInterrupt()
{
 
  // Cada vez que insertamos una moneda valida, incrementamos el contador de monedas y encendemos la variable de control,
  pulso++;
  MillisUltPulso = millis();
  Serial.println("Interrupcion");
   
}

//Interrupción Caudalimetro
void pulse() 
{
  if(agua==true)
  {
  waterFlow += 1.0 / 450.0; // Incremento ajustado
  }
   if(waterFlow>=1)
  {
    waterFlow = 0;
    digitalWrite(rele1,HIGH);
    digitalWrite(rele2,HIGH);
    digitalWrite(rele3,HIGH);
    digitalWrite(rele4,HIGH);
    digitalWrite(rele5,HIGH);
    agua = false;
    voidcompras = false;
    bug1 = true;
    reseteo = millis();
    activarAntibugs = true;
  }
}

void setup() 
{
  Serial.begin(9600);
  
//BOTONES CON RESISTENCIA INTERNA
  
  pinMode(boton1, INPUT_PULLUP);
  pinMode(boton2, INPUT_PULLUP);
  pinMode(boton3, INPUT_PULLUP);
  pinMode(boton4, INPUT_PULLUP);
  pinMode(boton5, INPUT_PULLUP);
  pinMode(boton6, INPUT_PULLUP);

//COINDETECTOR

  attachInterrupt(digitalPinToInterrupt(coin), coinInterrupt, RISING);

//CAUDALIMETROS

  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(caudal1), pulse, RISING);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(caudal2), pulse, RISING);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(caudal3), pulse, RISING);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(caudal4), pulse, RISING);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(caudal5), pulse, RISING);
  waterFlow = 0.0;

//BOMBAS

  pinMode(rele1,OUTPUT);
  pinMode(rele2,OUTPUT);
  pinMode(rele3,OUTPUT);
  pinMode(rele4,OUTPUT);
  pinMode(rele5,OUTPUT);

  digitalWrite(rele1,HIGH);
  digitalWrite(rele2,HIGH);
  digitalWrite(rele3,HIGH);
  digitalWrite(rele4,HIGH);
  digitalWrite(rele5,HIGH);
  

//LCD

  lcd.init();
  lcd.backlight();
  lcd.clear();
  mostrarCreditos();

//ACTIVANCIÓN DEL COIN DETECTOR
  pinMode(ondec,OUTPUT);
  digitalWrite(ondec,LOW);
}

void loop() 
{
  dinero();

  leerbotones();

  actualizarCreditos();
  
  if(activarAntibugs){
   antibugs(); 
  }

  
  if(dispensado == true){
    actualizarbarra();
  }
  
}

//FUNCIÓN PARA REGISTRAR LAS MONEDAS
void dinero()
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

//FUNCIÓN DE LEER LOS BOTONES
void leerbotones ()
{
  // Botón 1 
  if (digitalRead(boton1) == LOW) {
    if ((millis() - lastDebounceTime1) > debounceDelay && !boton1Presionado && CreditoAcum>=10 && !voidcompras) {
      Serial.println("Boton 1 presionado");
      Serial.println("Dispensando 1er producto");
      Serial.print("Credito Total: $ ");
      Serial.print(CreditoAcum);
      Serial.println(".00");
      agua = true;
      boton1Presionado = true;
      voidcompras = true;
      bug2 = true;
      Serial.print("valor bug2: ");
      Serial.println(bug2);
      lastDebounceTime1 = millis();
      CreditoAcum-=10;
      digitalWrite(rele1,LOW);
      digitalWrite(ondec,HIGH);
      Serial.println("desactivando el coin detector");
      iniciardispensado();
    }
    
    //NEGACIÓN DE COMPRA POR CREDITOS

    if ((millis() - lastDebounceTime1) > debounceDelay && !boton1Presionado && CreditoAcum<10){
      Serial.println("Boton 1 presionado");
      Serial.println("Creditos insuficientes");
      boton1Presionado = true;
      lastDebounceTime1 = millis();
      creditosinsuficientes(); 
    }
  } else {
    boton1Presionado = false;
  }

  // Botón 2 
  if (digitalRead(boton2) == LOW) {
    if ((millis() - lastDebounceTime2) > debounceDelay && !boton2Presionado && CreditoAcum>=12 && !voidcompras) {
      Serial.println("Boton 2 presionado");
      Serial.println("Dispensando 2do producto");
      Serial.print("Credito Total: $ ");
      Serial.print(CreditoAcum);
      Serial.println(".00");
      agua = true;
      boton2Presionado = true;
      voidcompras = true;
      bug2 = true;
      Serial.print("valor bug2: ");
      Serial.println(bug2);
      lastDebounceTime2 = millis();
      CreditoAcum-=12;
      digitalWrite(rele2,LOW);
      digitalWrite(ondec,HIGH);
      Serial.println("desactivando el coin detector");
      iniciardispensado();
    }
    //NEGACIÓN DE COMPRA POR CREDITOS

    if ((millis() - lastDebounceTime2) > debounceDelay && !boton2Presionado && CreditoAcum<12){
      Serial.println("Boton 2 presionado");
      Serial.println("Creditos insuficientes");
      boton2Presionado = true;
      lastDebounceTime2 = millis();
      creditosinsuficientes(); 
    }
  } else {
    boton2Presionado = false;
  }
  
  // Botón 3
  if (digitalRead(boton3) == LOW) {
    if ((millis() - lastDebounceTime3) > debounceDelay && !boton3Presionado && CreditoAcum>=14 && !voidcompras) {
      Serial.println("Boton 3 presionado");
      Serial.println("Dispensando 3er producto");
      Serial.print("Credito Total: $ ");
      Serial.print(CreditoAcum);
      Serial.println(".00");
      agua = true;
      boton3Presionado = true;
      voidcompras = true;
      bug2 = true;
      lastDebounceTime3 = millis();
      CreditoAcum-=14;
      digitalWrite(rele3,LOW);
      digitalWrite(ondec,HIGH);
      Serial.println("desactivando el coin detector");
      iniciardispensado();
    }
    //NEGACIÓN DE COMPRA POR CREDITOS

    if ((millis() - lastDebounceTime3) > debounceDelay && !boton3Presionado && CreditoAcum<14){
      Serial.println("Boton 3 presionado");
      Serial.println("Creditos insuficientes");
      boton3Presionado = true;
      lastDebounceTime3 = millis();
      creditosinsuficientes(); 
    }
  } else {
    boton3Presionado = false;
  }

  // Botón 4
  if (digitalRead(boton4) == LOW) {
    if ((millis() - lastDebounceTime4) > debounceDelay && !boton4Presionado && CreditoAcum>=16 && !voidcompras) {
      Serial.println("Boton 4 presionado");
      Serial.println("Dispensando 4to producto");
      Serial.print("Credito Total: $ ");
      Serial.print(CreditoAcum);
      Serial.println(".00");
      agua = true;
      boton4Presionado = true;
      voidcompras = true;
      bug2 = true;
      lastDebounceTime4 = millis();
      CreditoAcum-=16;
      digitalWrite(rele4,LOW);
      digitalWrite(ondec,HIGH);
      Serial.println("desactivando el coin detector");
      iniciardispensado();
    }
    //NEGACIÓN DE COMPRA POR CREDITOS

    if ((millis() - lastDebounceTime4) > debounceDelay && !boton4Presionado && CreditoAcum<16){
      Serial.println("Boton 4 presionado");
      Serial.println("Creditos insuficientes");
      boton4Presionado = true;
      lastDebounceTime4 = millis();
      creditosinsuficientes(); 
    }
  } else {
    boton4Presionado = false;
  }

  // Botón 5
  if (digitalRead(boton5) == LOW) {
    if ((millis() - lastDebounceTime5) > debounceDelay && !boton5Presionado && CreditoAcum>=18 && !voidcompras) {
      Serial.println("Boton 5 presionado");
      Serial.println("Dispensando 5to producto");
      Serial.print("Credito Total: $ ");
      Serial.print(CreditoAcum);
      Serial.println(".00");
      agua = true;
      boton5Presionado = true;
      voidcompras = true;
      bug2 = true;
      lastDebounceTime5 = millis();
      CreditoAcum-=18;
      digitalWrite(rele5,LOW);
      digitalWrite(ondec,HIGH);
      Serial.println("desactivando el coin detector");
      iniciardispensado();
    }
    //NEGACIÓN DE COMPRA POR CREDITOS

    if ((millis() - lastDebounceTime5) > debounceDelay && !boton5Presionado && CreditoAcum<12){
      Serial.println("Boton 5 presionado");
      Serial.println("Creditos insuficientes");
      boton5Presionado = true;
      lastDebounceTime5 = millis();
      creditosinsuficientes(); 
    }
  } else {
    boton5Presionado = false;
  }

  // Botón 6
  if (digitalRead(boton6) == LOW) {
    if ((millis() - lastDebounceTime6) > debounceDelay && !boton6Presionado && !voidcompras && CreditoAcum>0) {
      Serial.println("Boton 6 presionado");
      Serial.println("Devolviendo cambio");
      Serial.print("Credito Total: $ ");
      Serial.print(CreditoAcum);
      Serial.println(".00");
      //FALTA IMPLEMENTAR EL NUEVO CODIGO
      lastDebounceTime6 = millis();
    }
  } else {
    boton6Presionado = false;
  }
}


//FUNCIÓN PARA ACTUALIZAR EL LCD
void actualizarCreditos() {
 if (millis() - lastUpdateLCD >= lcdUpdateDelay) {
    if (actualizandoCreditos && CreditoAcum != creditosPrev) {
      mostrarCreditos();
      creditosPrev = CreditoAcum;
    }
    lastUpdateLCD = millis();
  }
}

//FUNCIÓN REPARADORA ULTRAMAMADISIMA DESTRUTORA DE QA´S TERRENEITOR 3000X

void antibugs() 
{  
 // Serial.println("entro al voidantibugs");

 // Serial.print("valor bug1: ");
 // Serial.println(bug1);
 // Serial.print("valor bug2: ");
 // Serial.println(bug2);
  
  if(millis()-reseteo>=5000 && bug2==true)
   {
    Serial.println("Ha pasado 1 segundo y bug2 es true");
    bug1=false;
    bug2=false;
    activarAntibugs = false;
    digitalWrite(ondec,LOW);
    Serial.println("Restablecimiento de valores si no se detecta el problema");
    Serial.println("Además se prende el coin detector");
  }
  
 // if(bug1==true && bug2==true && CreditoAcum>0)
  //{
    //CreditoAcum=0;
    //bug1=false;
    //bug2=false;
    //activarAntibugs = false;
    //digitalWrite(ondec,LOW);
    //Serial.println("Problema solucionado");
    //Serial.println("Además se prende el coin detector");
  //} activar esto solo si el credito se va a entregar al instante luego de hacer la compra y no necesitara un boton
}

void iniciardispensado(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispensando");
  lcd.setCursor(0, 1);
  lcd.print("Progreso: 0%   ");
  progreso = 0;
  actualiza = millis();
  dispensado = true;
  actualizandoCreditos = false; 
}

void actualizarbarra()
{   
    progreso = waterFlow*100;
    lcd.setCursor(10, 1); 
    lcd.print(progreso);
    lcd.print("%   "); 

    if (progreso >= 99) 
    {
      dispensando = false;
      dispensado = false;
      Serial.println("Se quita la barra de carga"); 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Listo!");
      Serial.println(waterFlow);
      delay(1000);
      actualizandoCreditos = true; 
      mostrarCreditos();
      Serial.println("Se vuelven a mostrar los creditos");
    }
}
