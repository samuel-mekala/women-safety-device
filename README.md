# 🆘 Women Safety Device with GPS Tracking & Alerts

> **ECS Project** · VIT-AP University  
> **Team:** A.V.S.M.K Swamy · Sathvik · Hemanth · **Samuel (21BCB7145)** · Nihal · Sai Sagar  
> **Guide:** Dr. Syed Khasim · SCOPE, VIT-AP

---

## 📌 Overview

In cases of women's safety emergencies, traditional phone-based apps fail — the phone may be grabbed, broken, or inaccessible. This project solves that.

We built a **standalone embedded IoT safety device** that:
- Requires **no smartphone** to operate
- Uses **fingerprint authentication** to prevent misuse
- Sends the user's **live GPS coordinates via SMS** to emergency contacts
- Sounds a **buzzer alarm** to alert nearby people
- **Works automatically** — if the woman doesn't check in within 1 minute, it triggers on its own

---

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

**Key Insight:** The user doesn't need to *do* anything in an emergency — the device acts automatically if she *stops* responding.

---

## 🔧 Hardware Components

| Component | Purpose |
|---|---|
| **Arduino Uno** | Main microcontroller |
| **GSM Module** | Send SMS alerts via cellular network |
| **GPS Module** | Get real-time location coordinates |
| **Fingerprint Scanner** | Authenticate the registered user |
| **LCD Display (16×2)** | Status messages to the user |
| **Buzzer** | Audio alarm for nearby people |
| **LED indicators** | Visual status feedback |
| **Push Buttons** | Enrollment trigger |
| **Power Supply** | Portable battery-powered |

---

## 💻 Software

| Component | Details |
|---|---|
| **IDE** | Arduino IDE |
| **Language** | C (Arduino/ATmega328) |
| **Libraries** | `Adafruit_Fingerprint.h` · `TinyGPS.h` · `LiquidCrystal.h` · `SoftwareSerial` · `Servo.h` |

---

## ⚙️ Key Functions

### Fingerprint Enrollment (`enrollFingerprint()`)
1. Prompts user to input an ID
2. Captures 2 images of the same finger
3. Converts to templates and matches them
4. Stores the fingerprint model in sensor memory

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

---

## 📁 Project Structure

```
women-safety-device/
├── firmware/
│   └── safety_device.ino      # Main Arduino sketch
├── schematics/
│   └── circuit_diagram.png    # Hardware wiring diagram
├── docs/
│   └── project_report.pdf
└── README.md
```

---

## 🚀 How to Deploy

```
1. Wire components as per circuit diagram
2. Flash firmware using Arduino IDE:
   - Open firmware/safety_device.ino
   - Select Board: Arduino Uno
   - Upload
3. Enroll fingerprint via Serial Monitor:
   - Type ID number when prompted
   - Scan finger twice
4. Store emergency contact numbers in code
5. Power the device with portable battery
```

---

## 🔮 Future Scope

- [ ] Compact into a **wearable** form factor (bracelet/pendant)
- [ ] Support **multiple emergency contacts**
- [ ] Two-way voice call capability
- [ ] Integration with **local police station numbers**
- [ ] Mobile app companion for remote status monitoring

---

## 📹 Demo

🎥 <a href="https://drive.google.com/file/d/1TGw5IVlSfi0Cq1VD3_UB_F7JaSD6vcau/view?usp=sharing" target="_blank">Working Prototype Video</a>

---

*VIT-AP University · SCOPE · IoT & Embedded Systems Project*
