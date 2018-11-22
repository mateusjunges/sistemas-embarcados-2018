#include <IRremoteESP8266.h>//  Biblioteca para acesso ao sensor IR - ESP8266
#include <IRsend.h>         // Biblioteca auxiliar - deve ser incluida junto com a IRremoteESP8266
#include <IRutils.h>        // Biblioteca auxiliar - deve ser incluida junto com a IRremoteESP8266
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

// Configuracoes iniciais
#define pinoBotao     0
#define pinoLED       BUILTIN_LED
#define pinoIR        2

// Controles basicos TV Samsung
#define POWER             0xE0E040BF // Ligar/Desligar 
#define VOLUME_UP_PIN     16
#define VOLUME_DOWN_PIN   05
#define CHANNEL_UP_PIN    04
#define CHANNEL_DOWN_PIN  14

#define SAMSUNG_BITS  32 // Tamanho do codigo de informacao para o dispositivo Sansung

//Códigos dos botões:
char* VOLUME_UP = "0xE0E0E01F";
char* VOLUME_DOWN = "0xE0E0D02F";
char* CHANNEL_UP = "0xE0E048B7";
char* CHANNEL_DOWN = "0xE0E008F7";

typedef struct // Cria uma STRUCT para armazenar os dados dos botoes
{
  int pino;
  int statusBotao;
  int statusBotaoAnterior = HIGH;
  unsigned long debounceAnterior = 0;
} Botao;

unsigned long tempoAnteriorDebounce = 0;
unsigned long debounceDelay = 50;   // tempo do debounce time; aumentar se saida oscila

/**
  Configuração wifi
**/
const char* ssid = "JUNGES";
const char* password = "mateus1234560";

/**Configuração MQTT*/
const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* topico = "Sistemas.Embarcados.Topico.SANSUNG";

char* payload = "";

WiFiClient clienteWIFI;
PubSubClient clienteMQTT(clienteWIFI);

String stringMacAddress;
char macAddress[6];

void conectaMQTT() {
  while (!clienteMQTT.connected()) {
    Serial.print("Aguardando conexao MQTT");
    if (clienteMQTT.connect(macAddress)) {
      Serial.println("MQTT conectado");
      clienteMQTT.subscribe(topico);
    } else {
      Serial.print("Falha, rc=");
      Serial.print(clienteMQTT.state());
      Serial.println(" tentando reconectar em 5 segundos!");
      delay(5000);
    }
  }
}

/* Botões para controlar a TV */
Botao volume_up;
Botao volume_down;
Botao channel_up;
Botao channel_down;

int statusLED = HIGH;

void setup()
{
  Serial.begin(115200);
  //Conexão WiFi:
  Serial.println("Conectando ao wifi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Mostra IP do servidor
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.RSSI());
  Serial.println("Use este endereço para conectar ao ESP8266");
  Serial.println();
  stringMacAddress = WiFi.macAddress();
  stringMacAddress.toCharArray(macAddress, 6);
  clienteMQTT.setServer(mqtt_server, 1883);
  pinMode(VOLUME_UP_PIN, INPUT);
  pinMode(VOLUME_DOWN_PIN, INPUT);
  pinMode(CHANNEL_UP_PIN, INPUT);
  pinMode(CHANNEL_DOWN_PIN, INPUT);
  volume_up.pino = VOLUME_UP_PIN;
  volume_down.pino = VOLUME_DOWN_PIN;
  channel_up.pino = CHANNEL_UP_PIN;
  channel_down.pino = CHANNEL_DOWN_PIN;
}

void loop() {
  if (!clienteMQTT.connected()) {
    conectaMQTT();
  }
  clienteMQTT.loop();
  volume_up = debounce(volume_up);
  volume_down = debounce(volume_down);
  channel_up = debounce(channel_up);
  channel_down = debounce(channel_down);
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
        statusLED = !statusLED;
        switch (b.pino) {
          case VOLUME_UP_PIN:
            payload = VOLUME_UP;
            clienteMQTT.publish(topico, payload);
            break;
          case VOLUME_DOWN_PIN:
            payload = VOLUME_DOWN;
            clienteMQTT.publish(topico, payload);
            break;
          case CHANNEL_UP_PIN:
            payload = CHANNEL_UP;
            clienteMQTT.publish(topico, payload);
            break;
          case CHANNEL_DOWN_PIN:
            payload = CHANNEL_DOWN;
            clienteMQTT.publish(topico, payload);
            break;
          default:
            Serial.println("Botão não encontrado");
            break;
        }
      }
    }
  }
  b.statusBotaoAnterior = leitura;
  return b;
}
