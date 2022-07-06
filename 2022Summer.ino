/* Fire Detecting System implementation source */

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

#define LCD_CHARS 16

// 측정하고 실제값 입력할 것
#define TEMP_LIMIT 0
#define LIGHT_LIMIT 0

LiquidCrystal_I2C lcd(0x3f, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);

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
void display(const String& msg, int line, int pos) {  // print to display
    clearLine(line);
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

//LED module test
void led_init_test() {
    Serial.println("Testing LED module");

    Serial.println("255 -> 0");
    for (int val = 255; val > 0; val--) {
        analogWrite(LED_PIN_RED, val);
        analogWrite(LED_PIN_BLUE, 255 - val);
        analogWrite(LED_PIN_GREEN, 128 - val);
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

void toggle_led(uint8_t color) {
    if (analogRead(color) == LED_OFF)
        analogWrite(color, LED_ON);
    else
        analogWrite(color, LED_OFF);
}

void led_beep() {
    if (analogRead(LED_PIN_RED) == LED_ON) analogWrite(LED_PIN_RED, LED_OFF);
    toggle_led(LED_PIN_RED);
    delay(250);
    toggle_led(LED_PIN_RED);
    delay(250);
}

bool detect_temp_warning() {
    if (get_temperature() > TEMP_LIMIT) return true;
    return false;
}

bool detect_light_warning() {
    if (get_light_level() < LIGHT_LIMIT) return true;
    return false;
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
}

// Add the main program code into the continuous loop() function
void loop() {
    display("Temp: " + String(get_temperature()), 0, 0);
    display("Light: " + String(get_light_level()), 1, 0);

    if (detect_light_warning() && detect_temp_warning()) {
        clearLine(0);
        clearLine(1);
        display("Fire!", 0, 0);
        led_beep();
    }
}
