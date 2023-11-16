#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keyboard.h>

#define LED_BLUE 5
#define LED_GREEN 6
#define SWITCH 7
#define BUZZER 8
#define LED_RED 9
#define SS_PIN 10 // SDA
#define RST_PIN 4 // RST

const int dTime = 500;

// Keyboard-Emulation sendet US-Tastaturlayout!!!

const char windows[] = "";
const char keypass[] = "";
const int chip[] = {230, 113, 248, 48}; // UID Chip
const int card[] = {227, 18, 142, 0};   // UID Karte
int code[4];        
int checkChip = 0;
int checkCard = 0;

bool rfidModeIsOn = false;
bool supportModeIsOn = false;

MFRC522 mfrc522(SS_PIN, RST_PIN); // Erzeuge Instanz von MFRC522

void accessGranted() {
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(50);
  digitalWrite(BUZZER, LOW);
  delay(1000);
  digitalWrite(LED_GREEN, LOW);
}

void accessDenied() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(50);
  digitalWrite(BUZZER, LOW);
  delay(50);
  digitalWrite(BUZZER, HIGH);
  delay(50);
  digitalWrite(BUZZER, LOW);
  delay(1000);
  digitalWrite(LED_RED, LOW);
}

void modeSupport() {
  rfidModeIsOn = false;
  if(supportModeIsOn == false) {    
    Serial.println("Wartungsmodus");
    digitalWrite(LED_BLUE, HIGH);
    delay(dTime);
    digitalWrite(LED_BLUE, LOW);
    delay(dTime);
    digitalWrite(LED_BLUE, HIGH);
    delay(dTime);
    digitalWrite(LED_BLUE, LOW);
    supportModeIsOn = true;
  }
  Keyboard.end();
  if(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {  // wenn Karte in Reichweite u. Inhalt erfolgreich ausgelesen
    for(byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i]);
      Serial.print(" ");
    }
    Serial.println();
    mfrc522.PICC_HaltA();   // Ruhemodus für Karte, wird nur einmalig ausgelesen
  }
}

void modeReadRFID() {
  supportModeIsOn = false;
  if(rfidModeIsOn == false) {
    Serial.println("Login-Modus");
    digitalWrite(LED_GREEN, HIGH);
    delay(dTime);
    digitalWrite(LED_GREEN, LOW);
    delay(dTime);
    digitalWrite(LED_GREEN, HIGH);
    delay(dTime);
    digitalWrite(LED_GREEN, LOW);
    rfidModeIsOn = true;
  }  
  if(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {  // wenn Karte in Reichweite u. Inhalt erfolgreich ausgelesen
    for(byte i = 0; i < mfrc522.uid.size; i++) {
      code[i] = mfrc522.uid.uidByte[i]; // Speichern der UID in Variable
      if(code[i] == chip[i]) {
        checkChip++;
      }
      else if(code[i] == card[i]) {
        checkCard++;
      }
    }
    if(checkChip == mfrc522.uid.size) {
      Keyboard.println(windows);
      accessGranted();       
    }
    else if(checkCard == mfrc522.uid.size) {
      Keyboard.println(keypass);
      accessGranted();
    }
    else {
      accessDenied();
    }
    checkChip = 0;
    checkCard = 0;
    mfrc522.PICC_HaltA();   // Ruhemodus für Karte, wird nur einmalig ausgelesen
  }
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Keyboard.begin();
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(SWITCH, INPUT);
}

void loop() {
  if(digitalRead(SWITCH) == HIGH) {
    modeReadRFID();
  }
  else {
    modeSupport();
  }
}