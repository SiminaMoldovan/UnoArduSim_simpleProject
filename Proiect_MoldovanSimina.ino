#define NOTE_D4 294
#define NOTE_C4 262
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784

#include <SPI.h>

const int buzzerPin = 6; // Pinul conectat la buzzer
const int motorPin = 5;  // Pinul conectat la motor
const int ledPins[] = {8, 12, 10, 11}; // LED-uri conectate la pini
const int sliderPin = A0;              // Slider conectat la pinul analogic A0
const int Tbit_us = 1000000 / 100;     // Timpul pentru impulsurile de ceas

#define BUTON1 2  // Butonul pentru afisarea mesajului initial
#define BUTON2 9  // Butonul pentru redarea melodiei Jazz Happy Birthday
#define BUTON3 1  // Butonul pentru redarea melodiei Original Happy Birthday

#define SID 11    // Pinul de date pentru comunicarea SPI
#define SS_PIN 4  // Select Slave pin pentru comunicarea SPI
#define SCK_PIN 13 // Pinul de ceas pentru comunicarea SPI
#define RS_PIN 7   // Pinul RS pentru controlul LCD-ului

// Melodia Happy Birthday
int melody1[] = {
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,
  NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
};

int durations1[] = {
  4, 4, 4, 4, 2, 2,
  4, 4, 4, 4, 2, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 2, 2
};

// Melodia Jazz Happy Birthday
int melody2[] = {
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_C5, NOTE_B4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_D5, NOTE_C5,
  NOTE_G4, NOTE_G4, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_F5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_C5
};

int durations2[] = {
  4, 4, 4, 4, 2, 2,
  4, 4, 4, 4, 2, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 2, 2
};

void setup() {
	pinMode(BUTON3, INPUT); // Configurare pin pentru butonul 3
	pinMode(BUTON1, INPUT); // Configurare pin pentru butonul 1
	pinMode(BUTON2, INPUT); // Configurare pin pentru butonul 2
	pinMode(buzzerPin, OUTPUT); // Configurare pin pentru buzzer
	pinMode(motorPin, OUTPUT);  // Configurare pin pentru motor

	for (int i = 0; i < 4; i++) {
		pinMode(ledPins[i], OUTPUT); // Configurare pini pentru LED-uri
		digitalWrite(ledPins[i], LOW); // Stinge toate LED-urile la început
	}

	pinMode(SID, OUTPUT); // Configurare pin pentru date SPI
	pinMode(SCK_PIN, OUTPUT); // Configurare pin pentru ceas SPI
	pinMode(SS_PIN, OUTPUT); // Configurare pin pentru selectarea slave-ului SPI
	pinMode(RS_PIN, OUTPUT); // Configurare pin RS pentru LCD
	pinMode(5, OUTPUT);      // Pin auxiliar
}

void loop() {
	if (digitalRead(BUTON1) == LOW) {
		delay(10); // Debouncing
		if (digitalRead(BUTON1) == LOW) {
			displayMessage("");
			displayMessage("Happy Birthday!     Choose your song:");
			delay(2000); // Pauza pentru afi?area mesajului
		}
	}

	if (digitalRead(BUTON2) == LOW) {
		delay(10); // Debouncing
		if (digitalRead(BUTON2) == LOW) {
			displayMessage("");
			displayMessage("1.Jazz Happy Birthday");
			playMelody(melody2, durations2, sizeof(durations2) / sizeof(int));
		}
	}

	if (digitalRead(BUTON3) == LOW) {
		delay(10); // Debouncing
		if (digitalRead(BUTON3) == LOW) {
			displayMessage("");
			displayMessage("2. Original Happy Birthday");
			playMelody(melody1, durations1, sizeof(durations1) / sizeof(int));
		}
	}
}

