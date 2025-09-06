int MotorA1 = 5;
int MotorA2 = 4;
int pwmMotorA = 3;

int MotorB1 = 9;
int MotorB2 = 8;
int pwmMotorB = 10;
int pinSTBY = 6;
char leer = 0;


void setup() {
  Serial.begin(9600);

  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(pwmMotorA, OUTPUT);

  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);
  pinMode(pwmMotorB, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    leer = Serial.read();
    switch (leer) {
      case 'a':
        adelante();
        Serial.println("Adelante");
        break;
      case 'b':
        atras();
        Serial.println("Atras");
        break;
      case 'c':
        giroIzquierda();
        Serial.println("Giro a la izquierda");
        break;
      case 'd':
        giroDerecha();
        Serial.println("Giro a la derecha");
        break;
      case 'e':
        detener();
        Serial.println("Detener");
        break;
    }
  }
}
