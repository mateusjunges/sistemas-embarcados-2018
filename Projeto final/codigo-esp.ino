//Bibliotecas:
#include <Ultrasonic.h> //Sensor ultrasônico
#include <ESP8266WiFi.h> //Wifi
#include <PubSubClient.h> //MQTT

//Definições de ligação da ponte H:
#define enable_A 4  //d2
#define enable_B 14 //d5
#define input_1 0   //d3
#define input_2 2   //d4
#define input_3 12  //d6
#define input_4 13  //d7

//Definição de ligação do sensor ultrasônico:
#define TRIGGER 16 //d0
#define ECHO 5     //d1

#define backDelay 0

//Declaração do sensor ultrasônico:
Ultrasonic ultrasonic(TRIGGER, ECHO);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

//Variáveis:
float distancia = 0; //Armazena a distância em relação ao sensor com o objeto mais próximo
float distancia_segura = 50; //Maior distância que o carrinho pode chegar de um objeto
float correcaoA = 0.5;
float correcaoB = 0.5;

bool isON = false;

const char* ssid = "JUNGES";
const char* password = "mateus1234560";
const char* server = "broker.mqtt-dashboard.com";
const char* topic = "mateusjunges.alceubritto.projeto.sistemas.embarcados";

String strMacAdress;
char macAddress[6];

void goBack(){
  digitalWrite(input_1, HIGH);
  digitalWrite(input_2, LOW);
  digitalWrite(input_3, LOW);
  digitalWrite(input_4, HIGH);
  delay(backDelay);
}
void goAhead(){
  digitalWrite(input_1, LOW); 
  digitalWrite(input_2, HIGH);
  digitalWrite(input_3, HIGH);
  digitalWrite(input_4, LOW); 
}
void turnRight(){
  digitalWrite(input_1, LOW); 
  digitalWrite(input_2, HIGH);
  digitalWrite(input_3, LOW);
  digitalWrite(input_4, HIGH);
}
void turnLeft(){
  digitalWrite(input_1, HIGH);
  digitalWrite(input_2, LOW);
  digitalWrite(input_3, HIGH);
  digitalWrite(input_4, LOW); 
}

void changeVelocity(int en_A, int en_B){
  analogWrite(enable_A, en_A);
  analogWrite(enable_B, en_B);
}

void _on(){
 isON = true;   
}

void _off(){
  analogWrite(enable_A, 0);
  analogWrite(enable_B, 0);
  isON = false;  
}

void callback(char* topico, byte* payload, unsigned int tam){
  Serial.println("[MENSAGEM RECEBIDA]");
  Serial.print("Topico: ");
  Serial.println(topico);
  Serial.print("Mensagem: ");
  for(int i = 0; i < tam; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if((char)payload[0] == 'L')
    _on();
  else if((char)payload[0] == 'D')
    _off();
  }


void connectMQTT(){
  while(!mqttClient.connected()){
    Serial.println("Aguardando conexão MQTT...");
    if(mqttClient.connect(macAddress)){
      Serial.println();
      Serial.println("MQTT Conectado");
      mqttClient.subscribe(topic);
    }else{
      Serial.print("Falha, rc = ");
      Serial.println(mqttClient.state());
      Serial.println("Tentando reconectar em 2 segundos...");
      delay(2000);
    }
  }  
}


void setup() {
  Serial.begin(115200);
  
  pinMode(input_1, OUTPUT);
  pinMode(input_2, OUTPUT);
  pinMode(input_3, OUTPUT);
  pinMode(input_4, OUTPUT);
  pinMode(enable_A, OUTPUT);
  pinMode(enable_B, OUTPUT);

  Serial.print("Conectando a rede: ");
  Serial.println(ssid);

  while(WiFi.status() != WL_CONNECTED){
    delay(200);
    Serial.print(".");  
  }
  Serial.println("WiFi Conectado");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.RSSI());
  Serial.println("Use este endereço para conectar ao ESP");
  mqttClient.setServer(server, 1883);
  mqttClient.setCallback(callback);
}



void loop() {
  if(!mqttClient.connected())
    connectMQTT();
  mqttClient.loop();
  
  if(isON){
    distancia = ultrasonic.convert(ultrasonic.timing(), Ultrasonic::CM);
  Serial.print("Distância: ");
  Serial.println(distancia);
  if(distancia <= distancia_segura){
    changeVelocity(600*correcaoA, 600*correcaoB);
    delay(50);
    changeVelocity(500*correcaoA, 500*correcaoB);
    delay(50);
    changeVelocity(450*correcaoA, 450*correcaoB);
    delay(50);
    goBack();
    turnRight();    
  }else{
    goAhead();
    changeVelocity(500*correcaoA, 500*correcaoB);
    delay(50);
    changeVelocity(600*correcaoA, 600*correcaoB);
    delay(50);
    changeVelocity(700*correcaoA, 700*correcaoB);
    delay(50);
    changeVelocity(850*correcaoA, 850*correcaoB);
    delay(50);
    changeVelocity(1000*correcaoA, 1000*correcaoB);
  }  
  }
}
