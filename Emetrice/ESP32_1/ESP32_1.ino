#include <esp_now.h>
#include <WiFi.h>

// Adresse mac de l'ESP Réceptrice
uint8_t broadcastAddress[] = {0x40, 0x22, 0xD8, 0x79, 0xEF, 0x10}; //40:22:D8:79:EF:10

// Structure contenant les données à envoyer
struct SensorData {
  int flexvalue;
  int motorvalue;
};

// Initiation des pins
const int flexSensorPin = A0;
 int flexSensorValue;

// Stockage des informations de l'ESP Réceptrice
esp_now_peer_info_t peerInfo;

// Fonction de rappel qui est exécutée lors de l'envoi du message pour voit si le message à été envoyé 
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  Serial.begin(115200);
   WiFi.mode(WIFI_STA);

// Initialisation de l'ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

// Enregistrement de la fonction de rappel  
  esp_now_register_send_cb(OnDataSent);
  
// Enregistrement de l'ESP Réceptrice 
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Ajout de pairs        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

int mapFlex(int ValueFlex){

  int angle;

  if (ValueFlex > 1000)
  {
    angle = map(ValueFlex, 1000, 4095, 150, 180);
  }
  else if (ValueFlex > 600)
  {
    angle = map(ValueFlex, 600, 1000, 60, 120);
  }
  else if (ValueFlex > 0)
  {
    angle = map(ValueFlex, 0, 600, 0, 60);
  } 

  return angle;
}

void loop() {
  flexSensorValue = analogRead(flexSensorPin);

  Serial.println("Valeur du capteur de flexion : ");
  Serial.println(flexSensorValue);

  SensorData dataToSend;
  dataToSend.flexvalue = flexSensorValue;
  dataToSend.motorvalue = mapFlex(flexSensorValue);
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &dataToSend, sizeof(dataToSend));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(200);
}