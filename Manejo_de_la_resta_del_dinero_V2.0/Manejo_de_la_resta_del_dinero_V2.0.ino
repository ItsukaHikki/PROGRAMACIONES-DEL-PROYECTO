// Aqui defini los pines de los LEDS
const int pinLED_10 = 13; // LED para moneda de 10 pesos
const int pinLED_5 = 12;  // LED para moneda de 5 pesos
const int pinLED_2 = 9;  // LED para moneda de 2 pesos
const int pinLED_1 = 8;  // LED para moneda de 1 peso

void setup() 
{
  
  pinMode(pinLED_10, OUTPUT);
  pinMode(pinLED_5, OUTPUT);
  pinMode(pinLED_2, OUTPUT);
  pinMode(pinLED_1, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  int monedas[] = {10, 5, 2, 1}; // Array de monedas disponibles
  int num_monedas = sizeof(monedas) / sizeof(monedas[0]); // Calcula el número de monedas disponibles

  int credito_restante = 23; // Guarda el crédito restante, debo de igualarlo despues a la variable del credito acumulado en el codigo original

  // Itera sobre las monedas disponibles
  for (int i = 0; i < num_monedas; i++) {
    // Calcula cuántas veces se puede restar la moneda actual del crédito restante
    int veces = credito_restante / monedas[i];
    // Actualiza el crédito restante restando la cantidad de monedas utilizadas
    credito_restante -= veces * monedas[i];

    
    Serial.print("Descuento de ");
    Serial.print(veces);
    Serial.print(" moneda(s) de ");
    Serial.print(monedas[i]);
    Serial.println(" pesos");

    // Enciende y apaga el LED correspondiente según la cantidad de veces que se descuente la moneda actual
    switch (monedas[i]) {
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

    // aqui puedo agregar otras funciones con los servormotores 

    // Si ya no queda crédito por descontar, debe de salir del bucle
    if (credito_restante == 0) {
      break;
    }
  }

  // Debo de agregar función de actualizar el crédito acumulado con el crédito restante después del descuento
}

// Función para encender y apagar un LED cierta cantidad de veces
void flashLED(int pinLED, int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pinLED, HIGH); // Enciende el LED
    delay(500); // Espera 0.5 segundos
    digitalWrite(pinLED, LOW); // Apaga el LED
    delay(500); // Espera 0.5 segundos
  }
}
