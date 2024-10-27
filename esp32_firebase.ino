#include "secrets.h"  
#include <Firebase.h> 

#define sensor 33
#define relay 4

Firebase fb(REFERENCE_URL); 
int previousMoisture = -1;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing System...");
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("Wi-Fi Connected!");
  
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH); 
  Serial.println("System Initialization Complete. System Ready.");
  previousMoisture = fb.getInt("preML");
}

void soilMoisture() {
  int value = analogRead(sensor);
  value = map(value, 0, 4095, 0, 100); 
  value = (value - 100) * -1; 
  String m_str = String(value) + "%";
  
  if (abs(value - previousMoisture) >= 5) { 
    fb.setString("m_level", m_str);   
    fb.setInt("preML", value);
    previousMoisture = value;
      }
}

void controlMotor() {
  int motorState = fb.getInt("motor");
  int moistureValue = previousMoisture; // Get the last known moisture value

  // Automatic control logic
  if (moistureValue < 20) {
    digitalWrite(relay, LOW); 
    
  } else if (moistureValue > 50 || motorState == 0  ) {
    digitalWrite(relay, HIGH); 
 
  }
   if  (motorState == 1 && moistureValue <= 90) {
    digitalWrite(relay, LOW); 
    
  } 
}


void loop() {
  Serial.println("---");
  soilMoisture();
  controlMotor();
  delay(200); 
}
