/* Beacon Reciever implementation source */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>  // LCD Display library
#include <nRF24L01.h>
#include <RF24.h>

#define LCD_CHARS 16

SoftwareSerial BSerial(5, 6); //BLE module | Tx to 5, Rx to 6
LiquidCrystal_I2C lcd(0x3f, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
RF24 radio(8, 10);

const byte addr[6] = "R0001";

// Clear line on display
void clearLine(int line) {
    for (int i = 0; i < LCD_CHARS; i++) {
        lcd.setCursor(i, line);
        lcd.print(" ");
    }
}

/**
* Print string to display
* @param msg : String to display
* @param line : line in display (range: 0 ~ 1)
* @param pos : position in display (range: 0~15)
*/
void display(String msg, int line, int pos) {  // print to display
    clearLine(line);
    lcd.setCursor(pos, line);
    lcd.print(msg);
}

// set BLE module to scan mode
void scan_ble() {
    Serial.println("Setting BLE module to scan mode");
    delay(1000);
    BSerial.println("AT+SCAN");  //Scan mode
}

// initialize RF module
void init_radio() {
    radio.begin();
    radio.openReadingPipe(0, addr);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
}

void setup() {
    // write your initialization code here
    Serial.begin(9600);
    Serial.println("Initializing...");
    BSerial.begin(9600);

    scan_ble();
    init_radio();
}

void loop() {
    // write your code here
    if (radio.available()) {
        String msg;
        radio.read(&msg, 32 * sizeof(char));
        if (msg.equals("QWxlcnQhIQ==")) { // received alert message
            if (BSerial.available()) {
                String readVal = BSerial.readString();
                Serial.println("Recieved : " + readVal);
                delay(100);

                //일단 아두이노에 블루투스 모듈 물려서 출력값 확인하고 구현할 것
            }
        }
    }
}