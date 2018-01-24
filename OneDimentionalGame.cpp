#include "OneDimentionalGame.h"

OneDimentionalGame::OneDimentionalGame() {
	init();
}

void OneDimentionalGame::init() {
	#if defined (__AVR_ATtiny85__)
		if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
	#endif

	strip.setBrightness(BRIGHTNESS);
	strip.begin();
	strip.show();
}

void OneDimentionalGame::run() {

	move_player();

	refreshScreen();
}

void OneDimentionalGame::refreshScreen() {
	blink_player();

	cast_field();
	upload_colors();
}

void OneDimentionalGame::blink_player() {
	if ((blink_time + blink_speed) < millis()) {
		blink_time = millis();
		player_type = (player_type == 'P') ? 'p' : 'P';
	}
}

void OneDimentionalGame::cast_field() {
	for (int i = 0; i < NUM_LEDS; i++) {
		switch (field[i]) {
		case 'L': colored[i] = RED; break;
		case 'l': colored[i] = GREEN; break;
		case 'Y': colored[i] = YELLOW; break;
		case 'P': colored[i] = PINK; break;
		case 'M': colored[i] = MAGENTA; break;
		case 'E': colored[i] = BLUE; break;
		default: colored[i] = OFF; break;
		}
		if (player_type == 'P')
			colored[player_index] = WHITE;
		else
			colored[player_index] = PLAYER;
	}
}

void OneDimentionalGame::upload_colors() {
	for (int i = 0; i < NUM_LEDS; i++) {
		uint32_t color = colored[i];
		strip.setPixelColor(i, color);
	}
	strip.show();
}


void OneDimentionalGame::position_player(char direction) {
	switch (direction)
	{
	case 'U':
		if (player_index < NUM_LEDS)
			player_index++;
		break;
	case 'D':
		if (player_index > 0)
			player_index--;
		break;
	default:
		break;
	}
}

void OneDimentionalGame::move_player() {
	if (Serial2.available()) {
		char direction = Serial2.read();
		Serial.write(direction);

		position_player(direction);
	}

	if (Serial.available()) {
		Serial2.write(Serial.read());
	}
}