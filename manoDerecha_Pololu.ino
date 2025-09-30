#include <Wire.h>
#include <VL53L0X.h>

// Objetos para los sensores (Pololu VL53L0X)
VL53L0X sensorF, sensorR, sensorL;

// Pines para XSHUT de cada sensor (todos comparten SDA, SCL, VCC y GND)
int pinSensorF = 10;
int pinSensorL = 11;
int pinSensorR = 12;

// Pines de motores
int pwmMotorA = 9;
int MotorA1 = 8;
int MotorA2 = 7;

int pinSTBY = 6;

int MotorB1 = 5;
int MotorB2 = 4;
int pwmMotorB = 3;

char leer = 0;

// Variable global: distancia mínima para considerar “cerca” (en mm)
int cerca = 40; // 100 mm = 10 cm

void setup() 
{
  Serial.begin(9600);
  Wire.begin();

  // Inicializamos los motores
  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(pwmMotorA, OUTPUT);

  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);
  pinMode(pwmMotorB, OUTPUT);

  // Configuramos pines XSHUT
  pinMode(pinSensorF, OUTPUT);
  pinMode(pinSensorL, OUTPUT);
  pinMode(pinSensorR, OUTPUT);

  // Apagamos todos los sensores
  digitalWrite(pinSensorF, LOW);
  digitalWrite(pinSensorL, LOW);
  digitalWrite(pinSensorR, LOW);
  delay(10);

  // Inicializamos cada sensor uno por uno con dirección diferente
  // 1. Sensor frontal
  digitalWrite(pinSensorF, HIGH);
  delay(10);
  sensorF.init(true);
  sensorF.setAddress(0x30);

  // 2. Sensor izquierdo
  digitalWrite(pinSensorL, HIGH);
  delay(10);
  sensorL.init(true);
  sensorL.setAddress(0x31);

  // 3. Sensor derecho
  digitalWrite(pinSensorR, HIGH);
  delay(10);
  sensorR.init(true);
  sensorR.setAddress(0x32);

  // Configuramos modo de medida continua
  sensorF.startContinuous(50);
  sensorL.startContinuous(55);
  sensorR.startContinuous(60);

  Serial.println("Sensores VL53L0X inicializados con direcciones únicas!");
}

void loop() 
{
  // Leemos distancias en mm
  int distF = sensorF.readRangeContinuousMillimeters();
  int distL = sensorL.readRangeContinuousMillimeters();
  int distR = sensorR.readRangeContinuousMillimeters();

  // Mostramos en serial
  Serial.print("Frente: ");
  Serial.print(distF);
  Serial.print(" mm  |  Izquierda: ");
  Serial.print(distL);
  Serial.print(" mm  |  Derecha: ");
  Serial.print(distR);
  Serial.println(" mm");

  /* Casos de ejemplo:
    1.- Sensor frontal no detecta pared cerca, sensor derecho detecta pared cerca → Avanza
    2.- Los 3 sensores detectan pared cerca (camino sin salida)                   → Media vuelta
    3.- El sensor derecho deja de detectar pared cerca                            → Vuelta a la derecha
  */

  // Estrategia de mano derecha:
  if (distR > cerca) {
    // Giro a la derecha si hay camino
    vueltaDerecha();
    delay(400); // calibrar para 90°
    avanzar();
  }
  else if (distF > cerca) {
    // Si frente está libre, avanzamos
    avanzar();
  }
  else {
    // Todos bloqueados → media vuelta
    mediaVuelta();
    delay(800); // calibrar para 180°
  }

  delay(100); // pequeña pausa entre decisiones
}


// Funciones básicas de movimiento (vacías por ahora)
void avanzar() 
{
  digitalWrite(pinSTBY, HIGH);

  // Motor A hacia adelante
  digitalWrite(MotorA1, HIGH);
  digitalWrite(MotorA2, LOW);
  analogWrite(pwmMotorA, 200);

  // Motor B hacia adelante
  digitalWrite(MotorB1, HIGH);
  digitalWrite(MotorB2, LOW);
  analogWrite(pwmMotorB, 200);

}

void mediaVuelta() 
{
  digitalWrite(pinSTBY, HIGH);

  // Motor A hacia adelante
  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, HIGH);
  analogWrite(pwmMotorA, 200);

  // Motor B hacia adelante
  digitalWrite(MotorB1, HIGH);
  digitalWrite(MotorB2, LOW);
  analogWrite(pwmMotorB, 200);
}

void vueltaDerecha() 
{
  digitalWrite(pinSTBY, HIGH);

  // Motor A hacia adelante
  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, LOW);
  analogWrite(pwmMotorA, 0);

  // Motor B hacia adelante
  digitalWrite(MotorB1, HIGH);
  digitalWrite(MotorB2, LOW);
  analogWrite(pwmMotorB, 200);
}

void vueltaIzquierda() 
{
  digitalWrite(pinSTBY, HIGH);

  // Motor A hacia adelante
  digitalWrite(MotorA1, HIGH);
  digitalWrite(MotorA2, LOW);
  analogWrite(pwmMotorA, 200);

  // Motor B hacia adelante
  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, LOW);
  analogWrite(pwmMotorB, 0);
}

void stop() 
{
  digitalWrite(pinSTBY, LOW);

  analogWrite(pwmMotorA, 0);
  analogWrite(pwmMotorB, 0);
}
