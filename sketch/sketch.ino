//#include "Firebase_Arduino_WiFiNINA.h"
#include <ArduinoBLE.h>
#include "secrets.h"
#include "meeting_anim.h"
#include "colors_anim.h"
#include "chase_anim.h"
#include "party_anim.h"
#include <FastLED.h>
#include "led_config.h"

#ifdef Firebase_Arduino_WiFiNINA_H
FirebaseData firebaseData;
const String path = "/test/int";
unsigned long nextFirebaseCheck;
#endif

#ifdef _ARDUINO_BLE_H_
#define BLENAME "BusyLight"
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service
// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
//BLEDevice central;
//unsigned long nextBLECheck;
#endif

int currAnimIdx = 0;
CRGB leds[NUM_LEDS];
Anim * currentAnim;
MeetingAnim meetingAnim{}; // 1
ColorsAnim colorsAnim{}; // 0
ChaseAnim chaseAnim{}; // 2
PartyAnim partyAnim{}; // 3
int *frame = new int[NUM_LEDS];
unsigned long nextFrameTime;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  setupSerial();
  randomSeed(analogRead(0));
  
  #ifdef Firebase_Arduino_WiFiNINA_H
  Serial.println("Connect to WiFi");
  connectWiFi();
  Firebase.begin(FIREBASE_HOST, SECRET_FIREBASE_AUTH, SECRET_SSID, SECRET_PASSWD);
  Firebase.reconnectWiFi(true);
  nextFirebaseCheck = millis();
  #endif

  #ifdef _ARDUINO_BLE_H_
  initBLE();
//  nextBLECheck = millis();
  #endif
  
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  currentAnim = &colorsAnim;
  nextFrameTime = millis();
  digitalWrite(LED_BUILTIN, LOW);
}

void setupSerial() {
  Serial.begin(9600);
  if (!Serial) {
    // Wait 3 sec for serial connection
    delay(10000);
  }
}

#ifdef _ARDUINO_BLE_H_
void initBLE() {
  if(!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    return;
  }
  BLE.setLocalName(BLENAME);
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(switchCharacteristic);
  BLE.addService(ledService);
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);
  switchCharacteristic.writeValue(0);
  BLE.advertise();
  Serial.println("BLE Peripheral Initalized");
  Serial.print("Name: ");
  Serial.println(BLENAME);
  Serial.print("Local Address: ");
  Serial.println(BLE.address());
}

void blePeripheralConnectHandler(BLEDevice central) {
  Serial.println("Connection Event!");
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  Serial.println("Disconnect Event!");
}

void switchCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  Serial.println("Switch Event!");
  int val = switchCharacteristic.value();
  Serial.print("Got val: ");
  Serial.println(val);
  if(val != currAnimIdx) {
    switchAnim(val);
  }
}
#endif

#ifdef Firebase_Arduino_WiFiNINA_H
void connectWiFi() {
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    status = WiFi.begin(SECRET_SSID, SECRET_PASSWD);
    Serial.print(status);
    switch(status) {
      case WL_CONNECTED:
        Serial.println(" Connected");
        break;
      case WL_AP_CONNECTED:
        Serial.println(" AP Connected");
        break;
      case WL_AP_LISTENING:
        Serial.println(" AP Listening");
        break;
      case WL_NO_MODULE:
        Serial.println(" No Module");
        break;
      case WL_IDLE_STATUS:
        Serial.println(" Idle Status");
        break;
      case WL_NO_SSID_AVAIL:
        Serial.println(" No SSID Available");
        break;
      case WL_SCAN_COMPLETED:
        Serial.println(" Scan Completed");
        break;
      case WL_CONNECT_FAILED:
        Serial.println(" Connection Failed");
        break;
      case WL_CONNECTION_LOST:
        Serial.println(" Connection Lost");
        break;
      case WL_DISCONNECTED:
        Serial.println(" Disconnected");
        break;
    }
    delay(300);
  }
  Serial.println("");
  Serial.print("Connected with IP: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  Serial.println("");
}
#endif

void loop() {
  unsigned long now = millis();

  #ifdef _ARDUINO_BLE_H_
  BLE.poll();
  #endif
  
  // this is probably going to have some drift, but that's ok for now
  if(now >= nextFrameTime) { 
    // render nextFrame
    frame = currentAnim->nextFrame(frame);
    copyFrameToLeds(frame);
    FastLED.show();
    // clac next frame time
    nextFrameTime = now + long(1000.0 / currentAnim->fps);
  }
  
  #ifdef Firebase_Arduino_WiFiNINA_H
  // check Firebase if we should change anim;
  if(now >= nextFirebaseCheck) {
    Serial.println("check firebase");
    int val = getFirebaseVal();
    if(val != currAnimIdx) {
      switchAnim(val);
    }
    nextFirebaseCheck = now + 1000;
  }
  #endif
  
  #ifdef _ARDUINO_BLE_H_
  BLE.poll();
  #endif
}

void switchAnim(int idx) {
  switch(idx) {
    case 0:
      currentAnim = &colorsAnim;
      currAnimIdx = idx;
      break;
    case 1:
      currentAnim = &meetingAnim;
      currAnimIdx = idx;
      break;
    case 2:
      currentAnim = &chaseAnim;
      currAnimIdx = idx;
      break;
    case 3:
      currentAnim = &partyAnim;
      currAnimIdx = idx;
      break;
  }

  #ifdef _ARDUINO_BLE_H_
  switchCharacteristic.writeValue(currAnimIdx);
  #endif
}

void copyFrameToLeds(int frame[]) {
  for(int i = 0; i < NUM_LEDS; i++) {
    int color = frame[i];
    int red = color >> 16 & 255;
    int green = color >> 8 & 255;
    int blue = color & 255;
    leds[i] = CRGB(red, green, blue);
    leds[i].fadeLightBy(128);
  }
}

#ifdef Firebase_Arduino_WiFiNINA_H
int getFirebaseVal() {
  int val = -1;
  if (Firebase.getInt(firebaseData, path)) {
    if (firebaseData.dataType() == "int") {
      val = firebaseData.intData();
    }
  } else {
    Serial.println(firebaseData.errorReason());
  }
  return val;
}
#endif
