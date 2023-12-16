#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Wira"
#define WIFI_PASSWORD "Wira1234"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDJ0XCNB9pP8zgwS8EfgKZJgVXT4_Qm44o"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://scorpybot-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData firebaseData;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int intValue;
float floatValue;

float RmAlpha;
float RmBeta;
float RmGamma;

bool signupOK = false;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {

    if (Firebase.get(firebaseData, "/rm/gamma")) {
      RmGamma = firebaseData.floatData();
      Serial.println(RmGamma);
    } else {
      Serial.println(firebaseData.errorReason());
    }

    if (Firebase.get(firebaseData, "/rm/alpha")) {
      RmAlpha = firebaseData.floatData();
      Serial.println(RmAlpha);
    } else {
      Serial.println(firebaseData.errorReason());
    }
  delay(100);
  }
  
