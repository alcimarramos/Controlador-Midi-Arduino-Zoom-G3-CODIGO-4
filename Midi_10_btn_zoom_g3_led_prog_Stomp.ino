//Bibliotecas utilizadas 
/* Bibliotecas utilizadas */
#include <MIDI.h>
#include <Usb.h>
#include <usbhub.h>
#include <usbh_midi.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

//Definindo nome para os pinos de entrada, onde ficam os botões 
#define pino2 A5   /*  pedal patch1 */ 
#define pino3 3   /*  pedal patch2 */
#define pino4 4   /*  pedal patch3 */
#define pino5 5   /*  pedal patch4 */
#define pino6 6   /*  pedal patch5 */
#define pino7 A0  /*  pedal Efeito 1 */
#define pino8 A1  /*  pedal Efeito 2 */
#define pino9 A2  /*  pedal Efeito 3 */
#define pino10 A3 /*  pedal Efeito 4 */
#define pino11 A4 /*  pedal Efeito 5 */

uint8_t _deviceID = 0x5A;
#define ZG3_CURRENT_PATCH_DATA_SIZE 120
int8_t CP_MEM[ZG3_CURRENT_PATCH_DATA_SIZE];
uint8_t       _readBuffer[MIDI_MAX_SYSEX_SIZE] = {0};

/* Pino onde liga o DIN dos LEDs */
#define PIN 7

//Numero de leds
#define NUMPIXELS 10

Adafruit_NeoPixel pixels=Adafruit_NeoPixel(NUMPIXELS,PIN,NEO_GRB+NEO_KHZ800);

//Definindo comunicação Midi USB
USB Usb; 
USBH_MIDI Midi(&Usb);

//Variáveis recebendo patches do Banco A
byte patchA0 = 0x00;
byte patchA1 = 0x01;
byte patchA2 = 0x02;
byte patchA3 = 0x03;
byte patchA4 = 0x04;
byte patchA5 = 0x05;
byte patchA6 = 0x06;
byte patchA7 = 0x07;
byte patchA8 = 0x08;
byte patchA9 = 0x09;

//Variáveis recebendo patches do Banco B
byte patchB0 = 0x0A;
byte patchB1 = 0x0B;
byte patchB2 = 0x0C;
byte patchB3 = 0x0D;
byte patchB4 = 0x0E;
byte patchB5 = 0x0F;
byte patchB6 = 0x10;
byte patchB7 = 0x11;
byte patchB8 = 0x12;
byte patchB9 = 0x13;

//Variáveis recebendo patches do Banco C
byte patchC0 = 0x14;
byte patchC1 = 0x15;
byte patchC2 = 0x16;
byte patchC3 = 0x17;
byte patchC4 = 0x18;
byte patchC5 = 0x19;
byte patchC6 = 0x1A;
byte patchC7 = 0x1B;
byte patchC8 = 0x1C;
byte patchC9 = 0x1D;

//Variáveis recebendo patches do Banco D
byte patchD0 = 0x1E;
byte patchD1 = 0x1F;
byte patchD2 = 0x20;
byte patchD3 = 0x21;
byte patchD4 = 0x22;
byte patchD5 = 0x23;
byte patchD6 = 0x24;
byte patchD7 = 0x25;
byte patchD8 = 0x26;
byte patchD9 = 0x27;

//Variáveis que receberão  o acionamento dos botões nos pinos de entrada
int pedal1;
int pedal2;
int pedal3;
int pedal4;
int pedal5;
int pedal6;
int pedal7;
int pedal8;
int pedal9;
int pedal10;

//Variáveis que receberão valores 0 ou 1 para  o mudança dos 2 patches em único pedal
int valorPedal1;
int valorPedal2;
int valorPedal3;
int valorPedal4;
int valorPedal5;
int valorPedal6;
int valorPedal7;
int valorPedal8;
int valorPedal9;
int valorPedal10;
int bank_Down;
int bank_Up;
int i = 0;
int bank = 0;
int outPinErr = 13;

