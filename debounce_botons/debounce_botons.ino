const int buttonPins[] = {13, 12, 11, 9, 7, 5}; // Pines de los botones
const int numButtons = sizeof(buttonPins) / sizeof(buttonPins[0]);
bool buttonStates[numButtons]; // Estado actual de los botones
bool lastButtonStates[numButtons]; // Último estado de los botones
unsigned long lastDebounceTime[numButtons]; // Última vez que el pin cambió de estado
const unsigned long debounceDelay = 50; // Tiempo de debounce (50 ms)

void setup() {
  Serial.begin(9600); // Inicializar comunicación serial
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT); // Configurar los pines de los botones como entrada
    buttonStates[i] = digitalRead(buttonPins[i]);
    lastButtonStates[i] = buttonStates[i];
    lastDebounceTime[i] = 0;
  }
}

void loop() {
  for (int i = 0; i < numButtons; i++) {
    int reading = digitalRead(buttonPins[i]); // Leer el estado del botón
    
    // Si el estado ha cambiado, reiniciar el temporizador de debounce
    if (reading != lastButtonStates[i]) {
      lastDebounceTime[i] = millis();
    }

    // Si ha pasado el tiempo de debounce, considerar el estado del botón como estable
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      // Si el estado ha cambiado
      if (reading != buttonStates[i]) {
        buttonStates[i] = reading;

        // Solo imprimir si el botón está en estado HIGH (presionado)
        if (buttonStates[i] == HIGH) {
          Serial.print("Botón en pin ");
          Serial.print(buttonPins[i]);
          Serial.println(" presionado");
        }
      }
    }

    // Guardar el estado del botón para la próxima lectura
    lastButtonStates[i] = reading;
  }
}
