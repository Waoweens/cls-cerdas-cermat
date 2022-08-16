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

const char *ssid = "CLS";
const char *password = "cls.sch.id";

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

char *type;
int plus;
bool negative;
int minus;

const char *html_index PROGMEM = R"+(
<!DOCTYPE html>
<html lang="en">

<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
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
			justify-content: center;
			align-items: center;

		}

		.fc-space {
			margin-top: 25px;
			gap: 10px;
		}

		.grid-container {
			display: grid;
			grid-template-columns: repeat(auto-fit, auto);
		}

		.grid-content {
			display: flex;
			flex-direction: column;
			align-items: center;
			justify-content: center;
			text-align: center;
			border: 10px solid rgba(255, 255, 255, 0)
		}

		.current-player {
			border: 10px solid rgba(0, 0, 255, 0.8) !important;
		}

		.center {
			text-align: center;
		}

		.score {
			font-size: 8em;
			font-weight: bold;
		}

		.group-name {
			font-size: 2em;
			font-weight: bold;
		}

		.btn-correct {
			background-color: #4CAF50;
			/* Green */
			border: none;
			color: white;
			padding: 15px 32px;
			text-align: center;
			text-decoration: none;
			display: inline-block;
			font-size: 16px;
		}

		.btn-incorrect {
			background-color: #c04444;
			/* Green */
			border: none;
			color: white;
			padding: 15px 32px;
			text-align: center;
			text-decoration: none;
			display: inline-block;
			font-size: 16px;
		}
	</style>
	<script>
		var gateway = `ws://${window.location.hostname}/ws`;
		var websocket;
		var scores;
		var currentPlayer;
		function initWebSocket() {
			console.log('Trying to open a WebSocket connection...');
			websocket = new WebSocket(gateway);
			websocket.onopen = onOpen;
			websocket.onclose = onClose;
			websocket.onmessage = onMessage; // <-- add this line
		}
		function onOpen(event) {
			console.log('Connection opened');
		}

		function onClose(event) {
			console.log('Connection closed');
			setTimeout(initWebSocket, 2000);
		}

		window.addEventListener('load', onLoad);

		function onLoad(event) {
			initWebSocket();
		}

		var scorePlusVal = 10;
		var scoreNegativeVal = false;
		var scoreMinusVal = 10;

		document.addEventListener("DOMContentLoaded", () => {
			var scorePlus = document.getElementById('scorePlus');
			var scoreNegative = document.getElementById('scoreNegative');
			var scoreMinus = document.getElementById('scoreMinus');

			scorePlus.addEventListener('input', (e) => {
				scorePlusVal = e.target.value;
				console.log(scorePlusVal);
			});
			scoreNegative.addEventListener('change', (e) => {
				scoreNegativeVal = e.target.checked;
				console.log(scoreNegativeVal);
			});

			scoreMinus.addEventListener('input', (e) => {
				scoreMinusVal = e.target.value;
				console.log(scoreMinusVal);
			});
		});



		function answer(type) {
			websocket.send(`${type},${scorePlusVal},${Number(scoreNegativeVal)},${scoreMinusVal}`);
			// websocket.send(type)
		}

		function onMessage(event) {
			scores = JSON.parse(event.data);
			currentPlayer = scores.currentPlayer;
			console.log(scores);

			if (currentPlayer == 0) {
				var elScores = document.getElementsByClassName('score');
				for (let item of elScores) {
					item.classList.remove('current-player');
				}
			} else {
				document.getElementById(`p${currentPlayer}-score`).classList.add('current-player');
			}

			document.getElementById('p1-score').innerHTML = scores.scores[0];
			document.getElementById('p2-score').innerHTML = scores.scores[1];
			document.getElementById('p3-score').innerHTML = scores.scores[2];
			document.getElementById('p4-score').innerHTML = scores.scores[3];
		}
	</script>
</head>

