#include <Arduino.h>

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <FirebaseESP32.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Inserta tus credenciales de red
#define WIFI_SSID "Wira"
#define WIFI_PASSWORD "Wira1234"

// Inserta la clave API de tu proyecto Firebase
#define API_KEY "AIzaSyDJ0XCNB9pP8zgwS8EfgKZJgVXT4_Qm44o"

// Inserta la URL de tu base de datos en tiempo real (RTDB)
#define DATABASE_URL "https://scorpybot-default-rtdb.firebaseio.com/"

// Define el objeto de datos de Firebase
FirebaseData firebaseData;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
float RmAlpha;
float RmBeta;
float RmGamma;

bool signupOK = false;

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando a Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Conectado con IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Asigna la clave API (obligatorio)
  config.api_key = API_KEY;

  // Asigna la URL de la RTDB (obligatorio)
  config.database_url = DATABASE_URL;

  // Regístrate (sign up)
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Registro exitoso");
    signupOK = true;
  } else {
    Serial.printf("Error en el registro: %s\n", config.signer.signupError.message.c_str());
  }

  // Asigna la función de devolución de llamada para la tarea de generación de token
  config.token_status_callback = tokenStatusCallback; // Ver addons/TokenHelper.h

  // Inicializa Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  firebaseData.setBSSLBufferSize(4096, 1024); // Tamaños de búfer RX y TX

}

void loop() {
  if (signupOK) {
    if (Firebase.get(firebaseData, "/rm/gamma")) {
      RmGamma = firebaseData.floatData();
      Serial.println("RmGamma: " + String(RmGamma));
    } else {
      Serial.println("Error al obtener RmGamma: " + firebaseData.errorReason());
    }

    if (Firebase.get(firebaseData, "/rm/alpha")) {
      RmAlpha = firebaseData.floatData();
      Serial.println("RmAlpha: " + String(RmAlpha));
    } else {
      Serial.println("Error al obtener RmAlpha: " + firebaseData.errorReason());
    }

    // Agrega cualquier otra lógica aquí según sea necesario

    // Puedes ajustar el tiempo de espera entre solicitudes a la base de datos según tus necesidades
    delay(0);
  } else {
    Serial.println("Registro no exitoso. Verifica las credenciales y la conexión a Internet.");
  }
}
