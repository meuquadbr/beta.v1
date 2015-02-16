/*######################################################################################   

v888b    d888 8888888888 888     888       .d88888b.  888     888       d8888 8888888b.  
8888b   d8888 888        888     888      d88P" "Y88b 888     888      d88888 888  "Y88b 
88888b.d88888 888        888     888      888     888 888     888     d88P888 888    888 
888Y88888P888 8888888    888     888      888     888 888     888    d88P 888 888    888 
888 Y888P 888 888        888     888      888     888 888     888   d88P  888 888    888 
888  Y8P  888 888        888     888      888 Y8b 888 888     888  d88P   888 888    888 
888   "   888 888        Y88b. .d88P      Y88b.Y8b88P Y88b. .d88P d8888888888 888  .d88P 
888       888 8888888888  "Y88888P"        "Y888888"   "Y88888P" d88P     888 8888888P"  
                                                 Y8b                                                                                                                                                                                                          
########################################################################################
                                  MEUQUAD.UNI.ME
                               FIRMWARE VERSÃO 3.0b
########################################################################################

########################################################################################   
                                  MODO DE DEBUG
######################################################################################*/
// #define DEBUG_ANGULO
// #define DEBUG_VELOCIDADES
// #define DEBUG_OUT_PID
// #define DEBUG_RECEPTOR
// #define DEBUG_YAW
// #define DEBUG_EEPROM
// #define DEBUG_CONTROLE

/*####################################################################################   
                        CONFIGURAÇÕES GERAIS DO QUADRICÓPTERO
#####################################################################################*/

#define MIN_ESC 1000
#define MAX_ESC 2000

#define MAX_INFLUENCIA_PID 800 //200
#define MAX_INFLUENCIA_PID_Z 800 //400

//PID X
#define KPX 4 //4
#define KIX 2.5 //2.5 //3 //5 //6
#define KDX 1.2 //1
#define CENTROX -2.22

//PID Y
#define KPY 4
#define KIY 2.5
#define KDY 1.2
#define CENTROY -0.43

//PID Z
#define KPZ 10 //11 //7.6
#define KIZ 0.5 //4.3 //5.5 //6 //6.5 //4
#define KDZ 2 //1.5 //0.5

//Definição de pinos
#define M0_PIN 3
#define M1_PIN 5
#define M2_PIN 6
#define M3_PIN 9

#define TEMPO_ESPERA 20 // Tempo de calibrar o mpu em segundos
#define ANGULO_MAXIMO 10

/*####################################################################################   
                                BIBLIOTECAS NECESSÁRIAS
#####################################################################################*/

#include "I2Cdev.h"
#include <MPU6050_6Axis_MotionApps20.h>
#include <Wire.h>
#include <Servo.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include "PID_v1.h" 
#include <MirfHardwareSpiDriver.h>
#include <EEPROMex.h>

/*####################################################################################   
                                VARIÁVEIS NECESSÁRIAS
#####################################################################################*/

//Controle
int V_MEDIA;
byte dados[21];
double angleY,angleX,angleZ;
double OutputY, OutputX, OutputZ;
double SetpointX = CENTROX;
double SetpointY = CENTROY;
double SetpointZ = 0;
double CENTROZ = 0;
double MeiodoX = CENTROX;
double MeiodoY = CENTROY;
int modY,modX,modZ;
PID ControleY(&angleY, &OutputY, &SetpointY, KPY, KIY, KDY, DIRECT);
PID ControleX(&angleX, &OutputX, &SetpointX, KPX, KIX, KDX, DIRECT);
PID ControleZ(&angleZ, &OutputZ, &SetpointZ, KPZ, KIZ, KDZ, DIRECT);

//Motores
Servo motor[4];
int v[4];

// Relacionados ao MPU6050
MPU6050 mpu;
bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;
VectorFloat gravity;
float ypr[3];

uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

volatile bool mpuInterrupt = false;
void dmpDataReady() {
    mpuInterrupt = true;
}

bool definiuZ = false;

//EEPROM
struct pardados {
int key;
float value;
};
pardados XYInputEEPROM[2];
pardados XYOutputEEPROM[2];

/*####################################################################################   
                                        SETUP
#####################################################################################*/
void setup() {
    iniMPU();    
    iniReceptor();
    iniMotores();
    iniControle();
    iniDebugMode();
    lereeprom();
}
/*####################################################################################   
                                        LOOP
#####################################################################################*/
void loop() {
    if (!dmpReady) return;
    while (!mpuInterrupt && fifoCount < packetSize) {
        if(millis()>TEMPO_ESPERA*1000){
            if(!definiuZ){
                SetpointZ = angleZ;
                CENTROZ = SetpointZ; 
                definiuZ = true;
            }
            receberSemFio();
            controlar();
        }
    }
    receberSemFio();
    debugMode();
    pegaXYZ();
}
