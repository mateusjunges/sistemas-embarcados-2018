#include <Servo.h>

#define pinoServo0 8
#define pinoServo1 7
#define pinoServo2 4

Servo servo0, servo1, servo2;  // Criacao de um objeto servo para controle

int pinoDIG = 12; // Pino digital para controle de um dos servos

int pinoPOT = 0; // Potenciometro para controle dos servos   

int minServo[3] = {40, 30, 20};
int maxServo[3] = {80, 60, 45};
int valorServo[3] = {41, 31, 21};
int incremento[3] = {1, 1, 1};

void setup() {
  pinMode(pinoServo0, OUTPUT);
  pinMode(pinoDIG, OUTPUT);
  pinMode(pinoServo1, OUTPUT);
  pinMode(pinoServo2, OUTPUT);
  servo0.attach(pinoServo0);  // Relaciona o pino com o Servo
  servo1.attach(pinoServo1);  // Relaciona o pino com o Servo
  servo2.attach(pinoServo2);  // Relaciona o pino com o Servo
}

void loop() {
  
  /*
  * Graus de Liberdade:
  * Servo 0: 40 -> 80
  * Servo 1: 30 -> 60
  * Servo 2: 20 -> 45
  */
  
  int leitura = analogRead(pinoPOT); // Leitura do valor analogico de entrada
  int valServo1 = map(leitura, 0, 1023, 0, 2000);// Valor para movimentação do servo1 (0 .. 2000 micro segundos)
  servoPulse(pinoDIG, valServo1); // Move servo2 para posicao desejada  
  
  movServo(0, pinoServo0); 
  movServo(1, pinoServo1);
  movServo(2, pinoServo2);
}
 
void servoPulse (int servo, int valor) {
  int ciclo = valor + 500; // Ajusta o valor do tempo para posicionar servo2      
  digitalWrite(servo, HIGH); // Eleva o sinal para 1
  delayMicroseconds(ciclo); // Aguarda o periodo do ciclo
  digitalWrite(servo, LOW); // Retorna o nivel do sinal a 0
  delay(20); // Periodo total 20ms 50Hz
}


/*
* servo é o número do servo onde vou mexer
* myServo é o pino digital onde o servo esta colocado
*/
void movServo(int servo, int myServo){
  //Se chegou no valor máximo ou mínimo, eu altero o sinal do incremento
  if(maxServo[servo] == valorServo[servo] || minServo[servo] == valorServo[servo]){
    incremento[servo] = incremento[servo] * -1;
  }
  
  valorServo[servo] += incremento[servo]; //valor usado no servo.write
  
  /*
  * Verifica qual é o servo onde escrever:
  */
  switch (servo){
    case 0: 
      servo0.write(valorServo[servo]);
      break;
    case 1: 
      servo2.write(valorServo[servo]);
      break;
    case 2: 
      servo1.write(valorServo[servo]);
      break;
  }
    
}

