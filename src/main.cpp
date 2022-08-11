/**
 * @file main.cpp
 * @author Cendekia Leadership School (info@cls.sch.id)
 * @version 1.0
 * @date 2022-08-06
 * @copyright Copyright (c) 2022 Cendekia Leadership School. MIT License
 *
 */
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "Arduino.h"
// #include <LiquidCrystal_I2C.h>
#include <ESPAsyncTCP.h>
#include <Wire.h>

// LiquidCrystal_I2C lcd(0x3F, 16, 9);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char* ssid = "KooshaWi-Fi1 2.4GHz";
const char* password = "theawiligar144";

const int p1_btn = 0;
const int p2_btn = 2;
const int p3_btn = 14;
const int p4_btn = 12;

const int p1_lamp = 16;
const int p2_lamp = 15;
const int p3_lamp = 5;
const int p4_lamp = 4;

const int bell = 13;

int currentPlayer = 0;
int p1_score = 0;
int p2_score = 0;
int p3_score = 0;
int p4_score = 0;

const char *html_index = R"+(
<!DOCTYPE html>
<html lang="en">
	<head>
		<link rel="preconnect" href="https://fonts.googleapis.com">
		<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
		<link href="https://fonts.googleapis.com/css2?family=Roboto&display=swap" rel="stylesheet">
		<title>Cerdas Cermat</title>

		<style>
			* {
				font-family: 'Roboto', sans-serif;
			}

			.flex-container {
				display: flex;
			}
		</style>
		<script defer>

		</script>
	</head>
	<body>
		<center><h1>Cerdas Cermat</h1></center>
		<div class="flex-container">
			<span>Player 1</span>
		</div>
	</body>
</html>
	)+";



void notifyClients() {
	ws.textAll("{\"currentPlayer\":" + String(currentPlayer) + ",\"scores\":[" +
			   String(p1_score) + ", " + String(p2_score) + ", " +
			   String(p3_score) + ", " + String(p4_score) + "]}");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
	AwsFrameInfo *info = (AwsFrameInfo *)arg;
	if (info->final && info->index == 0 && info->len == len &&
		info->opcode == WS_TEXT) {
		data[len] = 0;
		if (strcmp((char *)data, "correct") == 0) {
			notifyClients();
		}
		if (strcmp((char *)data, "incorrect") == 0) {
			notifyClients();
		}
	}
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
			 AwsEventType type, void *arg, uint8_t *data, size_t len) {
	switch (type) {
		case WS_EVT_CONNECT:
			Serial.printf("WebSocket client #%u connected from %s\n",
						  client->id(), client->remoteIP().toString().c_str());
			notifyClients();
			break;
		case WS_EVT_DISCONNECT:
			Serial.printf("WebSocket client #%u disconnected\n", client->id());
			break;
		case WS_EVT_DATA:
			handleWebSocketMessage(arg, data, len);
			break;
		case WS_EVT_PONG:
		case WS_EVT_ERROR:
			break;
	}
}

void initWebSocket() {
	ws.onEvent(onEvent);
	server.addHandler(&ws);
}

void nextRound() {
	currentPlayer = 0;
	digitalWrite(p1_lamp, LOW);
	digitalWrite(p2_lamp, LOW);
	digitalWrite(p3_lamp, LOW);
	digitalWrite(p4_lamp, LOW);
	notifyClients();
}

void setup() {
	pinMode(p1_btn, INPUT_PULLUP);
	pinMode(p2_btn, INPUT_PULLUP);
	pinMode(p3_btn, INPUT_PULLUP);
	pinMode(p4_btn, INPUT_PULLUP);
	pinMode(p1_lamp, OUTPUT);
	pinMode(p2_lamp, OUTPUT);
	pinMode(p3_lamp, OUTPUT);
	pinMode(p4_lamp, OUTPUT);
	pinMode(bell, OUTPUT);

	Serial.begin(115200);

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	Serial.print(WiFi.localIP());

	// lcd.init();
	// lcd.backlight();
	// lcd.setCursor(0, 0);
	// lcd.print(WiFi.localIP());

	initWebSocket();
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send_P(200, "text/html", html_index);
	});

	server.onNotFound([](AsyncWebServerRequest *request) {
		request->send_P(404, "text/plain", "Not found");
	});

	server.begin();
	nextRound();
}

void loop() {
	ws.cleanupClients();

	if (digitalRead(p1_btn) == LOW) {
		if (currentPlayer != 0 && currentPlayer != 1) return;

		currentPlayer = 1;
		digitalWrite(p1_lamp, HIGH);
		tone(bell, 1000, 500);
		notifyClients();
	}

	if (digitalRead(p2_btn) == LOW) {
		if (currentPlayer != 0 && currentPlayer != 2) return;
		currentPlayer = 2;
		digitalWrite(p2_lamp, HIGH);
		tone(bell, 2000, 500);
		notifyClients();
	}

	if (digitalRead(p3_btn) == LOW) {
		if (currentPlayer != 0 && currentPlayer != 3) return;
		currentPlayer = 3;
		digitalWrite(p3_lamp, HIGH);
		tone(bell, 3000, 500);
		notifyClients();
	}
	if (digitalRead(p4_btn) == LOW) {
		if (currentPlayer != 0 && currentPlayer != 4) return;
		currentPlayer = 4;
		digitalWrite(p4_lamp, HIGH);
		tone(bell, 4000, 500);
		notifyClients();
	}
}
