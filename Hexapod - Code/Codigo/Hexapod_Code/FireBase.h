#include <WiFi.h>

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

float LbAlpha;
float LbBeta;
float LbGamma;

float LfAlpha;
float LfBeta;
float LfGamma;

float LmAlpha;
float LmBeta;
float LmGamma;

float RbAlpha;
float RbBeta;
float RbGamma;

float RfAlpha;
float RfBeta;
float RfGamma;

float RmAlpha;
float RmBeta;
float RmGamma;

//OFFSET//

float Offset_LbAlpha = 90;
float Offset_LbBeta = 90;
float Offset_LbGamma = 90;

float Offset_LfAlpha = 90;
float Offset_LfBeta = 90;
float Offset_LfGamma = 90;

float Offset_LmAlpha = 90;
float Offset_LmBeta = 90;
float Offset_LmGamma = 90;

float Offset_RbAlpha = 90;
float Offset_RbBeta = 90;
float Offset_RbGamma = 90;

float Offset_RfAlpha = 90;
float Offset_RfBeta = 90;
float Offset_RfGamma = 90;

float Offset_RmAlpha = 90;
float Offset_RmBeta = 90;
float Offset_RmGamma = 90;

///

int MicroSeg_LbAlpha;
int MicroSeg_LbBeta;
int MicroSeg_LbGamma;

int MicroSeg_LfAlpha;
int MicroSeg_LfBeta;
int MicroSeg_LfGamma;

int MicroSeg_LmAlpha;
int MicroSeg_LmBeta;
int MicroSeg_LmGamma;

int MicroSeg_RbAlpha;
int MicroSeg_RbBeta;
int MicroSeg_RbGamma;

int MicroSeg_RfAlpha;
int MicroSeg_RfBeta;
int MicroSeg_RfGamma;

int MicroSeg_RmAlpha;
int MicroSeg_RmBeta;
int MicroSeg_RmGamma;

//

bool signupOK = false;

