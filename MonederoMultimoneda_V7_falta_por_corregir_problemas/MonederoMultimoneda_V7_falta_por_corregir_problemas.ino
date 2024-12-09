#include <LiquidCrystal_I2C.h>
#include <PinChangeInterrupt.h>
#include <Wire.h>
unsigned long when=0;
// Definición de los pines de los LEDs
const int pinLED_10 = 22; // LED para moneda de 10 pesos
const int pinLED_5 = 23;  // LED para moneda de 5 pesos
const int pinLED_2 = 24;   // LED para moneda de 2 pesos
const int pinLED_1 = 25;   // LED para moneda de 1 peso

//Constantes para los pines.
const int SignalPin = 2;
const int bcancelar = 13;
const int bcloro = 12; //precio 10

//estados para realizar acciones
unsigned long botones;
int bestadoc = 0; 
int agua= false;
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
volatile float waterFlow; // Cambiado a float
const int sensorPin = 10;
int r1=8;

bool bug=false;
bool sapo=false;


unsigned long reseteo = 0;

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
    // Configuración de los pines de los LEDs como salidas
  pinMode(pinLED_10, OUTPUT);
  pinMode(pinLED_5, OUTPUT);
  pinMode(pinLED_2, OUTPUT);
  pinMode(pinLED_1, OUTPUT);


  // Inicialización de la comunicación serial para depuración
  Serial.begin(9600);
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
 
  // Cada vez que insertamos una moneda valida, incrementamos el contador de monedas y encendemos la variable de control,
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
  
//Loop de compras 
  if(millis()-botones >= 200) 
{
//Función de compra del cloro
if (digitalRead(bcloro) == HIGH && bestadoc == 0 && CreditoAcum>=10) 
{
CreditoAcum = CreditoAcum - 10;
cambio();
if(CreditoAcum < 0)
{
  CreditoAcum = 0; 
}
digitalWrite(r1,LOW);
Serial.print("Credito Total: $ ");
Serial.print(CreditoAcum);
Serial.println(".00");
botones=millis();
bestadoc=1;
lcd.clear();
agua=true;
sapo=true;
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
  botones=millis();
  bestadoc=1;
  lcd.clear();
}
//Función antirrebote del botón 
if (bestadoc==1 && digitalRead(bcancelar)==LOW){bestadoc=0;} //cancelamiento de compra
}

//Función para poner Fin al despacho del producto
 if(waterFlow>=1)
  {
    waterFlow=0;
    digitalWrite(r1,HIGH);
    agua=false;
    bug=true;
    reseteo=millis();
    Serial.print("reseteo: ");
    Serial.println(reseteo);
    Serial.print("millis(): ");
    Serial.println(millis());
  }


  if(millis()-reseteo>=5000 && bug==true)
   {
    Serial.println("Han pasado 15 segundos y sapo es true");
    bug=false;
    sapo=false;
    Serial.println("Restablecimiento de valores si no se detecta el problema");
  }
  if(millis()- when>=1000)
  {
  Serial.print("Tiempo actual (millis()): ");
  Serial.println(millis());
  Serial.print("Tiempo de reseteo: ");
  Serial.println(reseteo);
  Serial.print("Diferencia de tiempo: ");
  Serial.println(millis() - reseteo);
  when=millis();
  }
  
  if(bug==true && sapo==true && CreditoAcum>0)
  {
    CreditoAcum=0;
    bug=false;
    sapo=false;
    Serial.println("tranquilo papu yo soluciono el error");
  }

  


}

void cambio() 
{
  if(CreditoAcum>=0){
  // Array de monedas disponibles
  int monedas[] = {10, 5, 2, 1};
  int num_monedas = sizeof(monedas) / sizeof(monedas[0]); // Calcula el número de monedas disponibles

  int credito_restante = CreditoAcum; // Crédito restante (debe ser igualado al crédito acumulado)

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
    switch (monedas[i]) 
    {
      case 10:
        flashLED(pinLED_10, veces);
        break;
      case 5:
        flashLED(pinLED_5, veces);
        break;
      case 2:
        flashLED(pinLED_2, veces);
        break;
      case 1:
        flashLED(pinLED_1, veces);
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
}

void flashLED(int pinLED, int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pinLED, HIGH); // Enciende el LED
    delay(500);                  // Espera 0.5 segundos
    digitalWrite(pinLED, LOW);   // Apaga el LED
    delay(500);                  // Espera 0.5 segundos
  }
}



// Función para actualizar el crédito acumulado
void actualizarCreditoAcumulado(int credito_restante) {
  CreditoAcum=0;
}
