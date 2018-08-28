#include <ESP8266WiFi.h>

const char* ssid = "MyASUS";
const char* senha = "9a4281138522";
const String servidor = "192.168.43.49"; // seu servidor
String path;

int pinoBotao = 0;
int botao = 0;         // variable for reading the push button status

void setup () {
  pinMode(pinoBotao, INPUT);

  Serial.begin(115200);
  // Conexao to Wi-Fi
  Serial.print("Conectando ");
  Serial.println(ssid);
  WiFi.begin(ssid, senha);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Mostra IP do servidor
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Use este endereço para conectar ao ESP8266");
  Serial.println();
}

void loop() {
  WiFiClient client;
  //Le o valor da porta analogica 
  int leitura = digitalRead(pinoBotao);
  
  if (!client.connect(servidor, 80)) {
    Serial.println("Conexao falhou!");
    return;
  }

 path = leitura == LOW ? "btnCima" : "btnBaixo";
 
  // Envia a requisicao ao servidor com a acao desejada -> path
  client.print(String("GET /") + path + " HTTP/1.1\r\n" +
               "Host: " + servidor + "\r\n" +
               "Connection: close\r\n" +
               "\r\n");
  client.stop();
  delay(100); //Tempo para não sobrecarregar o servidor
}

