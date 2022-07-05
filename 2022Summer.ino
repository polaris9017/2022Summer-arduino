// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       2022Summer.ino
    Created:	2022-07-02 오후 5:32:37
    Author:     DESKTOP-FEJUQIH\Kim.K
*/

// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

// The setup() function runs once each time the micro-controller starts
#include <OneWire.h>
#include <DallasTemperature.h>  // Temperature sensor library
#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>  // LCD Display library

#define ONE_WIRE_BUS 2
#define LIGHT_SENSOR_BUS 0

#define LED_PIN_RED 11
#define LED_PIN_BLUE 10
#define LED_PIN_GREEN 9
#define LED_ON 255
#define LED_OFF 0

LiquidCrystal_I2C lcd(0x3f, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);

SoftwareSerial BSerial(5, 6); //BLE module | Tx to 5, Rx to 6

/**
* Print string to display
* @param msg : String to display
* @param line : line in display (range: 0 ~ 1)
* @param pos : position in display (range: 0~15)
*/
void display(String msg, int line, int pos) {  // print to display
    lcd.setCursor(pos, line);
    lcd.print(msg);
}

// get temperature from temperature sensor
float get_temperature() {
    tempSensor.requestTemperatures();
    return tempSensor.getTempCByIndex(0);
}

// get light level from light sensor
int get_light_level() {
    return analogRead(LIGHT_SENSOR_BUS);
}

// set BLE module to scan mode
void scan_ble() {
    delay(1000);
    BSerial.println("AT+SCAN");  //Scan mode
}

void led_beep() {
    analogWrite(LED_PIN_RED, 255);
}

//LED module test
void led_init_test() {
    Serial.println("Testing LED module");

    Serial.println("255 -> 0");
    for (int val = 255; val > 0; val--) {
        analogWrite(LED_PIN_RED, val);
        analogWrite(LED_PIN_BLUE, 255-val);
        analogWrite(LED_PIN_GREEN, 128-val);
        delay(1);
    }

    Serial.println("0 -> 255");
    for (int val = 0; val < 255; val++) {
        analogWrite(LED_PIN_RED, val);
        analogWrite(LED_PIN_BLUE, 255 - val);
        analogWrite(LED_PIN_GREEN, 128 - val);
        delay(1);
    }

    Serial.println("LED test done!");
}

void toggle_led(uint8_t color, int mode) {
    analogWrite(color, mode);
}

void setup() {
    lcd.init();
    lcd.backlight();

    tempSensor.begin();  //Init temp. sensor

    pinMode(LED_PIN_RED, OUTPUT);
    pinMode(LED_PIN_BLUE, OUTPUT);
    pinMode(LED_PIN_GREEN, OUTPUT);
    led_init_test();

    Serial.begin(9600);  //baud rate
    //Serial.println("d");
}

// Add the main program code into the continuous loop() function
void loop() {
    display("test message!", 0, 1);
    display("Temp: " + String(get_temperature()), 0, 1);
    display("Light: " + String(get_light_level()), 0, 1);
    toggle_led(LED_PIN_RED, LED_ON);
}
