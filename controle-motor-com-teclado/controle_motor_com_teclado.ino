#include <Keypad.h> //Biblioteca Keypad

#define pino_ponte_H 3
#define pino_ponte_H_1 6
#define pino_led_horario 13
#define pino_led_anti_horario 2
#define pino_potenciometro A5
const byte numLinhas = 4; // Linhas do teclado
const byte numColunas = 4; // Colunas do teclado
char teclaPressionada;
char tecla_anterior; 

//Matriz de caracteres do teclado
char matrizTeclas[numLinhas][numColunas] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pinosLinhas[numLinhas] = {12, 11, 10, 9}; //pinos das linhas
byte pinosColunas[numColunas] = {8, 7, 5, 4}; //pinos das colunas

//Inicializacao do teclado
Keypad teclado = Keypad( makeKeymap(matrizTeclas), pinosLinhas, pinosColunas, numLinhas, numColunas); 


void setup(){
  Serial.begin(9600);
  Serial.println("Pressione uma tecla...");
  Serial.println();
}
  
void loop(){
  int leitura_pot = analogRead(pino_potenciometro);
  int leitura_pot_map = map(leitura_pot, 0, 1023, 0, 255);
  
  teclaPressionada = teclado.getKey(); //VERIFICA SE ALGUMA DAS TECLAS FOI PRESSIONADA
  if(teclaPressionada != NO_KEY)
    tecla_anterior = teclaPressionada;
  
  if(teclaPressionada == '1' || tecla_anterior == '1')
    girar_sentido_horario(leitura_pot_map);
  else if (teclaPressionada == '2' || tecla_anterior == '2')
    girar_sentido_anti_horario(leitura_pot_map);
  else if (teclaPressionada == '0' || tecla_anterior == '0')
    parar();
  
}

void girar_sentido_horario(int potencia){
  digitalWrite(pino_ponte_H, LOW);
    analogWrite(pino_ponte_H_1, potencia);
    digitalWrite(pino_led_horario, HIGH);
    digitalWrite(pino_led_anti_horario, LOW);
}
void girar_sentido_anti_horario(int potencia){
  analogWrite(pino_ponte_H, potencia);
    digitalWrite(pino_ponte_H_1, LOW);
    digitalWrite(pino_led_horario, LOW);
    digitalWrite(pino_led_anti_horario, HIGH);
}
void parar(){
  digitalWrite(pino_ponte_H, LOW);
    digitalWrite(pino_ponte_H_1, LOW);
    digitalWrite(pino_led_horario, LOW);
    digitalWrite(pino_led_anti_horario, LOW);
}