void FireBase_Setup() {

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

void FireBase_Loop() {
  if (signupOK) {
    
    if (Firebase.get(firebaseData, "/lb/alpha")) {
      LbAlpha = firebaseData.floatData();
    } else {
      ESP.restart();
    }

    if (Firebase.get(firebaseData, "/lb/beta")) {
      LbBeta = firebaseData.floatData();
    }

    if (Firebase.get(firebaseData, "/lb/gamma")) {
      LbGamma = firebaseData.floatData();
    }

//////

    if (Firebase.get(firebaseData, "/lf/alpha")) {
      LfAlpha = firebaseData.floatData();
    }

    if (Firebase.get(firebaseData, "/lf/beta")) {
      LfBeta = firebaseData.floatData();
    }

    if (Firebase.get(firebaseData, "/lf/gamma")) {
      LfGamma = firebaseData.floatData();
    }

///////

    if (Firebase.get(firebaseData, "/lm/alpha")) {
      LmAlpha = firebaseData.floatData();
    }

    if (Firebase.get(firebaseData, "/lm/beta")) {
      LmBeta = firebaseData.floatData();
    }

    if (Firebase.get(firebaseData, "/lm/gamma")) {
      LmGamma = firebaseData.floatData();
    }

/////// asdsad

    if (Firebase.get(firebaseData, "/rb/alpha")) {
      RbAlpha = firebaseData.floatData();
    } 

    if (Firebase.get(firebaseData, "/rb/beta")) {
      RbBeta = firebaseData.floatData();
    }

    if (Firebase.get(firebaseData, "/rb/gamma")) {
      RbGamma = firebaseData.floatData();
    }

//////

    if (Firebase.get(firebaseData, "/rf/alpha")) {
      RfAlpha = firebaseData.floatData();
    }

    if (Firebase.get(firebaseData, "/rf/beta")) {
      RfBeta = firebaseData.floatData();
    }

    if (Firebase.get(firebaseData, "/rf/gamma")) {
      RfGamma = firebaseData.floatData();
    }

///////

    if (Firebase.get(firebaseData, "/rm/alpha")) {
      RmAlpha = firebaseData.floatData();
    }

    if (Firebase.get(firebaseData, "/rm/beta")) {
      RmBeta = firebaseData.floatData();
    }

    if (Firebase.get(firebaseData, "/rm/gamma")) {
      RmGamma = firebaseData.floatData();
    }

    // Agrega cualquier otra lógica aquí según sea necesario

    // Puedes ajustar el tiempo de espera entre solicitudes a la base de datos según tus necesidades
    delay(0);
  } else {
    Serial.println("Registro no exitoso. Verifica las credenciales y la conexión a Internet.");
  }
}

void FireBase_Print(){
  Serial.println("/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/");
  Serial.println("LbAlpha: " + String(LbAlpha));
  Serial.println("LbBeta: " + String(LbBeta));
  Serial.println("LbGamma: " + String(LbGamma));
  Serial.println("LfAlpha: " + String(LfAlpha));
  Serial.println("LfBeta: " + String(LfBeta));
  Serial.println("LfGamma: " + String(LfGamma));
  Serial.println("LmAlpha: " + String(LmAlpha));
  Serial.println("LmBeta: " + String(LmBeta));
  Serial.println("LmGamma: " + String(LmGamma));
  Serial.println("RbAlpha: " + String(RbAlpha));
  Serial.println("RbBeta: " + String(RbBeta));
  Serial.println("RbGamma: " + String(RbGamma));
  Serial.println("RfAlpha: " + String(RfAlpha));
  Serial.println("RfBeta: " + String(RfBeta));
  Serial.println("RfGamma: " + String(RfGamma));
  Serial.println("RmAlpha: " + String(RmAlpha));
  Serial.println("RmBeta: " + String(RmBeta));
  Serial.println("RmGamma: " + String(RmGamma));
}

int FireangleToMicroseconds(double angle) {
    double val = 500.0 + (((2500.0 - 500.0) / 180.0) * angle);
    return (int)val;
  }

void FireBase_Final_Angle() {
  
  MicroSeg_LbAlpha = FireangleToMicroseconds(LbAlpha+Offset_LbAlpha);
  MicroSeg_LbBeta = FireangleToMicroseconds(LbBeta+Offset_LbBeta);
  MicroSeg_LbGamma = FireangleToMicroseconds(LbGamma+Offset_LbGamma);

  MicroSeg_LfAlpha = FireangleToMicroseconds(LfAlpha+Offset_LfAlpha);
  MicroSeg_LfBeta = FireangleToMicroseconds(LfBeta+Offset_LfBeta);
  MicroSeg_LfGamma = FireangleToMicroseconds(LfGamma+Offset_LfGamma);

  MicroSeg_LmAlpha = FireangleToMicroseconds(LmAlpha+Offset_LmAlpha);
  MicroSeg_LmBeta = FireangleToMicroseconds(LmBeta+Offset_LmBeta);
  MicroSeg_LmGamma = FireangleToMicroseconds(LmGamma+Offset_LmGamma);

  MicroSeg_RbAlpha = FireangleToMicroseconds(RbAlpha+Offset_RbAlpha);
  MicroSeg_RbBeta = FireangleToMicroseconds(RbBeta+Offset_RbBeta);
  MicroSeg_RbGamma = FireangleToMicroseconds(RbGamma+Offset_RbGamma);

  MicroSeg_RfAlpha = FireangleToMicroseconds(RfAlpha+Offset_RfAlpha);
  MicroSeg_RfBeta = FireangleToMicroseconds(RfBeta+Offset_RfBeta);
  MicroSeg_RfGamma = FireangleToMicroseconds(RfGamma+Offset_RfGamma);

  MicroSeg_RmAlpha = FireangleToMicroseconds(RmAlpha+Offset_RmAlpha);
  MicroSeg_RmBeta = FireangleToMicroseconds(RmBeta+Offset_RmBeta);
  MicroSeg_RmGamma = FireangleToMicroseconds(RmGamma+Offset_RmGamma);

}

