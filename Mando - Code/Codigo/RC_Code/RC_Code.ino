// Inclusion de librerias

#include <SPI.h> // Incluye la biblioteca SPI para la comunicación serial
#include <nRF24L01.h> // Incluye la biblioteca para el módulo de radio nRF24L01
#include <RF24.h> // Incluye la biblioteca RF24 para manejar el módulo de radio

#include <SoftwareSerial.h> // Para trabajar con un puerto serial extra de ser NECESARIO

//________________________________________________________________________________________________________________________________________________


//________________________________________________________________________________________________________________________________________________

RF24 radio(9, 10); // Crea una instancia de la clase RF24, los parámetros son los pines CE y CSN

const byte address[6] = "00011"; // Dirección del canal de comunicación

//________________________________________________________________________________________________________________________________________________

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

//________________________________________________________________________________________________________________________________________________

RC_Data_Package rc_data; // Crea una instancia de la estructura RC_Data_Package

// Define los pines a los que están conectados los joysticks, sliders y botones pulsadores
const int joy1XPin = A4;
const int joy1YPin = A5;
const int joy1ButtonPin = 3;

const int joy2XPin = A3;
const int joy2YPin = A2;
const int joy2ButtonPin = 6;

const int joy3XPin = A14;
const int joy3YPin = A15;
const int joy3ButtonPin = 20;

const int joy4XPin = A12;
const int joy4YPin = A13;
const int joy4ButtonPin = 19;

const int joy5XPin = A1;
const int joy5YPin = A0;
const int joy5ButtonPin = 7;

const int slider1Pin = A8;
const int slider2Pin = A10;
const int slider3Pin = A11;
const int slider4Pin = A9;

const int pushButton1Pin = 6;
const int pushButton2Pin = 7;

int oldpotVal = -1;

void setup()
{
  Serial.begin(9600); // Inicia la comunicación serial a una velocidad de transmisión de datos de 9600 baudios

  pinMode(joy1ButtonPin,INPUT); // Configura el pin del botón del joystick 1 como entrada
  digitalWrite(joy1ButtonPin, HIGH); // Activa la resistencia pull-up interna

  pinMode(joy2ButtonPin,INPUT); // Configura el pin del botón del joystick 2 como entrada
  digitalWrite(joy2ButtonPin, HIGH); // Activa la resistencia pull-up interna

  pinMode(joy3ButtonPin,INPUT); // Configura el pin del botón del joystick 1 como entrada
  digitalWrite(joy3ButtonPin, HIGH); // Activa la resistencia pull-up interna

  pinMode(joy4ButtonPin,INPUT); // Configura el pin del botón del joystick 2 como entrada
  digitalWrite(joy4ButtonPin, HIGH); // Activa la resistencia pull-up interna

  pinMode(joy5ButtonPin,INPUT); // Configura el pin del botón del joystick 2 como entrada
  digitalWrite(joy5ButtonPin, HIGH); // Activa la resistencia pull-up interna

  pinMode(pushButton1Pin,INPUT); // Configura el pin del botón pulsador 1 como entrada
  digitalWrite(pushButton1Pin, HIGH); // Activa la resistencia pull-up interna

  pinMode(pushButton2Pin,INPUT); // Configura el pin del botón pulsador 2 como entrada
  digitalWrite(pushButton2Pin, HIGH); // Activa la resistencia pull-up interna

  radio.begin(); // Inicia el módulo de radio
  radio.openWritingPipe(address); // Abre un canal de escritura con la dirección especificada
  radio.setPALevel(RF24_PA_MIN); // Establece el nivel de potencia de salida al mínimo
  radio.stopListening(); // Pone el módulo de radio en modo transmisión (en lugar de recepción)
}

void loop()
{
    /* En este bloque se leen los valores de los joysticks, sliders y botones pulsadores,
    se mapean a un rango adecuado y se almacenan en la estructura rc_data. */

    rc_data.joy1_X = map(analogRead(joy1XPin), 0, 1023, 0, 255);
    rc_data.joy1_Y = map(analogRead(joy1YPin), 0, 1023, 0, 255);
    rc_data.joy1_Button = !digitalRead(joy1ButtonPin);

    rc_data.joy2_X = map(analogRead(joy2XPin), 0, 1023, 0, 255);
    rc_data.joy2_Y = map(analogRead(joy2YPin), 0, 1023, 0, 255);
    rc_data.joy2_Button = !digitalRead(joy2ButtonPin);

    rc_data.joy3_X = map(analogRead(joy3XPin), 0, 1023, 0, 255);
    rc_data.joy3_Y = map(analogRead(joy3YPin), 0, 1023, 0, 255);
    rc_data.joy3_Button = !digitalRead(joy3ButtonPin);

    rc_data.joy4_X = map(analogRead(joy4XPin), 0, 1023, 0, 255);
    rc_data.joy4_Y = map(analogRead(joy4YPin), 0, 1023, 0, 255);
    rc_data.joy4_Button = !digitalRead(joy4ButtonPin);

    rc_data.joy5_X = map(analogRead(joy5XPin), 0, 1023, 0, 255);
    rc_data.joy5_Y = map(analogRead(joy5YPin), 0, 1023, 0, 255);
    rc_data.joy5_Button = !digitalRead(joy5ButtonPin);

    rc_data.slider1 = map(smoothPotVal(analogRead(slider1Pin)),0,1023,100,0);
    rc_data.slider2 = map(smoothPotVal(analogRead(slider2Pin)),0,1023,100,0);

    rc_data.pushButton1 = !digitalRead(pushButton1Pin);
    rc_data.pushButton2 = !digitalRead(pushButton2Pin);

    RC_DisplayData(); // Llama a la función RC_DisplayData para mostrar los datos en el monitor serial

    radio.write(&rc_data, sizeof(RC_Data_Package)); // Envía los datos a través del módulo de radio

}

int smoothPotVal(int potVal){
  int potValComp;

  /* En este bloque se ajusta el valor leído del potenciómetro para que tenga una respuesta más lineal. */

  if (potVal <= 44)
  {
    potValComp = map(potVal, 0, 44, 0, 255); // bottom 25% of pot remapped to 0 to 25% of 1023
  }
  if (potVal > 44 && potVal < 975)
  {
    potValComp = map(potVal, 45, 974, 255, 767); // 50% middle of pot remapped to 25% to 75%% of 1023
  }
  if (potVal >= 975 )
  {
    potValComp = map(potVal, 975, 1023, 767, 1023); // top 25% of pot remapped to 75% to 100%% of 1023
  }

  return potValComp;
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

