#include <Wire.h>
#include <Adafruit_VL53L0X.h>

// Crea el objeto del sensor
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando sensor VL53L0X...");

  if (!lox.begin()) {
    Serial.println("Error al iniciar VL53L0X!");
    while (1);
  }
  Serial.println("Sensor VL53L0X iniciado correctamente!");
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;

  // Tomar medición
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {  // 4 fuera de rango
    Serial.print("Distancia: ");
    Serial.print(measure.RangeMilliMeter);
    Serial.println(" mm");
  } else {
    Serial.println("Fuera de rango");
  }

  delay(500); // medio s
}