<body>
	<h1 class="center">Cerdas Cermat</h1>
	<div class="grid-container">
		<div class="grid-content">
			<div class="grid-content group-name">Group 1</div>
			<div id="p1-score" class="grid-content score">0</div>
		</div>
		<div class="grid-content">
			<div class="grid-content group-name">Group 2</div>
			<div id="p2-score" class="grid-content score">0</div>
		</div>
		<div class="grid-content">
			<div class="grid-content group-name">Group 3</div>
			<div id="p3-score" class="grid-content score">0</div>
		</div class="grid-content">
		<div class="grid-content">
			<div class="grid-content group-name">Group 4</div>
			<div id="p4-score" class="grid-content score">0</div>
		</div>
	</div>


	<div class="flex-container fc-space">
		<button onclick="answer('correct')" class="btn-correct">Correct</button>
		<button onclick="answer('incorrect')" class="btn-incorrect">Incorrect</button>
	</div>

	<div class="flex-container fc-space">
		<label for="scorePlus">Increase score by:</label>
		<input type="text" id="scorePlus" name="scorePlus" value="10">
	</div>

	<div class="flex-container fc-space">
		<input type="checkbox" id="scoreNegative" name="scoreNegative">
		<label for="scoreNegative">Decrease score on incorrect</label>
	</div>

	<div class="flex-container" style="margin-top: 5px;gap: 10px;">
		<label for="scoreMinus">Decrease score by:</label>
		<input type="text" id="scoreMinus" name="scoreMinus" value="10">
	</div>

</body>

</html>
)+";

void notifyClients() {
	ws.textAll("{\"currentPlayer\":" + String(currentPlayer) + ",\"scores\":[" +
			   String(p1_score) + ", " + String(p2_score) + ", " +
			   String(p3_score) + ", " + String(p4_score) + "]}");
}

void nextRound() {
	currentPlayer = 0;
	digitalWrite(p1_lamp, LOW);
	digitalWrite(p2_lamp, LOW);
	digitalWrite(p3_lamp, LOW);
	digitalWrite(p4_lamp, LOW);
	notifyClients();
}
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
	AwsFrameInfo *info = (AwsFrameInfo *)arg;
	if (info->final && info->index == 0 && info->len == len &&
		info->opcode == WS_TEXT) {
		data[len] = 0;

		// if (strcmp((char *)data, "correct") == 0) {
		// 	if (currentPlayer == 1) p1_score = p1_score + 10;
		// 	if (currentPlayer == 2) p2_score = p2_score + 10;
		// 	if (currentPlayer == 3) p3_score = p3_score + 10;
		// 	if (currentPlayer == 4) p4_score = p4_score + 10;
		// 	nextRound();
		// }
		// if (strcmp((char *)data, "incorrect") == 0) {
		// 	nextRound();
		// }

		Serial.println((char *)data);

		type = strtok((char *)data, ",");
		plus = atoi(strtok(NULL, ","));
		negative = (bool)atoi(strtok(NULL, ","));
		minus = atoi(strtok(NULL, ","));
		Serial.print("type: ");
		Serial.println(type);
		Serial.print("plus: ");
		Serial.println(plus);
		Serial.print("negative: ");
		Serial.println(negative);
		Serial.print("minus: ");
		Serial.println(minus);

		if (strcmp(type, "correct") == 0) {
			if (currentPlayer == 1) p1_score = p1_score + plus;
			if (currentPlayer == 2) p2_score = p2_score + plus;
			if (currentPlayer == 3) p3_score = p3_score + plus;
			if (currentPlayer == 4) p4_score = p4_score + plus;
			nextRound();
		}

		if (strcmp(type, "incorrect") == 0) {
			if (negative == 1) {
				if (currentPlayer == 1) p1_score = p1_score - minus;
				if (currentPlayer == 2) p2_score = p2_score - minus;
				if (currentPlayer == 3) p3_score = p3_score - minus;
				if (currentPlayer == 4) p4_score = p4_score - minus;
			}
			nextRound();
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
			Serial.printf("WebSocket data from #%u (%u bytes): %s\n",
						  client->id(), len, (char *)data);
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
		delay(500);
	}

	Serial.print(">");
	Serial.print("IPADDR");
	Serial.print(WiFi.localIP());
	Serial.println("<");

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
