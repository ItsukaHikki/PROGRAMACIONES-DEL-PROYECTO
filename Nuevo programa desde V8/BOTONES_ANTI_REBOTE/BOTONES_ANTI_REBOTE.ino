//BOTONES
const int buttonPins[] = {13, 12, 11, 9, 7, 5}; // Pines de los botones aqui tonoto
const int numButtons = sizeof(buttonPins) / sizeof(buttonPins[0]);
bool buttonStates[numButtons]; // Aqui namas van los estados
bool lastButtonStates[numButtons]; // El último estado de los botones
unsigned long lastDebounceTime[numButtons]; // La última vez que el pin cambió de estado
const unsigned long debounceDelay = 50; // Tiempo de debounce (50 ms)

void setup() {
  Serial.begin(9600); 
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP); // Configurar los pines con INPUT_PULLUP PARA evitar alguna falla
    buttonStates[i] = digitalRead(buttonPins[i]);
    lastButtonStates[i] = buttonStates[i];
    lastDebounceTime[i] = 0;
  }
}

void loop() 
{
  for (int i = 0; i < numButtons; i++) {
    int reading = digitalRead(buttonPins[i]); // Lectura del estado del boton
    
    // Si cambia un estado,se reinicia el temporizador de debounce
    if (reading != lastButtonStates[i]) {
      lastDebounceTime[i] = millis();
    }

    // Si ha pasado el tiempo de debounce, se debe de considerar el estado del botón como estable/continuo 
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      // Si el estado ha cambiado
      if (reading != buttonStates[i]) {
        buttonStates[i] = reading;

        // Solo imprimir si el botón está en estado HIGH (presionado)
        if (buttonStates[i] == HIGH) {
          ejecutarAccion(i);  // Llama a la función correspondiente según el botón presionado
        }
      }
    }

    // Guardar el estado del botón para la próxima lectura
    lastButtonStates[i] = reading;
  }
}

// Funciones para ejecutar una acción según el botón presionado
void ejecutarAccion(int buttonIndex) {
  switch (buttonIndex) {
    case 0:
    if(CreditoAcum >= 13)//Pin13
    {
      CreditoAcum -=10; 
    }
    else 
    {
      Serial.println ("No hay suficientes creditos");
    }
      break;
      
    case 1:
     if (CreditoAcum >= 12) //Pin12
     {
      CreditoAcum -=12;
     }
     else
     {
      Serial.println("No hay Suficientes creditos");
     }
      break;
      
    case 2:
     if (CreditoAcum >= 11) //Pin11
     {
      CreditoAcum -=11;
     }
     else
     {
      Serial.println("No hay suficientes creditos");
     }
      break;
      
    case 3:
      if (CreditoAcum >= 9) //Pin 9
      {
        CreditoAcum -=9;
      }
      else
      {
        Serial.println("No hay suficientes creditos");
      }
      break;

    case 4:
      if (CreditoAcum >=7) //Pin 7
      {
        CreditoAcum -=7;
      }
      else
      {
        Serial.println("No hay suficientes creditos");
      }
      break;

    case 5:
      if(CreditoAcum >=5) //Pin5
      {
        CreditoAcum -=5;
      }
      else
      {
        Serial.println("No hay suficientes creditos");
      }
      break;
      
    default:
      Serial.println("Error: Botón desconocido");
      break;
  }
}
