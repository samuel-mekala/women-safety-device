# 🆘 Women Safety Device with GPS Tracking & Alerts

> **ECS Project** · VIT-AP University  
> **Team:** A.V.S.M.K Swamy · Sathvik · Hemanth · **Samuel (21BCB7145)** · Nihal · Sai Sagar  
> **Guide:** Dr. Syed Khasim · SCOPE, VIT-AP

-----

## 📌 Overview

In cases of women’s safety emergencies, traditional phone-based apps fail — the phone may be grabbed, broken, or inaccessible. This project solves that.

We built a **standalone embedded IoT safety device** that:

- Requires **no smartphone** to operate
- Uses **fingerprint authentication** to prevent misuse
- Sends the user’s **live GPS coordinates via SMS** to emergency contacts
- Sounds a **buzzer alarm** to alert nearby people
- **Works automatically** — if the woman doesn’t check in within 1 minute, it triggers on its own

-----

## 🔐 How It Works

```
DEVICE SETUP
    → Woman registers fingerprint on device
    → Emergency contact numbers stored in GSM module

DEVICE ACTIVATED (woman turns it on)
    ↓
  Continuous fingerprint scan expected every 60 seconds
    ↓
NORMAL: Fingerprint detected → System stays silent
    ↓
EMERGENCY: No fingerprint for 60s (unconscious, attacked, restrained)
    ↓
  ┌─────────────────────┐    ┌────────────────────────────────────┐
  │  BUZZER SOUNDS      │    │  SMS SENT with Google Maps Link    │
  │  (alerts bystanders)│    │  "Coordinates: lat, lon → Maps URL"│
  └─────────────────────┘    └────────────────────────────────────┘
```

**Key Insight:** The user doesn’t need to *do* anything in an emergency — the device acts automatically if she *stops* responding.

-----

## 🔧 Hardware Components

|Component              |Purpose                             |
|-----------------------|------------------------------------|
|**Arduino Uno**        |Main microcontroller                |
|**GSM Module**         |Send SMS alerts via cellular network|
|**GPS Module**         |Get real-time location coordinates  |
|**Fingerprint Scanner**|Authenticate the registered user    |
|**LCD Display (16×2)** |Status messages to the user         |
|**Buzzer**             |Audio alarm for nearby people       |
|**LED indicators**     |Visual status feedback              |
|**Push Buttons**       |Enrollment trigger                  |
|**Power Supply**       |Portable battery-powered            |

-----

## 💻 Software

|Component    |Details                                                                                  |
|-------------|-----------------------------------------------------------------------------------------|
|**IDE**      |Arduino IDE                                                                              |
|**Language** |C (Arduino/ATmega328)                                                                    |
|**Libraries**|`Adafruit_Fingerprint.h` · `TinyGPS.h` · `LiquidCrystal.h` · `SoftwareSerial` · `Servo.h`|

-----

## ⚙️ Key Functions

### Fingerprint Enrollment (`enroll_finger()`)

1. Prompts user to input an ID
1. Captures 2 images of the same finger
1. Converts to templates and matches them
1. Stores the fingerprint model in sensor memory

### GPS Reading (`read_gps()`)

- Continuously parses NMEA data from GPS module
- Extracts latitude/longitude in real-time

### SMS Alert (`send_sms()`)

- Sends AT commands to GSM module
- Message includes: alert text + Google Maps link with coordinates
- Sent to all pre-registered emergency numbers

### Main Loop (`loop()`)

```
Every cycle:
  → Read GPS coordinates
  → Display "Scan Thumb" on LCD
  → Wait for fingerprint scan
  → If recognized: reset 60s timer
  → If timer expires: trigger buzzer + send SMS
```

-----

## 📁 Project Structure

```
women-safety-device/
├── women_safety_device.ino    # Main Arduino sketch (fingerprint + GPS + GSM alerts)
└── README.md
```

-----

## 🚀 How to Deploy

### Step 1 — Install Arduino IDE

Download and install from: https://www.arduino.cc/en/software

### Step 2 — Install required libraries

Open Arduino IDE → **Sketch → Include Library → Manage Libraries**, then search and install:

|Library                 |Search for                           |
|------------------------|-------------------------------------|
|`Adafruit_Fingerprint.h`|`Adafruit Fingerprint Sensor Library`|
|`TinyGPS.h`             |`TinyGPS`                            |
|`LiquidCrystal.h`       |Built-in — no install needed         |
|`SoftwareSerial.h`      |Built-in — no install needed         |

### Step 3 — Update emergency contact numbers

Open `women_safety_device.ino` and find the `send_sms()` function.  
Replace the phone numbers with your actual emergency contacts:

```c
Serial.print("AT+CMGS=\"9949592453\"\r\n");  // ← Replace with Contact 1
Serial.print("AT+CMGS=\"8978137886\"\r\n");  // ← Replace with Contact 2
Serial.println("ATD8978137886;");             // ← Replace with number to call
```

### Step 4 — Wire the hardware

Connect components to Arduino Uno as per pin definitions in the sketch:

|Component         |Pins                                  |
|------------------|--------------------------------------|
|LCD (16×2)        |RS=8, E=9, D4=10, D5=11, D6=12, D7=13 |
|Fingerprint sensor|RX=2, TX=3 (SoftwareSerial) · VCC=3.3V|
|GPS module        |Shares Serial (pin 0/1) at 9600 baud  |
|GSM module        |Shares Serial at 9600 baud            |
|Buzzer            |Pin 5                                 |
|Enroll button     |Pin 6 (INPUT_PULLUP)                  |


> **Note:** GPS and GSM both share the hardware Serial in this sketch. For a real deployment, use an Arduino Mega which has multiple hardware Serial ports (Serial1, Serial2) to separate them.

### Step 5 — Upload the sketch

1. Connect Arduino Uno to your PC via USB
1. Open `women_safety_device.ino` in Arduino IDE
1. Go to **Tools → Board → Arduino Uno**
1. Go to **Tools → Port** → select the correct COM port
1. Click **Upload** (→ arrow button)

### Step 6 — Enroll your fingerprint (first time only)

1. **Hold the enroll button (pin 6) while powering on** the device
1. LCD will show `ENROL..`
1. Place your finger on the sensor when prompted
1. Remove finger, then place the same finger again
1. LCD confirms enrollment — device is ready

### Step 7 — Normal operation

1. Power on the device (without holding enroll button)
1. LCD shows `SCAN THUMB..` with live GPS coordinates
1. Place registered finger → buzzer sounds briefly → SMS alert sent to emergency contacts with Google Maps link
1. Device is now active and monitoring

-----

## 🔮 Future Scope

- [ ] Compact into a **wearable** form factor (bracelet/pendant)
- [ ] Support **multiple emergency contacts**
- [ ] Two-way voice call capability
- [ ] Integration with **local police station numbers**
- [ ] Mobile app companion for remote status monitoring

-----

## 📹 Demo

🎥 <a href="https://drive.google.com/file/d/1TGw5IVlSfi0Cq1VD3_UB_F7JaSD6vcau/view?usp=sharing" target="_blank">Working Prototype Video</a>

-----

*VIT-AP University · SCOPE · IoT & Embedded Systems Project*