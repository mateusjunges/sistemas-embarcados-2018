#include <Servo.h>

#define numero_servos 3

int pino_servos[3] = {8, 7, 4};

Servo servos[3];  // Criacao de um objeto servo para controle

int pinoDIG = 12; // Pino digital para controle de um dos servos

int pinoPOT = 0; // Potenciometro para controle dos servos   

int minServo[3] = {40, 30, 20};
int maxServo[3] = {80, 60, 45};
int valorServo[3] = {41, 31, 21};
int incremento[3] = {1, 1, 1};

void setup() {
  for(int n = 0; n < numero_servos; n++){
  	pinMode(pino_servos[n], OUTPUT);
  }
  pinMode(pinoDIG, OUTPUT);
  
  //Relaciona os servos com seus devidos pinos:
  for(int n = 0; n < numero_servos; n++){
  	servos[n].attach(pino_servos[n]);
  }
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
  
  movServo(0, pino_servos[0]); 
  movServo(1, pino_servos[1]);
  movServo(2, pino_servos[2]);
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
  servos[servo].write(valorServo[servo]);
  	
}
