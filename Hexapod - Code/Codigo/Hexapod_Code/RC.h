#include <SPI.h> // Incluye la biblioteca SPI para la comunicación serial
#include <nRF24L01.h> // Incluye la biblioteca para el módulo de radio nRF24L01
#include <RF24.h> // Incluye la biblioteca RF24 para manejar el módulo de radio

RF24 radio(4, 5); // Crea una instancia de la clase RF24, los parámetros son los pines CE y CSN

const byte address[6] = "00011"; // Dirección del canal de comunicación

unsigned long rc_last_received_time = 0; // Almacena el último momento en que se recibieron datos
unsigned long rc_current_time = 0; // Almacena el momento actual

// Estructura para almacenar los datos del control remoto
struct RC_Data_Package {

  byte joy1_X; // Posición X del joystick 1
  byte joy1_Y; // Posición Y del joystick 1
  byte joy1_Button; // Estado del botón del joystick 1

  byte joy2_X; // Posición X del joystick 2
  byte joy2_Y; // Posición Y del joystick 2
  byte joy2_Button; // Estado del botón del joystick 2

  byte joy3_X; // Posición X del joystick 3
  byte joy3_Y; // Posición Y del joystick 3
  byte joy3_Button; // Estado del botón del joystick 3

  byte joy4_X; // Posición X del joystick 4
  byte joy4_Y; // Posición Y del joystick 4
  byte joy4_Button; // Estado del botón del joystick 4

  byte joy5_X; // Posición X del joystick 5
  byte joy5_Y; // Posición Y del joystick 5
  byte joy5_Button; // Estado del botón del joystick 5

  byte slider1; // Posición del slider 1
  byte slider2; // Posición del slider 2

  byte pushButton1; // Estado del botón pulsador 1
  byte pushButton2; // Estado del botón pulsador 2
};

RC_Data_Package rc_data; // Crea una instancia de la estructura RC_Data_Package para almacenar los datos actuales
RC_Data_Package rc_data_previous; // Crea una instancia de la estructura RC_Data_Package para almacenar los datos previos

void RC_Setup();
bool RC_GetDataPackage();
void RC_DisplayData();
void RC_ResetData();

void RC_Setup(){
    radio.begin(); // Inicia el módulo de radio
    radio.openReadingPipe(0, address); // Abre un canal de lectura con la dirección especificada
    radio.setPALevel(RF24_PA_MIN); // Establece el nivel de potencia de salida al mínimo
    radio.startListening(); // Pone el módulo de radio en modo recepción (en lugar de transmisión)
}

bool RC_GetDataPackage(){
    if (radio.available()) { 
        rc_data_previous = rc_data; 
        radio.read(&rc_data, sizeof(RC_Data_Package)); 
        rc_last_received_time = millis(); 
    }

    rc_current_time = millis();
    if(rc_current_time - rc_last_received_time > 250){
        RC_ResetData();
        return false;
    }

    return true;
}

void RC_DisplayData(){
    /* En este bloque se muestran los valores de los joysticks, sliders y botones pulsadores en el monitor serial. */

    Serial.print("Joy1 X: ");
    Serial.print(rc_data.joy1_X);

    Serial.print(" | Joy1 Y: ");
    Serial.print(rc_data.joy1_Y);

    Serial.print(" | Joy1 Button: ");
    Serial.print(rc_data.joy1_Button);

    Serial.print(" | Joy2 X: ");
    Serial.print(rc_data.joy2_X);

    Serial.print(" | Joy2 Y: ");
    Serial.print(rc_data.joy2_Y);

    Serial.print(" | Joy2 Button: ");
    Serial.print(rc_data.joy2_Button);

    Serial.print(" | Joy3 Y: ");
    Serial.print(rc_data.joy3_X);

    Serial.print(" | Joy3 Y: ");
    Serial.print(rc_data.joy3_Y);

    Serial.print(" | Joy3 Button: ");
    Serial.print(rc_data.joy3_Button);

    Serial.print(" | Joy4 X: ");
    Serial.print(rc_data.joy4_X);

    Serial.print(" | Joy4 Y: ");
    Serial.print(rc_data.joy4_Y);

    Serial.print(" | Joy4 Button: ");
    Serial.print(rc_data.joy4_Button);

    Serial.print(" | Slider 1: ");
    Serial.print(rc_data.slider1);

    Serial.print(" | Slider 2: ");
    Serial.print(rc_data.slider2);

    Serial.print(" | Push Button 1: ");
    Serial.print(rc_data.pushButton1);

    Serial.print(" | Push Button 2: ");
    Serial.println(rc_data.pushButton2);
}

void RC_ResetData(){
   /* En este bloque se resetean los valores de los joysticks, sliders y botones pulsadores en caso de que no se reciban datos durante un cierto periodo de tiempo. */

   rc_data.joy1_X = 127;
   rc_data.joy1_Y = 127;
   rc_data.joy1_Button = 0;

   rc_data.joy2_X = 127;
   rc_data.joy2_Y = 127;
   rc_data.joy2_Button = 0;

   rc_data.joy3_X = 127;
   rc_data.joy3_Y = 127;
   rc_data.joy3_Button = 0;

   rc_data.joy4_X = 127;
   rc_data.joy4_Y = 127;
   rc_data.joy4_Button = 0;

   rc_data.joy5_X = 127;
   rc_data.joy5_Y = 127;
   rc_data.joy5_Button = 0;

   rc_data.slider1 = 75;
   rc_data.slider2 = 26;

   rc_data.pushButton1 = 0;
   rc_data.pushButton2 = 0;
}
