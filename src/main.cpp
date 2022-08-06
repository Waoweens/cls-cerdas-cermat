#include "Arduino.h"

const int p1_btn = 9;
const int p2_btn = 10;
const int p3_btn = 11;

const int p1_lamp = 6;
const int p2_lamp = 7;
const int p3_lamp = 8;

const int bell = 5;
const int reset = 4;

void setup() {
	pinMode(p1_btn, INPUT_PULLUP);
	pinMode(p2_btn, INPUT_PULLUP);
	pinMode(p3_btn, INPUT_PULLUP);

	pinMode(p1_lamp, OUTPUT);
	pinMode(p2_lamp, OUTPUT);
	pinMode(p3_lamp, OUTPUT);

	pinMode(bell, OUTPUT);
	pinMode(reset, INPUT_PULLUP);

	Serial.begin(9600);
}

int currentPlayer = 0;
void loop() {
	if (digitalRead(reset) == LOW) {
		currentPlayer = 0;
		digitalWrite(p1_lamp, LOW);
		digitalWrite(p2_lamp, LOW);
		digitalWrite(p3_lamp, LOW);
	}
	if (digitalRead(p1_btn) == LOW) {
		if (currentPlayer != 0 && currentPlayer != 1) {
			return;
		}
		currentPlayer = 1;
		digitalWrite(p1_lamp, HIGH);
		tone(bell, 2000, 1000);
	}
	if (digitalRead(p2_btn) == LOW) {
		if (currentPlayer != 0 && currentPlayer != 2) {
			return;
		}
		currentPlayer = 2;
		digitalWrite(p2_lamp, HIGH);
		tone(bell, 2000, 1000);
	}
	if (digitalRead(p3_btn) == LOW) {
		if (currentPlayer != 0 && currentPlayer != 3) {
			return;
		}
		currentPlayer = 3;
		digitalWrite(p3_lamp, HIGH);
		tone(bell, 2000, 1000);
	}
	Serial.println(currentPlayer);
}
