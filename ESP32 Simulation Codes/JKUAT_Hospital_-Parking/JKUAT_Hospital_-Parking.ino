//librerias
#include <WiFi.h>
#include "PubSubClient.h"

//Parking Slot Details

int maxCapacity = 3;
int availableSlots = 0;
int slotCount;

//

//Pin Configuration

#define pinTrig1 19
#define pinEcho1 18

#define pinTrig2 5
#define pinEcho2 4

#define pinTrig3 12
#define pinEcho3 13

//conexiones
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "13.60.36.20";

WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi(){
  delay(10);
  Serial.print("");
  Serial.print("verbinden: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password,6);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wifi connected");
  Serial.print("Target IP: ");
  Serial.println(WiFi.localIP());
}

void setup(){
  Serial.begin(115200); //velocidad de comunicación
  setup_wifi();
  client.setServer(mqtt_server, 1883);

   //PinMode Configuration
  pinMode(pinTrig1, OUTPUT);
  pinMode(pinEcho1, INPUT);

  //Ultrasonic Sensor 2
  pinMode(pinTrig2, OUTPUT);
  pinMode(pinEcho2, INPUT);

   //Ultrasonic Sensor 3
  pinMode(pinTrig3, OUTPUT);
  pinMode(pinEcho3, INPUT);

}


int readSlot(int TrigPin, int EchoPin){
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  int duration = pulseIn(EchoPin, HIGH);
  int distance =(duration * 0.034 / 2);
  // Serial.print("Distance in CM: ");
  // Serial.println(distance);
  return distance;
}



void loop(){

  slotCount = 0;

  int slot1 = readSlot(pinTrig1, pinEcho1);
  int slot2 = readSlot(pinTrig2, pinEcho2);
  int slot3 = readSlot(pinTrig3, pinEcho3);

  if( slot1<100){
    slotCount++;
  }

   if( slot2<100){
    slotCount++;
  }

   if( slot3<100){
    slotCount++;
  }

  int availableSlots = maxCapacity - slotCount;


  char avSlot[5];
  dtostrf(availableSlots, 4, 0, avSlot);

  Serial.println(availableSlots);



  if(!client.connected()){
    reconnected();
  }

  // TempAndHumidity data = dhtSensor.getTempAndHumidity();
  // String temperatur = "Temp " + String(data.temperature, 2) + "°C";
  // String humidity = "Feuchte " + String(data.humidity, 2) + "%";
  // Serial.println(temperatur);
  // Serial.println(humidity);
  // delay(1000);

   client.loop();
   client.publish("slot/jhospital", avSlot);

  delay(1000);
}


void reconnected(){
  while(!client.connected()){
    Serial.print("Init....");

    if(client.connect("00cc40eb-9b47-446e-91e8-6f247ddc3b54")){
      Serial.println("Connected");
      client.subscribe("btg/temp");
      Serial.println("MQTT publish");
    } else{
      Serial.print("ERROR");
      Serial.print(client.state());
      Serial.print("next try in 4 seconds");
      delay(4000);
    }
  }
}
