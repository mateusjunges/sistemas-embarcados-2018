#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <ESP8266WiFi.h>

#define PIN_S0  12
#define PIN_S1  13
#define PIN_S2  14

// WiFi network info.
const char* ssid = "JUNGES";
const char* senha = "mateus1234560";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "ea9b35d0-91a2-11e8-b90f-f7e25f9cf9c8";
char password[] = "23631a45a16a73bddce0f3d1dc0f480a97142ba3";
char clientID[] = "f4a19600-91a2-11e8-b389-d93e10e08451";

//Controle dos canais do multiplexador
int pins[3] = {PIN_S0, PIN_S1, PIN_S2};
void muxWrite(int canal) {
  for (int i = 0; i < 3; i++)  {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], bitRead(canal, i));
  }
}

void setup() {
  // Conexao to Wi-Fi
  Serial.begin(115200);
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

  Cayenne.begin(username, password, clientID);//, ssid, wifiPassword);
}

void loop() {
  Cayenne.loop();
}

CAYENNE_IN(20) {
  Serial.println("Mux canal 0");
  muxWrite(0);
}

CAYENNE_IN(21) {
  Serial.println("Mux canal 1");
  muxWrite(1);
}

CAYENNE_IN(22) {
  Serial.println("Mux canal 2");
  muxWrite(2);
}

CAYENNE_IN(23) {
  Serial.println("Mux canal 3");
  muxWrite(3);
}

CAYENNE_IN(24) {
  Serial.println("Mux canal 4");
  muxWrite(4);
}

CAYENNE_IN(25) {
  Serial.println("Mux canal 5");
  muxWrite(5);
}

CAYENNE_IN(26) {
  Serial.println("Mux canal 6");
  muxWrite(6);
}

CAYENNE_IN(27) {
  Serial.println("Mux canal 7");
  muxWrite(7);
}
