#include <ArduinoJson.h>

#include <FirebaseCloudMessaging.h>
#include <Firebase.h>
#include <FirebaseHttpClient.h>
#include <FirebaseArduino.h>
#include <FirebaseError.h>
#include <FirebaseObject.h>


#include <ESP8266WiFi.h>


#define FIREBASE_HOST "prudent-n-safe.firebaseio.com"
#define FIREBASE_AUTH "XC6XGUFzaP5HQpmXGybPZuLe5YoO61sPTN4wWQLi"
#define WIFI_SSID "KPR-MIX"
#define WIFI_PASSWORD "kpr48389"
//chr FIREBASE+PATH = "hbxqGAj6zKT9Om3Y8ZXuwNYgu7t2/witch1/"

int senserMoving = D3;
int senserFire = D4;
int statusMove = 0;
int statusFire = 0;
void setup() {
  Initialize();
  WiFiConnect();
  

  // Do something
}

void loop() {
  // put your main code here, to run repeatedly:
  readSenser();
  delay(500);
  
}

void readSenser(){
  statusMove = 0;
  statusFire = 0;
  statusMove = analogRead(senserMoving);
  statusFire = analogRead(senserFire);
  Serial.print("Moving : ");
  Serial.println(analogRead(senserMoving));
  Serial.print("Fire : ");
  Serial.println(analogRead(senserFire));

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  if(statusMove >= 4){
    Firebase.setBool("hbxqGAj6zKT9Om3Y8ZXuwNYgu7t2/switch1/", true);
  }else{
    Firebase.setBool("hbxqGAj6zKT9Om3Y8ZXuwNYgu7t2/switch1/", false);
  }
  if(statusFire >= 1200){
    Firebase.setBool("hbxqGAj6zKT9Om3Y8ZXuwNYgu7t2/switch2/", true);
  }else{
    Firebase.setBool("hbxqGAj6zKT9Om3Y8ZXuwNYgu7t2/switch2/", false);
  }
  
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  
}

void Initialize(){
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    
}

void WiFiConnect(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
//
  

}
