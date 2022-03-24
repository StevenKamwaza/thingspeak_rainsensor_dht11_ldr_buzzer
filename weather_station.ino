#include <ESP8266WiFi.h>
#include "DHT.h"
#include <ThingSpeak.h>

//declaring variables
const char *wifiName = "Che-Steve";
const char *password ="10345679";
//ldr sensor
const int LDR = A0;
//alarm variables
int buzzer = D1;
long startBuzzer, stopBuzzer;
bool flag = false;
unsigned long buzzerTime = 0;
#define rain D3
boolean isRaining;
DHT dht(D4, DHT11);
WiFiClient client;
long thingsChannelNum = 1635256;
const char thingWriteAPI[] = "95IR0A340G6P54M3";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(wifiName, password);

  //checking if the nodemcu is connected
  while(WiFi.status() != WL_CONNECTED){
    delay(200);
    Serial.println("Please Turn on WiFi");
  }
  Serial.println();
  Serial.println("Connected");
  Serial.println(WiFi.localIP());

  //ldr sensor
  
  dht.begin();
  pinMode(buzzer,OUTPUT);
  ThingSpeak.begin(client);
  
}

void runBuzzer(){
  //tone(buzzer,2000,500);
  digitalWrite (buzzer, HIGH); //turn buzzer on
//  delay(100);
//  digitalWrite (buzzer, LOW);  //turn buzzer off
//  delay(100);
 }

void loop() {
  // put your main code here, to run repeatedly:
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  //read digital value
  int rainValue = digitalRead(rain);
  //buzzer
  if(digitalRead(rain)==HIGH){
    if(!flag){
        flag = true;
        buzzerTime = millis();
    }
    if((millis() - buzzerTime) >= 5000){
      runBuzzer();
    }
  }else{
    flag =false;
  }
  
  if(rainValue==0){
    Serial.println(" It is raining : " + (String) rainValue);
  }
  else{
    Serial.println(" No rain : " + (String) rainValue);
  }
  //ldr sensor
  int lightValue = analogRead(A0);
  
  Serial.println(" Temperature: " + (String) temp);
  Serial.println(" Humidity: " + (String) hum);
  
  Serial.println(" LDR : " + (String) lightValue);

  //check if any reading is nan
  if(isnan(hum)|| isnan(temp) || isnan(rainValue)||isnan(lightValue)){
    Serial.println("Failed to read a sensor");
    return;
  }
  
  ThingSpeak.writeField(thingsChannelNum, 1, temp,thingWriteAPI);
  ThingSpeak.writeField(thingsChannelNum, 2, hum,thingWriteAPI);
  ThingSpeak.writeField(thingsChannelNum, 3, rainValue,thingWriteAPI);
  ThingSpeak.writeField(thingsChannelNum, 4, lightValue,thingWriteAPI);
  delay(2000);

}
