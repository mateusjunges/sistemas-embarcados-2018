//Libraries:
#include <Ultrasonic.h>

//Defines:
#define TRIGGER 16 //Pino Trigger do sensor HCSR04
#define ECHO    05 //Pino ECHO do sensor HCSR04
#define leftMotorVelocity  6 //Velocidade do motor da esquerda
#define rightMotorVelocity 6 //Velocidade do motor da direita
#define leftMotorPin1 1
#define rightMotorPin1 1
#define leftMotorPin2 1
#define rightMotorPin2 1

//Variables:
float distance; // Grava a distância do objeto mais próximo em frente ao sensor
long duration;  // Grava o tempo de reflexão do som quando bate em um objeto
const int secureDistance = 35; //Máxima distância que é permitida o carrinho chegar de um objeto
const int _backTime_ = 700; //Tempo que fica indo para trás, em milisegundos
//Sensor HC-SR04
Ultrasonic ultrasonicSensor(TRIGGER, ECHO);

//Configuration:
void setup() {
  Serial.begin(115200);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  delay(3000); //Tempo para colocar o carrinho no chão
}

/* Retorna a distância do carrinho em relação ao objeto mais próximo */
float getDistance(){
  return ultrasonicSensor.convert(ultrasonicSensor.timing(), Ultrasonic::CM);
}

void turnRight(){
    //Motor esquerdo para frente:
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
    
    //Motor lado direito para trás:
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);  
}

void goBack(){
  //Colocar o motor do lado esquerdo para andar para trás:
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
    //Colocar o motor do lado direito para andar para trás:
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
    delay(_backTime_);  //Tempo que o carrinho fica indo para trás 
}

void changeVelocity(int v1, int v2){
    //Alterar a velocidade do motor do lado esquerdo:
    analogWrite(leftMotorVelocity, v1);
    //Alterar a velocidade do motor do lado direito:
    analogWrite(rightMotorVelocity, v2);  
}
void goAhead(){
  //Motor lado esquerdo para frente:
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  //Motor lado direito para frente:
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);  
}

//Main Loop:
void loop() {
  //Read de ultrasonic distance:
  distance = getDistance();
  if (distance <= secureDistance){ 
                //Se a distância do carrinho em 
                //relação a um objeto for menor que uma distancia
                //definida:
    changeVelocity(3, 3);
    goBack();
    turnRight();
  }else { //Se a distância não for menor que a distância definida:
     changeVelocity(6, 6); 
     goAhead();
  }  
}



