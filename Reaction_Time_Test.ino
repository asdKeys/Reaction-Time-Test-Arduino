#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int green_led = 6;
const int yellow_led = 7;
const int red_led = 8;
const int benchmarker_pin = 10;

int button_state;
String state;
String ready;
String display;
long delayAmount;
long delayTime;
unsigned long startTime;
unsigned long elapsed;

void setup() {
  lcd.begin(16, 2);
  pinMode(green_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(benchmarker_pin, INPUT);
  lcd.clear();
  lcd.setCursor(0, 0);
  digitalWrite(green_led, LOW);
  digitalWrite(yellow_led, LOW);
  digitalWrite(red_led, HIGH);
  state = "off";
  button_state = LOW;
  ready = "off";
  display = "no";
  delayAmount = 0;
  delayTime = 0;
  startTime = 0;
  elapsed = 0;
  randomSeed(analogRead(A0));
}

void loop() {
  state_off();
  state_on();
  start_test();
  reaction_time();
}

void state_off() {
  if (state == "off") {
    lcd.setCursor(0, 0);
    lcd.print("Press Button To");
    lcd.setCursor(0, 1);
    lcd.print("Begin");
    digitalWrite(green_led, LOW);
    digitalWrite(yellow_led, LOW);
    digitalWrite(red_led, HIGH);
    button_state = digitalRead(benchmarker_pin);
    if (button_state == HIGH) {
      while (digitalRead(benchmarker_pin) == HIGH) {
        delay(5);
      }
      state = "on";
      ready = "no";
      display = "no";
    }
    delay(5);
  }
}

void state_on() {
  if (state == "on") {
    if (display == "no") {
      lcd.clear();
    }
    if (ready == "yes") {
      digitalWrite(green_led, HIGH);
      digitalWrite(yellow_led, LOW);
    }
    else {
      digitalWrite(green_led, LOW);
      digitalWrite(yellow_led, HIGH);
    }
    digitalWrite(red_led, LOW);
    delay(5);
  }
}

void start_test() {
  if (ready == "no" && state == "on") {
    startTime = millis();
    delayAmount = random(2, 10);
    delayAmount = delayAmount * 1000;
    delayTime = 0;
    while (delayAmount > delayTime && state == "on") {
      button_state = digitalRead(benchmarker_pin);
      if (button_state == HIGH) {
        while (digitalRead(benchmarker_pin) == HIGH) {
          delay(5);
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Too Early!");
        digitalWrite(green_led, LOW);
        digitalWrite(yellow_led, LOW);
        digitalWrite(red_led, HIGH);
        while (digitalRead(benchmarker_pin) == LOW) {
          delay(5);
        }
        while (digitalRead(benchmarker_pin) == HIGH) {
          delay(5);
        }
        state = "off";
        ready = "off";
        display = "no";
        button_state = LOW;
        lcd.clear();
        return;
      }
      delayTime = delayTime + 5;
      delay(5);
    }
    if (state == "on") {
      ready = "yes";
    }
    delay(5);
  }
}

void reaction_time() {
  if (ready == "yes") {
    button_state = digitalRead(benchmarker_pin);
    if (button_state == HIGH && display == "no") {
      elapsed = millis() - startTime;
      while (digitalRead(benchmarker_pin) == HIGH) {
        delay(5);
      }
      display = "yes";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(elapsed % 1000);
      lcd.print("ms");
    }
    if (display == "yes") {
      button_state = digitalRead(benchmarker_pin);
      if (button_state == HIGH) {
        while (digitalRead(benchmarker_pin) == HIGH) {
          delay(5);
        }
        display = "no";
        state = "off";
        button_state = LOW;
        ready = "off";
        lcd.clear();
      }
    }
    delay(5);
  }
}