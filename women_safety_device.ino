#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

// ─── Pin Definitions ─────────────────────────────────────────────────────────
int btn = 6;   // Enroll button (hold at startup to enter enroll mode)
int buz = 5;   // Buzzer

// LCD: RS=8, E=9, D4=10, D5=11, D6=12, D7=13
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

// Fingerprint sensor on Software Serial pins 2 (RX), 3 (TX)
SoftwareSerial mySerial(2, 3);

// GPS
TinyGPS gps;
float flat = 0, flon = 0;

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t operation;
uint8_t id;
int iij = 0;

// ─── GPS Reading ─────────────────────────────────────────────────────────────
void read_gps() {
    bool newData = false;
    unsigned long chars;
    unsigned short sentences, failed;

    for (unsigned long start = millis(); millis() - start < 1000;) {
        while (Serial.available()) {
            char c = Serial.read();
            if (gps.encode(c))
                newData = true;
        }
    }

    if (newData) {
        unsigned long age;
        gps.f_get_position(&flat, &flon, &age);
    }
}

// ─── Fingerprint Sensor Init ─────────────────────────────────────────────────
void detectFingerprintScanner() {
    if (finger.verifyPassword()) {
        delay(10);
    } else {
        delay(10);
        while (1) { delay(1); }
    }
}

void verifyScannerParameters() {
    delay(10);
}

// ─── Setup ───────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);

    pinMode(btn, INPUT_PULLUP);
    pinMode(buz, OUTPUT);

    lcd.print("   WELCOME");

    while (!Serial);
    delay(100);

    finger.begin(57600);
    detectFingerprintScanner();
    verifyScannerParameters();

    // Hold enroll button at startup to register a new fingerprint
    if (digitalRead(btn) == 0) {
        delay(3000);
        enrollFingerprint();
    }
}

// ─── Fingerprint Enrollment ───────────────────────────────────────────────────
uint8_t getFingerprintEnroll() {
    // First scan
    int p = -1;
    while (p != FINGERPRINT_OK) {
        p = finger.getImage();
    }

    p = finger.image2Tz(1);
    if (p != FINGERPRINT_OK) return p;

    delay(2000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER) {
        p = finger.getImage();
    }

    p = -1;
    // Second scan
    while (p != FINGERPRINT_OK) {
        p = finger.getImage();
    }

    p = finger.image2Tz(2);
    if (p != FINGERPRINT_OK) return p;

    // Create and store model
    p = finger.createModel();
    if (p != FINGERPRINT_OK) return p;

    p = finger.storeModel(id);
    return (p == FINGERPRINT_OK) ? true : p;
}

void enrollFingerprint() {
    while (iij == 0) {
        lcd.clear();
        lcd.print("ENROL..");
        id = 1;
        if (id == 0) {
            iij = 1;
            break;
        }
        while (!getFingerprintEnroll());
    }
}

// ─── SMS Alert via GSM ───────────────────────────────────────────────────────
// NOTE: GSM module must be connected to Arduino serial (shared with GPS here).
// In hardware, use a second SoftwareSerial for GSM, or use hardware Serial1 on Mega.
// Emergency contact numbers are hardcoded below — update before deploying.

void send_sms() {
    read_gps();
    digitalWrite(buz, 1);

    String location = "https://www.google.com/maps/search/?api=1&query="
                      + String(flat, 6) + "," + String(flon, 6);
    String message  = "Alert: Person is in danger at Loc:\n" + location;

    // ── Send to Contact 1 ──
    Serial.println("AT");           delay(1000);
    Serial.println("ATE0");         delay(1000);
    Serial.println("AT+CMGF=1");    delay(1000);
    Serial.print("AT+CMGS=\"9949592453\"\r\n");  // ← Replace with emergency contact 1
    delay(1000);
    Serial.println(message);
    Serial.println((char)26);       // Ctrl+Z to send
    delay(5000);

    // ── Send to Contact 2 ──
    Serial.println("AT");           delay(1000);
    Serial.println("ATE0");         delay(1000);
    Serial.println("AT+CMGF=1");    delay(1000);
    Serial.print("AT+CMGS=\"8978137886\"\r\n");  // ← Replace with emergency contact 2
    delay(1000);
    Serial.println(message);
    Serial.println((char)26);
    delay(10000);

    // ── Voice call ──
    Serial.println("ATD8978137886;");   // ← Replace with number to call

    delay(2000);
    digitalWrite(buz, 0);
}

// ─── Main Loop ───────────────────────────────────────────────────────────────
void loop() {
    read_gps();

    lcd.setCursor(0, 0);
    lcd.print("                 ");
    lcd.setCursor(0, 0);
    lcd.print("SCAN THUMB..");

    lcd.setCursor(0, 1);
    lcd.print("                 ");
    lcd.setCursor(0, 1);
    lcd.print(String(flat, 4) + " " + String(flon, 4));

    uint8_t p = finger.getImage();

    if (p == FINGERPRINT_NOFINGER) {
        delay(10);
    } else if (p == FINGERPRINT_OK) {
        p = finger.image2Tz();

        if (p != FINGERPRINT_OK) {
            // Image conversion failed → unauthorized
            lcd.setCursor(0, 0);
            lcd.print("                 ");
            lcd.setCursor(0, 0);
            lcd.print("UNAUTHORIZED..");
            digitalWrite(buz, 1);
            delay(1000);
            digitalWrite(buz, 0);
            return;
        }

        p = finger.fingerFastSearch();

        if (p != FINGERPRINT_OK) {
            // No match found → unauthorized
            lcd.setCursor(0, 0);
            lcd.print("                 ");
            lcd.setCursor(0, 0);
            lcd.print("UNAUTHORIZED..");
            digitalWrite(buz, 1);
            delay(1000);
            digitalWrite(buz, 0);
        } else {
            // Authorized fingerprint → send SOS alert
            lcd.setCursor(0, 0);
            lcd.print("                 ");
            lcd.setCursor(0, 0);
            lcd.print("Sending Alert:" + String(finger.fingerID));
            digitalWrite(buz, 1);
            send_sms();
        }

        delay(1500);
    }
}