byte efx[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05} ;
byte stefx[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

int espera = 10; //Tempo em ms até poder enviar o comando novamente

void setup() {

//Definindo pinos de entrada
  pinMode(pino2, INPUT_PULLUP);
  pinMode(pino3, INPUT_PULLUP);
  pinMode(pino4, INPUT_PULLUP);
  pinMode(pino5, INPUT_PULLUP);
  pinMode(pino6, INPUT_PULLUP);
  pinMode(pino7, INPUT_PULLUP);
  pinMode(pino8, INPUT_PULLUP);
  pinMode(pino9, INPUT_PULLUP);
  pinMode(pino10, INPUT_PULLUP);
  pinMode(pino11, INPUT_PULLUP);
  
  pixels.begin();
  pixels.setBrightness(50);
  apagaLed();

  Serial.begin(115200); //inicia comunicação serial e define a velocidade
  iniciar();

}

void loop() {

  //Variáveis que recebem  o acionamento dos botões nos pinos de entrada
   pedal1 = digitalRead(pino2);
   pedal2 = digitalRead(pino3);
   pedal3 = digitalRead(pino4);
   pedal4 = digitalRead(pino5);
   pedal5 = digitalRead(pino6);
   pedal6 = digitalRead(pino7);
   pedal7 = digitalRead(pino8);
   pedal8 = digitalRead(pino9);
   pedal9 = digitalRead(pino10);
   pedal10 = digitalRead(pino11);


//////////////////// PEDAL 1 ////////////////////
  if ( pedal1 == LOW ) { /*----------------------- Se o pedal 1 for pressionado */
      valorPedal1 = !valorPedal1; /*-------------- Valor do pedal inverte sempre que é acionado entre 0 e 1 */
      
      if ( pedal1 == LOW && valorPedal1 == 1 ){
      SendMIDI(patchA1 + (bank * 10)); /*--------- Envia o patch A1, e sobe, ou desce, 20 presets cada vez que o pedal benco é acionado */
      led1on(); /*-------------------------------- Liga o Led do pedal */
      delay(espera); /*--------------------------- Tempo até poder enciar o comando novamente */ 
        editmodeonoff();
      }
      
      else if ( pedal1 == LOW && valorPedal1 == 0 ){
      SendMIDI(patchA2 + (bank * 10)); /*--------- Envia o patch B1, e sobe, ou desce, 20 presets cada vez que o pedal benco é acionado */
      led1on_(); /*------------------------------- Liga o Led do pedal */
      delay(espera); /*--------------------------- Tempo até poder enciar o comando novamente */ 
        editmodeonoff();
      }
    valorPedaloff1(); /*-------------------------- Zera o valor dos outros pedais */
  }

//////////////////// PEDAL 2 ////////////////////
  if ( pedal2 == LOW ) { /*----------------------- Se o pedal 1 for pressionado */
      valorPedal2 = !valorPedal2; /*-------------- Valor do pedal inverte sempre que é acionado entre 0 e 1 */
      
      if ( pedal2 == LOW && valorPedal2 == 1 ){
      SendMIDI(patchA3 + (bank * 10)); /*--------- Envia o patch A1, e sobe, ou desce, 20 presets cada vez que o pedal benco é acionado */
      led2on(); /*-------------------------------- Liga o Led do pedal */
      delay(espera); /*--------------------------- Tempo até poder enciar o comando novamente */ 
        editmodeonoff();
      }
      
      else if ( pedal2 == LOW && valorPedal1 == 0 ){
      SendMIDI(patchA4 + (bank * 10)); /*--------- Envia o patch B1, e sobe, ou desce, 20 presets cada vez que o pedal benco é acionado */
      led2on_(); /*------------------------------- Liga o Led do pedal */
      delay(espera); /*--------------------------- Tempo até poder enciar o comando novamente */ 
        editmodeonoff();
      }
    valorPedaloff2(); /*-------------------------- Zera o valor dos outros pedais */
  }

//////////////////// PEDAL 3 ////////////////////
  if ( pedal3 == LOW ) { /*----------------------- Se o pedal 1 for pressionado */
      valorPedal3 = !valorPedal3; /*-------------- Valor do pedal inverte sempre que é acionado entre 0 e 1 */
      
      if ( pedal3 == LOW && valorPedal3 == 1 ){
      SendMIDI(patchA5 + (bank * 10)); /*--------- Envia o patch A1, e sobe, ou desce, 20 presets cada vez que o pedal benco é acionado */
      led3on(); /*-------------------------------- Liga o Led do pedal */
      delay(espera); /*--------------------------- Tempo até poder enciar o comando novamente */ 
        editmodeonoff();
      }
      
      else if ( pedal3 == LOW && valorPedal3 == 0 ){
      SendMIDI(patchA6 + (bank * 10)); /*--------- Envia o patch B1, e sobe, ou desce, 20 presets cada vez que o pedal benco é acionado */
      led3on_(); /*------------------------------- Liga o Led do pedal */
      delay(espera); /*--------------------------- Tempo até poder enciar o comando novamente */ 
        editmodeonoff();
      }
    valorPedaloff3(); /*-------------------------- Zera o valor dos outros pedais */
  }

//////////////////// PEDAL 4 ////////////////////
  if ( pedal4 == LOW ) { /*----------------------- Se o pedal 1 for pressionado */
      valorPedal4 = !valorPedal4; /*-------------- Valor do pedal inverte sempre que é acionado entre 0 e 1 */
      
      if ( pedal4 == LOW && valorPedal4 == 1 ){
      SendMIDI(patchA7 + (bank * 10)); /*--------- Envia o patch A1, e sobe, ou desce, 20 presets cada vez que o pedal benco é acionado */
      led4on(); /*-------------------------------- Liga o Led do pedal */
      delay(espera); /*--------------------------- Tempo até poder enciar o comando novamente */ 
        editmodeonoff();
      }
      
      else if ( pedal4 == LOW && valorPedal4 == 0 ){
      SendMIDI(patchA8 + (bank * 10)); /*--------- Envia o patch B1, e sobe, ou desce, 20 presets cada vez que o pedal benco é acionado */
      led4on_(); /*------------------------------- Liga o Led do pedal */
      delay(espera); /*--------------------------- Tempo até poder enciar o comando novamente */ 
        editmodeonoff();
      }
    valorPedaloff4(); /*-------------------------- Zera o valor dos outros pedais */
  }

//////////////////// PEDAL 5 ////////////////////
  if ( pedal5 == LOW ) { /*----------------------- Se o pedal 1 for pressionado */
      valorPedal5 = !valorPedal5; /*-------------- Valor do pedal inverte sempre que é acionado entre 0 e 1 */
      
      if ( pedal5 == LOW && valorPedal5 == 1 ){
      SendMIDI(patchA9 + (bank * 10)); /*--------- Envia o patch A1, e sobe, ou desce, 20 presets cada vez que o pedal benco é acionado */
      led5on(); /*-------------------------------- Liga o Led do pedal */
      delay(espera); /*--------------------------- Tempo até poder enciar o comando novamente */ 
        editmodeonoff();
      }
      
      else if ( pedal5 == LOW && valorPedal5 == 0 ){
      SendMIDI(patchA0 + (bank * 10)); /*--------- Envia o patch B1, e sobe, ou desce, 20 presets cada vez que o pedal benco é acionado */
      led5on_(); /*------------------------------- Liga o Led do pedal */
      delay(espera); /*--------------------------- Tempo até poder enciar o comando novamente */ 
        editmodeonoff();
      }
    valorPedaloff5(); /*-------------------------- Zera o valor dos outros pedais */
  }
  

//////////////////// PEDAL 6 ////////////////////

  if ( !pedal6 ) {
       Sendonoff(stefx[0], 0);
  }

//////////////////// PEDAL 7 ////////////////////

  if ( !pedal7) {
       Sendonoff(stefx[1], 1);
  }

//////////////////// PEDAL 8 ////////////////////

  if ( !pedal8) {
       Sendonoff(stefx[2], 2);
  }

//////////////////// PEDAL 9 ////////////////////

  if ( !pedal9 ) {
       Sendonoff(stefx[3], 3);
  }

//////////////////// PEDAL 10 ///////////////////

  if ( !pedal10 ) {
       Sendonoff(stefx[4], 4);
  }  

}


/* --------------------------------------------------------------------- */
/* ****************************** FUNÇÔES ****************************** */
/* --------------------------------------------------------------------- */


void SendMIDI(byte number) { //Função para enviar os comandos Midi
  Usb.Task();
  byte Message[2];           // Construindo mensagem midi (2 bytes)
  Message[0] = 0xC0;         // 0xC0 para Program Change
  Message[1] = number;       // Number é o program/patch
  Midi.SendData(Message);    // Envia a mensagem 
  delay(10);
}

void editmodeonoff(){
    enableEditorMode(false);
    delay(10);
    enableEditorMode(true);
    requestPatchData();
    delay(10);
    enableEditorMode(false);
}

void iniciar(){
  if (Usb.Init() == -1) { // Testa se o USB foi conectado e aguarda conectar
  while (1);
  }
  while (Usb.getUsbTaskState() != USB_STATE_RUNNING) {
    Usb.Task(); //se USB iniciou, continue
  }
  for (int i = 0; i < 4; i++) { //forçando os 4 primeiros comandos enviados pra pedaleira que são ignorados
    SendMIDI(patchA1);
    delay(200);
  }
  led1on();
  editmodeonoff();
}


void Sendonoff( byte state, int num){
  Usb.Task();
  stefx[num] = !state;
  uint8_t activeedit[] = {0xF0,0x52,0x00,0x5A,0x50,0xF7}; 
  uint8_t onoff[] = {0xF0, 0x52, 0x00, 0x5A, 0x31, efx[num], 0x00, stefx[num], 0x00, 0xF7}; //zoom G3
  delay(50);
  Midi.SendSysEx(activeedit, sizeof(activeedit)); // Send the message
  delay(50);
  Midi.SendSysEx(onoff, sizeof(onoff));           // Send the message
  delay(100);
}


void readResponse() {
    uint16_t recv_read = 0;
    uint16_t recv_count = 0;
    uint8_t rcode = 0;
    delay(400);     
    Usb.Task();
    do {rcode = Midi.RecvData(&recv_read, (uint8_t *)(_readBuffer + recv_count));
           if(rcode == 0) {
           recv_count += recv_read;  
           }
        }
        while(/*recv_count < MIDI_MAX_SYSEX_SIZE &&*/ rcode == 0);
        for(int i = 0, j = 0; i < MIDI_MAX_SYSEX_SIZE; i++) {
        _readBuffer[j++] = _readBuffer[++i];
        _readBuffer[j++] = _readBuffer[++i];
        _readBuffer[j++] = _readBuffer[++i]; 
        }
  }


void sendBytes(uint8_t * aBytes, const __FlashStringHelper * aMessage = NULL) {
     Usb.Task();
     Midi.SendData(aBytes);
}


 void requestPatchData() {
  uint8_t     PD_PAK[] = { 0xF0, 0x52, 0x00, 0x5A /* device ID */, 0x29, 0xF7 };
    PD_PAK[3] = _deviceID;
    sendBytes(PD_PAK, F("REQ PATCH DATA"));
    readResponse();
    memcpy( CP_MEM, _readBuffer, ZG3_CURRENT_PATCH_DATA_SIZE );
    stefx[0] = (CP_MEM[6] &1);
    stefx[1] = (CP_MEM[19]&1);
    stefx[2] = (CP_MEM[33]&1);
    stefx[3] = (CP_MEM[47]&1);
    stefx[4] = (CP_MEM[60]&1);
    stefx[5] = (CP_MEM[74]&1);
}


void enableEditorMode(bool aEnable) {
  uint8_t       EM_PAK[] = { 0xF0, 0x52, 0x00, 0x5A /* device ID */, 0x50 /* 0x50: on - 0x51: off */, 0xF7 };// set editor mode on / off
  EM_PAK[3] = _deviceID;
  EM_PAK[4] = aEnable ? 0x50 : 0x51;
  sendBytes(EM_PAK, F("EDITOR ON"));
}


//Acende o Led 1 e apaga os demais
void led1on() {
  pixels.setPixelColor(9, pixels.Color(255,0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}
  
void led1on_() {
  pixels.setPixelColor(9, pixels.Color(0, 255,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}


//Acende o Led 2 e apaga os demais
void led2on() {
  pixels.setPixelColor(9, pixels.Color(0,0,0));
  pixels.setPixelColor(8, pixels.Color(255, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}
  
void led2on_() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 255, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}

//Acende o Led 3 e apaga os demais
void led3on() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(255, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}

void led3on_() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 255, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}

//Acende o Led 4 e apaga os demais
void led4on() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(255, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
  }
  
void led4on_() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 255, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}

//Acende o Led 5 e apaga os demais
void led5on() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(255, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}
  
void led5on_() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 255, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}

//Acende o Led 6 e apaga os demais
void led6on() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(255, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
  }
  
  void led6on_() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 255, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}

//Acende o Led 7 e apaga os demais
  void led7on() {
  pixels.setPixelColor(9, pixels.Color(0, 0, 0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(255, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}

  void led7on_() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 255, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}

//Acende o Led 8 e apaga os demais
  void led8on() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(255, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}

  void led8on_() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 255, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}

//Acende o Led 9 e apaga os demais
  void led9on() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();
}

  void led9on_() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.show();
}

  void led10on() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(4, pixels.Color(255, 0, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}

  void led10on_() {
  pixels.setPixelColor(9, pixels.Color(0, 0,0));
  pixels.setPixelColor(8, pixels.Color(0, 0, 0));
  pixels.setPixelColor(7, pixels.Color(0, 0, 0));
  pixels.setPixelColor(6, pixels.Color(0, 0, 0));
  pixels.setPixelColor(4, pixels.Color(0, 255, 0));
  pixels.setPixelColor(5, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}



//Led do banco
  void ledbankon() {
  pixels.setPixelColor(4, pixels.Color(255, 0, 255));
  pixels.show();
  }
  
  void ledbankon_() {
  pixels.setPixelColor(4, pixels.Color(0, 0, 255));
  pixels.show();
}


//Zera os valores todos os pedais
void valorPedaloff() {
     valorPedal1 = 0;
     valorPedal2 = 0;
     valorPedal3 = 0;
     valorPedal4 = 0;
     valorPedal5 = 0;
     valorPedal6 = 0;
     valorPedal7 = 0;
     valorPedal8 = 0;
     valorPedal9 = 0;
    }

//Zera os valores todos os pedais exceto o Pedal 1
void valorPedaloff1() {
     valorPedal2 = 0;
     valorPedal3 = 0;
     valorPedal4 = 0;
     valorPedal5 = 0;
    }

//Zera os valores todos os pedais exceto o Pedal 2
void valorPedaloff2() {
     valorPedal1 = 0;
     valorPedal3 = 0;
     valorPedal4 = 0;
     valorPedal5 = 0;
    }

//Zera os valores todos os pedais exceto o Pedal 3
void valorPedaloff3() {
     valorPedal1 = 0;
     valorPedal2 = 0;
     valorPedal4 = 0;
     valorPedal5 = 0;
    }

//Zera os valores todos os pedais exceto o Pedal 4
void valorPedaloff4() {
     valorPedal1 = 0;
     valorPedal2 = 0;
     valorPedal3 = 0;
     valorPedal5 = 0;
    }

//Zera os valores todos os pedais exceto o Pedal 5
void valorPedaloff5() {
     valorPedal1 = 0;
     valorPedal2 = 0;
     valorPedal3 = 0;
     valorPedal4 = 0;
    }

void apagaLed() {
  for (int i = 0; i <= 6 ; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
}


/* Função para Descer os Bancos */
void bankDown() {
  if( i > 0 ){
      i--;
      bank = i;
      delay(50);
   }
    if( i == 0 ) { 
       i = 0;
       bank = i;
       delay(50);
      }
}

/* Função para Subir os Bancos */
void bankUp() {
  if( i >= 0 && i <= 4 ){
      i++;
      bank = i;
      delay(50);
   }
    if( i > 4 ) { 
       i = 0;
       bank = i;
       delay(50);
      }
}
