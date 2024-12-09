void setup()
{
  Serial.begin(9600);

}

void loop() 
{
  
    int monedas[] = {10, 5, 2, 1}; // Array de monedas disponibles
    int num_monedas = sizeof(monedas) / sizeof(monedas[0]); // Calcula el número de monedas disponibles

    int credito_restante = 13; // Guarda el crédito restante que se debe descontar

    // Itera sobre las monedas disponibles
    for (int i = 0; i < num_monedas; i++) {
        // Calcula cuántas veces se puede restar la moneda actual del crédito restante
        int veces = credito_restante / monedas[i];
        // Actualiza el crédito restante restando la cantidad de monedas utilizadas
        credito_restante -= veces * monedas[i];

        // Imprime información en el puerto serie
        Serial.print("Descuento de ");
        Serial.print(veces);
        Serial.print(" moneda(s) de ");
        Serial.print(monedas[i]);
        Serial.println(" pesos");

        // Aquí podrías realizar acciones adicionales, como devolver monedas físicas si fuera necesario

        // Si ya no queda crédito por descontar, sal del bucle
        if (credito_restante == 0) {
            break;
        }
    }

    // Actualiza el crédito acumulado con el crédito restante después del descuento



}
