// Canal A (Motor 1)
#define AIN1 6
#define AIN2 7
#define PWMA 11

// Canal B (Motor 2)
#define BIN1 5
#define BIN2 4
#define PWMB 3

// Pin standby (activar chip)
#define STBY 9

#include <Wire.h>
#include <Adafruit_VL53L0X.h>

// --- Sensor ---
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(9600);

  // Pines motores
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH); // activar chip

  // Iniciar sensor
  if (!lox.begin()) {
    Serial.println("Error al iniciar VL53L0X");
    while (1);
  }
  Serial.println("VL53L0X listo");
}

// Funciones para motores
void motorA(int speed, boolean dir) {
  digitalWrite(AIN1, dir);
  digitalWrite(AIN2, !dir);
  analogWrite(PWMA, speed);
}

void motorB(int speed, boolean dir) {
  digitalWrite(BIN1, dir);
  digitalWrite(BIN2, !dir);
  analogWrite(PWMB, speed);
}

void ambosMotores(int speed, boolean dir) {
  motorA(speed, dir);
  motorB(speed, dir);
}

void detenerMotores() {
  motorA(0, true);
  motorB(0, true);
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) { // medición válida
    int distancia = measure.RangeMilliMeter / 10; // en cm
    Serial.print("Distancia: ");
    Serial.print(distancia);
    Serial.println(" cm");

    if (distancia > 20) {
      // Avanzar
      ambosMotores(200, true);
    } else {
      // Obstáculo: detener y retroceder un poco
      detenerMotores();
      delay(500);

      ambosMotores(200, false); // atrás
      delay(1000);

      detenerMotores();
      delay(500);
    }
  } else {
    Serial.println("Error de medición");
    detenerMotores();
  }

  delay(100);
}



