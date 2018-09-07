#include <ESP8266WiFi.h>
#include <SSD1306Wire.h>
#include <AM2320.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#define SDA_PIN       4 //pino do SDA
#define SCL_PIN       5 //pino do SCL
#define ENDERECO_OLED 0x3C //endereço do display OLED
#define TAMANHO       GEOMETRY_128_32 //geometria do display (pode possuir 64 colunas)
#define TIME_ZONE -3 // Time zone para horário de brasília

//Botão para troca dos dados no display:
#define CHANGE_BUTTON 0

//Configurações de conexão na rede WiFi:
const char* ssid = "JUNGES1";
const char* password = "embarcados";

//Variáveis utilizadas no programa:
unsigned long count; // armazena o valor dos milisegundos até o próximo intervalo
unsigned long interval = 1000; //Tempo em milisegundos do intervalo
float temperature;  //temperatura
float humity;       //humidade
const int max_time = 30000; //tempo máximo para tentar conectar no wifi
bool showtemperature = false;
bool showhours = false;
bool estadoDisplay = false;

int outputState = HIGH;      // estado atual da saída
int buttonState;             // estado do botão no pino CHANGE_BUTTON
int lastButtonState = LOW;   // estado anterior lido do botão no pino CHANGE_BUTTON

//As duas próximas variáveis são unsigned long por conta do tempo, medido
//em milisegundos, e rapidamente se tornam um número maior do que o que pode ser armazenado em um int
unsigned long lastDebounceTime = 0;  // tempo decorrido desde que a saída mudou pela ultima vez
unsigned long debounceDelay = 10;    // tempo de debounce

SSD1306Wire display(ENDERECO_OLED, SDA_PIN, SCL_PIN, TAMANHO); // SDA, SCL -> Configuracao do display SSD1306

//Horas:
WiFiUDP ntpUDP; // Cliente UDP para o NTP
NTPClient horaCliente(ntpUDP, "pool.ntp.org", TIME_ZONE * 3600, 60000); // Config do Cliente NTP

//Temperatura:
AM2320 sensor; // Cria uma instancia do sensor AM2320

void setup() {//config

  pinMode(CHANGE_BUTTON, INPUT_PULLUP);

  Serial.begin(115200); //inicia comunicação serial

  display.init(); //inicia o display OLED
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_24);
  display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Conectando");
  display.display();

  //Conexão com WiFi:
  int waiting = 0; //máximo de 10 segundos para conectar no WiFi
  WiFi.begin(ssid, password);
  Serial.println("Conectando a ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  // Conectou, então mostra o IP da conexao no display OLED
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(display.getWidth() / 2, 18, "IP: " + WiFi.localIP().toString());
  display.display();
  Serial.println("IP: ");
  Serial.print(WiFi.localIP());

  horaCliente.begin();
  sensor.begin(SDA_PIN, SCL_PIN); // Conecta o sensor AM2320 ao barramento I2C - SDA, SCL
  count = millis(); // Inicializa o contador para o intervalo
  Serial.println("Config OK");
}

void loop() {
  if(digitalRead(CHANGE_BUTTON) == LOW){
    estadoDisplay = !estadoDisplay;
  }
  changeDisplay();
}

//Função responsável pelas horas:
void showHours() {
  String infoDisplay;
  if (millis() - count > interval) {
    horaCliente.update(); // Atualiza a hora no sistema utilizando o servidor NTP
    infoDisplay = horaCliente.getFormattedTime();
    // Mostra hora na porta Serial
    Serial.println(infoDisplay); // Imprime informacao formatada na serial
    // Mostra hora no display OLED
    display.clear();
    display.drawRect(0, 0, display.getWidth() - 1, display.getHeight() - 1);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawString(display.getWidth() / 2,  0, infoDisplay);
    display.setFont(ArialMT_Plain_10);
    display.drawString(display.getWidth() / 2, 16, "IP: " + WiFi.localIP().toString());
    display.display();
    count = millis();
  }
}

//Função responsável pela temperatura:
void showTemperature() {
  String infoDisplay; // Variavel que armazena a informacao a ser mostrada no display ou porta serial
  char strDisplay[30]; // Variavel auxiliar para armazenar infoDisplay formatada
  if (millis() - count > interval) {
    //if (sensor.measure()) { // Verifica se o sensor esta operacional para leitura dos valores
    temperature = 24;//sensor.getTemperature(); // Obtem o valor de temperatura
    humity = 80;//sensor.getHumidity(); // Obtem o valor da umidade relativa
    sprintf(strDisplay, "%.1fºC  -  %.0f%%", temperature, humity); // Formata a saida para ser mostrada no display
    //} else {
    //  sprintf(strDisplay, "Erro leitura");
    //}
    infoDisplay = strDisplay; // Atualiza o conteudo da informacao para String infoDisplay
    Serial.println(infoDisplay); // Imprime informacao formatada na serial
    // Mostra informacao atualizada da hora no display OLED
    display.clear();
    display.drawRect(0, 0, display.getWidth() - 1, display.getHeight() - 1);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 16, infoDisplay);
    display.setFont(ArialMT_Plain_10);
    display.drawString(display.getWidth() / 2, display.getHeight() / 2 + (TAMANHO == GEOMETRY_128_64 ? 5 : 0), "IP: " + WiFi.localIP().toString());
    display.display();

    count = millis();
  }
}

void changeDisplay() {
  if(estadoDisplay == HIGH){ //high = temperatura, LOW  relogio
    showTemperature();
  }else{
    showHours();
  }
}


