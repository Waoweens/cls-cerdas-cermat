/**
 * @file main.cpp
 * @author Cendekia Leadership School (info@cls.sch.id)
 * @version 1.0
 * @date 2022-08-06
 * @copyright Copyright (c) 2022 Cendekia Leadership School. MIT License
 * 
 */

#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 9);

const int p1_btn = 10;
const int p2_btn = 11;
const int p3_btn = 12;
const int p1_lamp = 7;
const int p2_lamp = 8;
const int p3_lamp = 9;
const int bell = 6;
const int correct = 5;
const int incorrect = 4;

int currentPlayer = 0;
int p1_score = 0;
int p2_score = 0;
int p3_score = 0;
int correct_lastState = 0;
int incorrect_lastState = 0;

void setup() {
	pinMode(p1_btn, INPUT_PULLUP);
	pinMode(p2_btn, INPUT_PULLUP);
	pinMode(p3_btn, INPUT_PULLUP);
	pinMode(p1_lamp, OUTPUT);
	pinMode(p2_lamp, OUTPUT);
	pinMode(p3_lamp, OUTPUT);
	pinMode(bell, OUTPUT);
	pinMode(correct, INPUT_PULLUP);
	pinMode(incorrect, INPUT_PULLUP);

	lcd.init();
	lcd.backlight();
	lcd.setCursor(0, 0);
	lcd.print("  P1   P2   P3  ");
	lcd.setCursor(0, 1);
	lcd.print("  00   00   00  ");
}

void nextRound() {
	// reset current player indicator
	currentPlayer = 0;

	// turn off all lamps
	digitalWrite(p1_lamp, LOW);
	digitalWrite(p2_lamp, LOW);
	digitalWrite(p3_lamp, LOW);

	// update score count
	lcd.setCursor(0, 1);
	lcd.print("                ");
	lcd.setCursor(2, 1);
	lcd.print((p1_score < 10 && p1_score >= 0) ? "0" + String(p1_score)
											  : p1_score);
	lcd.setCursor(7, 1);
	lcd.print((p2_score < 10 && p2_score >= 0) ? "0" + String(p2_score)
											  : p2_score);
	lcd.setCursor(12, 1);
	lcd.print((p3_score < 10 && p3_score >= 0) ? "0" + String(p3_score)
											  : p3_score);
}

void loop() {
	if (digitalRead(p1_btn) == LOW) {
		if (currentPlayer != 0 && currentPlayer != 1) {
			return;
		}
		currentPlayer = 1;
		digitalWrite(p1_lamp, HIGH);
		tone(bell, 1000, 500);
	}

	if (digitalRead(p2_btn) == LOW) {
		if (currentPlayer != 0 && currentPlayer != 2) {
			return;
		}
		currentPlayer = 2;
		digitalWrite(p2_lamp, HIGH);
		tone(bell, 2000, 500);
	}

	if (digitalRead(p3_btn) == LOW) {
		if (currentPlayer != 0 && currentPlayer != 3) {
			return;
		}
		currentPlayer = 3;
		digitalWrite(p3_lamp, HIGH);
		tone(bell, 3000, 500);
	}

	if (digitalRead(correct) != correct_lastState) {
		if (digitalRead(correct) == LOW) {
			if (currentPlayer == 1) {
				p1_score++;
			} else if (currentPlayer == 2) {
				p2_score++;
			} else if (currentPlayer == 3) {
				p3_score++;
			}
			nextRound();
		}

	}
	correct_lastState = digitalRead(correct);

	if (digitalRead(incorrect) != incorrect_lastState) {
		if (digitalRead(incorrect) == LOW) {
			// if (currentPlayer == 1) {
			// 	p1_score--;
			// } else if (currentPlayer == 2) {
			// 	p2_score--;
			// } else if (currentPlayer == 3) {
			// 	p3_score--;
			// }
			nextRound();
		}
	}
	incorrect_lastState = digitalRead(incorrect);
}
