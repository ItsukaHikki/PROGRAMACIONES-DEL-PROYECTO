//Constantes para los pines.
const int SignalPin = 2;

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

void setup() {

// Inicializamos la comunicacion serial, para ver los resultados en el monitor.
Serial.begin(9600);

//Agregamos la interrupcion con el pin indicado.
attachInterrupt(digitalPinToInterrupt(SignalPin), coinInterrupt, RISING);
    
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
}
