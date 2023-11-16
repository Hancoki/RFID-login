#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keyboard.h>

#define LED_BLUE 5
#define LED_GREEN 6
#define SWITCH 7
#define BUZZER 8
#define LED_RED 9
#define SS_PIN 10
#define RST_PIN 4

const int DTIME = 500; // Delay for LED's

// Define your login credentials here, just for compiling the code 
const char password1[] = "";
const char password2[] = "";
// [...]

// !!! WARNING: IT IS ABSOLUTELY NOT RECOMMENT TO SAVE YOUR LOGIN CREDENTIALS IN SOURCE CODE !!!

// Define the UID's from your RFID tags as an array of integer
const int tag1[4] = {230, 113, 248, 48}; // chip
const int tag2[4] = {227, 18, 142, 0};   // card
// [...]

int code[4];        
int checkChip = 0;
int checkCard = 0;

bool rfidModeIsOn = false;
bool supportModeIsOn = false;

MFRC522 mfrc522(SS_PIN, RST_PIN); // Creating an instance of MFRC522

void grantAccess() {
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(DTIME/10);
  digitalWrite(BUZZER, LOW);
  delay(2*DTIME);
  digitalWrite(LED_GREEN, LOW);
}

void denyAccess() {
  digitalWrite(LED_RED, HIGH);
  for(int i = 0; i < 3; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(DTIME/10);
    digitalWrite(BUZZER, LOW);
    delay(DTIME/10);
  }
  delay(2*DTIME);
  digitalWrite(LED_RED, LOW);
}

void activateSupportMode() {
  rfidModeIsOn = false;
  if(supportModeIsOn == false) {   
    supportModeIsOn = true;
    digitalWrite(LED_BLUE, HIGH); 
    Serial.println("Welcome to maintenance mode! Here you can discover the UID's of your RFID tags.");
  }

  Keyboard.end();

  if(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {  // Checking presence of RFID tag and a readable UID
    for(byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i]);
      Serial.print(" ");
    }
    Serial.println();
    mfrc522.PICC_HaltA();   // Go on halt to read the tag only one time
  }
}

void activateRfidMode() {
  supportModeIsOn = false;
  digitalWrite(LED_BLUE, LOW);
  if(rfidModeIsOn == false) {
    rfidModeIsOn = true;
    for(int i = 0; i < 2; i++) {
      digitalWrite(LED_GREEN, HIGH);
      delay(DTIME);
      digitalWrite(LED_GREEN, LOW);
      delay(DTIME);
      digitalWrite(LED_BLUE, HIGH);
      delay(DTIME);
      digitalWrite(LED_BLUE, LOW);
      delay(DTIME);
      digitalWrite(LED_RED, HIGH);
      delay(DTIME);
      digitalWrite(LED_RED, LOW);
      delay(DTIME);
    }
  }

  if(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {  // Checking presence of RFID tag and a readable UID
    for(byte i = 0; i < mfrc522.uid.size; i++) {
      code[i] = mfrc522.uid.uidByte[i]; // Saving the UID
      if(code[i] == tag1[i]) {
        checkChip++;
      }
      else if(code[i] == tag2[i]) {
        checkCard++;
      }
    }
    if(checkChip == mfrc522.uid.size) {
      Keyboard.println(password1);
      grantAccess();       
    }
    else if(checkCard == mfrc522.uid.size) {
      Keyboard.println(password2);
      grantAccess();
    }
    else {
      denyAccess();
    }
    checkChip = 0;
    checkCard = 0;
    mfrc522.PICC_HaltA(); // Go on halt to read the tag only one time
  }
}

void setup() {
  Serial.begin(9600); // Initialize interfaces
  SPI.begin();
  mfrc522.PCD_Init();
  Keyboard.begin();
  
  pinMode(LED_BLUE, OUTPUT);  // Setup IO pins
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(SWITCH, INPUT);
}

void loop() {
  if(digitalRead(SWITCH) == HIGH) { // Switch for activating the RFID mode
    activateRfidMode();
  } else {
    activateSupportMode();
  }
}