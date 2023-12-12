#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>


Servo monServo;

// Structure contenant les données à envoyer
struct SensorData {
  int flexvalue;
  int motorvalue;
};
// Variable pour stocker les données reçues
SensorData receivedData;

int flexSensorValue;
int flexSensorPin = A0;

int angle = 0; 
int minAngle = 0; 
int maxAngle = 180;

// Fonction de rappel qui sera appelée lorsque l'ESP32 reçevra les données.
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  
if (len == sizeof(SensorData)) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));

    Serial.print("Octets reçus : ");
    Serial.println(len);
  
    Serial.print("Angle Flex : ");
    Serial.println(receivedData.flexvalue);

    Serial.print("Angle Moteur : ");
    Serial.println(receivedData.motorvalue);

    monServo.write(receivedData.motorvalue); // Déplacez le servo à l'angle reçu
  } else {
    Serial.println("Longueur de données invalide");
  }
  Serial.println("  ");
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  
  monServo.attach(14); // Changer D1 par la broche connectée au SG90

}
 
void loop() {

}