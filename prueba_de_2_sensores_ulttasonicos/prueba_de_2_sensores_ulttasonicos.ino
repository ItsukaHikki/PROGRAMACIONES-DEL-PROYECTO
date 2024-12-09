#define TRIG_PIN1 26 // Pin GPIO26 de la ESP32 conectado al pin TRIG del primer sensor ultrasónico
#define ECHO_PIN1 25 // Pin GPIO25 de la ESP32 conectado al pin ECHO del primer sensor ultrasónico
#define TRIG_PIN2 23 // Pin GPIO23 de la ESP32 conectado al pin TRIG del segundo sensor ultrasónico
#define ECHO_PIN2 22 // Pin GPIO22 de la ESP32 conectado al pin ECHO del segundo sensor ultrasónico
#define LED_PIN 17   // Pin GPIO17 de la ESP32 conectado al LED

float duration_us1, distance_cm1, duration_us2, distance_cm2;

void setup() {
    Serial.begin(9600);
    pinMode(TRIG_PIN1, OUTPUT);
    pinMode(ECHO_PIN1, INPUT);
    pinMode(TRIG_PIN2, OUTPUT);
    pinMode(ECHO_PIN2, INPUT);
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // Medición para el primer sensor
    digitalWrite(TRIG_PIN1, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN1, LOW);
    duration_us1 = pulseIn(ECHO_PIN1, HIGH);
    distance_cm1 = 0.017 * duration_us1;

    // Medición para el segundo sensor
    digitalWrite(TRIG_PIN2, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN2, LOW);
    duration_us2 = pulseIn(ECHO_PIN2, HIGH);
    distance_cm2 = 0.017 * duration_us2;

    // Enciende o apaga el LED según la distancia medida
    if (distance_cm1 < 50 || distance_cm2 < 50) {
        digitalWrite(LED_PIN, HIGH); // Enciende el LED
    } else {
        digitalWrite(LED_PIN, LOW); // Apaga el LED
    }

    // Imprime los valores en el Monitor Serie
    Serial.print("Distancia 1: ");
    Serial.print(distance_cm1);
    Serial.println(" cm");
    Serial.print("Distancia 2: ");
    Serial.print(distance_cm2);
    Serial.println(" cm");

    delay(500);
}
