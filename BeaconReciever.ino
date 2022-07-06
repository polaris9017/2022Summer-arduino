/* Beacon Reciever implementation source */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>  // LCD Display library

#define LCD_CHARS 16

SoftwareSerial BSerial(5, 6); //BLE module | Tx to 5, Rx to 6
LiquidCrystal_I2C lcd(0x3f, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

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

void setup() {
    // write your initialization code here
    Serial.begin(9600);
    Serial.println("Initializing...");
    BSerial.begin(9600);

    scan_ble();
}

void loop() {
    // write your code here
    if (BSerial.available()) {
        String readVal = BSerial.readString();
        Serial.println("Recieved : " + readVal);
        delay(100);

        //�ϴ� �Ƶ��̳뿡 ������� ��� ������ ��°� Ȯ���ϰ� ������ ��
    }
}