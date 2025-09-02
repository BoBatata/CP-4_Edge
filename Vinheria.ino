// Nicoals Tetsuo Kimura - RM 565377
// Pedro Santos - RM 564188
// Pedro Cunha Coutinho - RM 564188
// Breno Henrique Bortoloti Santos - RM 564188
// Thomaz Vasconcelos Mendes - RM 564188

#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
 
#define DHTPIN 15 // Pino GPIO15 do ESP32 para o DHT22
#define DHTTYPE DHT11 // Tipo de sensor DHT (DHT22)
DHT dht(DHTPIN, DHTTYPE);
 
#define POT_PIN 34 // Pino GPIO34 do ESP32 para o Potenciômetro
 
// Informações para conexão no wifi ou via servidor Thing Speak
const char* ssid = "Wokwi-GUEST"; // "FIAP-IOT" Rede Wi-Fi
const char* password = ""; // "F!@p25.IOT" Senha da rede Wi-Fi
const char* apiKey = "837NQQH521XTO8IM"; // Write API Key
const char* server = "http://api.thingspeak.com"; // Servidor ThingSpeak
 
void setup() {
  Serial.begin(115200);
  dht.begin();
 
  // Configuração dos pinos
  pinMode(POT_PIN, INPUT);
 
  // Verificação de Wifi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" conectado!");
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Pegando os valores dos sensores
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int potValue = analogRead(POT_PIN); // Leitura do Potenciômetro
 
    if (isnan(h) || isnan(t)) {
      Serial.println("Falha ao ler o sensor DHT22!");
      return;
    }

    // Envio de dados para o ThingSpeak
    HTTPClient http;
    String url = String(server) + "/update?api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h) + "&field3=" + String(potValue);
    http.begin(url);
 
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString(); // Resposta da requisição HTTP
      Serial.println("Dados enviados ao ThingSpeak.");
      Serial.print("Código HTTP: ");
      Serial.println(httpCode);
      Serial.println("Resposta: ");
      Serial.println(payload);
    } else {
      Serial.print("Erro ao enviar dados. Código HTTP: ");
      Serial.println(httpCode);
    }
   
    http.end();
  } else {
    Serial.println("WiFi não conectado. Tentando reconectar...");
  }
 
  // Espera 15 segundos para enviar a requisição novamente
  delay(2500);
}