#define PIN_BOTAO1  0
#define PIN_BOTAO2  2
#define pinoMUX     15
#define PIN_S0      12
#define PIN_S1      13
#define PIN_S2      9

#define PIN_BUTTON_A 16 //Pino botao 00
#define PIN_BUTTON_B 5 //pino botao 01
#define PIN_BUTTON_C 10 // pino botao 02

int acender[3] = {LOW, LOW, LOW};

typedef struct // Cria uma STRUCT para armazenar os dados dos botoes
{
    int pino; 
    int canal = -1;
    int valor;
    int statusBotao;
    int statusBotaoAnterior = LOW;    
    unsigned long debounceAnterior = 0;
} Botao;

Botao botao1;
Botao botao2;

Botao botao_A;
Botao botao_B;
Botao botao_C;

int val[8] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};

//Controle dos canais do multiplexador
int pins[3] = {PIN_S0, PIN_S1, PIN_S2};

void muxWrite(int canal) {
  for (int i = 0; i < 3; i++)  {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], bitRead(canal, i));
    digitalWrite(pinoMUX, val[canal]);
  }
}

void setup() {
  Serial.begin(115200);
  botao1.pino = PIN_BOTAO1;
  botao1.canal = 0;
  botao2.pino = PIN_BOTAO2;
  botao2.canal = 3;
  botao_A.pino = PIN_BUTTON_A;
  botao_B.pino = PIN_BUTTON_B;
  botao_C.pino = PIN_BUTTON_C;
 
  pinMode(botao1.pino, INPUT_PULLUP);
  pinMode(botao2.pino, INPUT_PULLUP);
  pinMode(pinoMUX, OUTPUT);
  pinMode(botao_A.pino, INPUT_PULLUP);
  pinMode(botao_B.pino, INPUT_PULLUP);
  pinMode(botao_C.pino, INPUT_PULLUP);
}

void loop() {
  botao1 = debounce(botao1);
  botao2 = debounce(botao2);
  
  botao_A = debounce(botao_A);
  botao_B = debounce(botao_B);
  botao_C = debounce(botao_C);

  int ctr = botao_A.valor << 2 | botao_B.valor << 1 | botao_C.valor;

  val[ctr] = !val[ctr];
  Serial.print("canal");
  Serial.print(ctr);
  
  for (int i = 0; i < 8; i++) {
    muxWrite(i);
    delay(1);
  }
}



Botao debounce(Botao b) {
  int leitura = digitalRead(b.pino);
  if (leitura != b.statusBotaoAnterior) {
    b.debounceAnterior = millis();
  }
  if ((millis() - b.debounceAnterior) > 50) {
    if (leitura != b.statusBotao) {
      b.statusBotao = leitura;
      if (b.statusBotao == HIGH) {
        b.valor = !b.valor;
        val[b.canal] = !val[b.canal];
        Serial.print("Canal: ");
        Serial.print(b.canal);
        Serial.print(" = ");
        Serial.println(val[b.canal]);
      }
    }
  }
  b.statusBotaoAnterior = leitura;
  return b;
}