void playMelody(int melody[], int durations[], int size) {
	// Cite?te valoarea sliderului
	float sliderValue = analogRead(sliderPin);
	float speedFactor = sliderValue / 1023.0; // Normalizare între 0 ?i 1
	float tempo = 1.0 + (1.0 - speedFactor) * 2.0; // Tempo variaza între 1 ?i 3

	digitalWrite(motorPin, HIGH); // Activeaza motorul
	for (int note = 0; note < size; note++) {
		int duration = 1000 / durations[note]; // Calculeaza durata notei
		int adjustedDuration = duration / tempo; // Ajustam durata notei în func?ie de slider
		tone(buzzerPin, melody[note], adjustedDuration); // Reda nota la buzzer
		controlLEDs(note); // Controleaza LED-urile
		delay(adjustedDuration * 1.5); // Pauza între note
		noTone(buzzerPin); // Opre?te nota curenta
	}
	digitalWrite(motorPin, LOW); // Dezactiveaza motorul
	for (int i = 0; i < 4; i++) { // Stinge toate LED-urile la final
		digitalWrite(ledPins[i], LOW);
	}
}

void controlLEDs(int step) {
	for (int i = 0; i < 4; i++) {
		digitalWrite(ledPins[i], (i == step % 4) ? HIGH : LOW); // Aprinde pe rând fiecare LED
	}
}

void displayMessage(const char* message) {
	digitalWrite(SS_PIN, LOW); // Selecteaza slave-ul
	linie_in_repaus(); // Ini?ializeaza linia SPI
	LCD_init(); // Ini?ializeaza LCD-ul

	SPI_wr_byte(0b00000001); // Clear display
	delay(2); // Pauza pentru ini?ializare

	for (int i = 0; message[i] != '\0'; ++i) {
		SPI_wr_byte(message[i]); // Trimite fiecare caracter catre LCD
	}

	linie_in_repaus();
	digitalWrite(SS_PIN, HIGH); // Deselecteaza slave-ul
}

void linie_in_repaus() {
	digitalWrite(SID, HIGH); // Seteaza linia de date HIGH
	digitalWrite(SCK_PIN, LOW); // Seteaza linia de ceas LOW
}

void pulsCeas() {
	delayMicroseconds(Tbit_us / 2); // Jumatate din perioada bitului
	digitalWrite(SCK_PIN, HIGH); // Impuls HIGH pe linia de ceas
	delayMicroseconds(Tbit_us / 2); // Jumatate din perioada bitului
	digitalWrite(SCK_PIN, LOW); // Impuls LOW pe linia de ceas
}

void LCD_init() {
	delay(15); // A?teapta pentru ini?ializarea LCD-ului
	digitalWrite(RS_PIN, LOW); // Selecteaza modul de comanda
	SPI_wr_byte(0x30); // Comanda de resetare
	delay(5); // Pauza pentru stabilizare
	for (int i = 0; i < 2; i++) {
		SPI_wr_byte(0b00111000); // Seteaza modul de func?ionare al LCD-ului
		delayMicroseconds(160); // Pauza scurta
	}
	SPI_wr_byte(0x10); // Seteaza cursorul la pozi?ia ini?iala
	delayMicroseconds(40);
	SPI_wr_byte(0b00000001); // Clear display
	delay(2);
	SPI_wr_byte(0b00000110); // Seteaza modul de mi?care al cursorului
	delayMicroseconds(40);
	SPI_wr_byte(0b00001111); // Activeaza afi?ajul ?i cursorul
	delayMicroseconds(40);
	SPI_wr_byte(0x80); // Seteaza cursorul la începutul primei linii
	delayMicroseconds(40);
	digitalWrite(RS_PIN, HIGH); // Selecteaza modul de date
	delayMicroseconds(40);
}

void SPI_wr_byte(char x) {
	int bitActual;
	for (int j = 7; j >= 0; j--) { // Itereaza prin fiecare bit al octetului
		bitActual = bitRead(x, j); // Cite?te bitul curent
		if (bitActual == 0)
			digitalWrite(SID, LOW); // Seteaza linia de date LOW
		else
			digitalWrite(SID, HIGH); // Seteaza linia de date HIGH
		pulsCeas(); // Genereaza un impuls pe linia de ceas
	}
}
