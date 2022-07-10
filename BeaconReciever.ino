/* Beacon Reciever implementation source */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>  // LCD Display library

#define LCD_CHARS 16
#define RSSI_LIMIT -90

const int nPin_BTTx = 2;  //전송
const int nPin_BTRx = 3;  //수신

const String addr_Beacon1 = "D4:36:39:69:8C:3A";
const String addr_Beacon2 = "78:04:73:B7:A6:12";
const String addr_Beacon3 = "78:04:73:B7:E4:35";

SoftwareSerial BSerial(nPin_BTTx, nPin_BTRx);  // RX, TX
LiquidCrystal_I2C lcd(0x27, 16, 2);            // set the LCD address to 0x27 for a 16 chars and 2 line display

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
    display("BLE module scan", 0, 0);
    delay(500);
    BSerial.println("AT+CLIENT=O");
    delay(500);
    BSerial.println("AT+SCANINTERVAL=2000");
    delay(500);
    BSerial.println("AT+SCAN=0");  //Scan mode
    display("BLE scan set!", 0, 0);
}

// initialize LCD display
void init_lcd() {
    lcd.init();  // initialize the lcd
    lcd.backlight();
}

String getRSSI(const String &msg) {
    int idx = msg.indexOf("RSSI[");;
    return msg.substring(idx + 5, idx + 8);
}

// determine location
char getLocation(const int *p) {
    if (p[0] > RSSI_LIMIT && p[1] < RSSI_LIMIT && p[2] < RSSI_LIMIT) return 'A';
    else if (p[0] < RSSI_LIMIT && p[1] > RSSI_LIMIT && p[2] < RSSI_LIMIT) return 'B';
    else if (p[0] < RSSI_LIMIT && p[1] < RSSI_LIMIT && p[2] > RSSI_LIMIT) return 'C';
    else if (p[0] > RSSI_LIMIT && p[1] > RSSI_LIMIT && p[2] < RSSI_LIMIT) return 'D';
    else if (p[0] > RSSI_LIMIT && p[1] < RSSI_LIMIT && p[2] > RSSI_LIMIT) return 'E';
    else if (p[0] < RSSI_LIMIT && p[1] > RSSI_LIMIT && p[2] > RSSI_LIMIT) return 'F';
    else if (p[0] > RSSI_LIMIT && p[1] > RSSI_LIMIT && p[2] > RSSI_LIMIT) return 'G';

    return 0;
}

// determine direction
String getDirection(const char loc) {
    switch (loc) {
        case 'A':
        case 'E':
        case 'G':
            return "N";
        case 'B':
            return "NNE";
        case 'C':
            return "NNW";
        case 'D':
            return "E";
        case 'F':
            return "W";
        default:
            return "";
    }
}

void setup() {
    Serial.begin(9600);
    BSerial.begin(9600);

    init_lcd();
    scan_ble();
}

void loop() {
    String val = "";
    int strength[3] = {};
    if (BSerial.available()) {
        val = BSerial.readString();

        int idx_addr = 6;
        String addr = val.substring(idx_addr + 2, idx_addr + 18);

        String rssi = getRSSI(val);

        if (addr == addr_Beacon1) strength[0] = rssi.toInt();
        else if (addr == addr_Beacon2) strength[1] = rssi.toInt();
        else if (addr == addr_Beacon3) strength[2] = rssi.toInt();

        //display(addr, 0, 0);
        char loc = getLocation(strength);
        display("RSSI = " + rssi, 1, 0);
        display("DIRECTION = " + getDirection(loc), 0, 0);
    }
}
