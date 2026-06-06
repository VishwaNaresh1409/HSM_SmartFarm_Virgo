# HSM — Hardware Sensor Module + Virgo Farm Intelligence

A dual-component project combining an **ESP32 IoT firmware** for real-time vehicle crash/impact detection with an **AI-powered precision agriculture platform** called **Virgo**. The firmware publishes accelerometer data over MQTT to the cloud, while the front-end presents a full product landing page for the Virgo smart farming solution.

---

## Project Components

### 1. ESP32 Crash Detection Firmware (`main.cpp`)

An Arduino/PlatformIO sketch that reads acceleration data from an **MPU6050 IMU**, computes a G-force value, and publishes a JSON payload to an MQTT broker (HiveMQ Cloud) over a secure TLS connection every second.

**What it does:**
- Connects to Wi-Fi and establishes a secure MQTT connection (port 8883, TLS)
- Reads raw X/Y/Z acceleration from the MPU6050 via I²C
- Computes resultant G-force: `√(ax² + ay² + az²) / 16384`
- Publishes readings to the topic `vehicle/crash/data` as JSON
- Prints data to Serial Monitor for local debugging
- Auto-reconnects to MQTT broker if connection drops

**Published payload format:**
```json
{
  "ax": 1024,
  "ay": -512,
  "az": 16100,
  "g": 1.003
}
```

---

### 2. Virgo — AI Farm Intelligence Dashboard (`index.html`)

A fully self-contained, animated landing page for **Virgo**, an AI-driven precision agriculture platform developed at Punjab Engineering College, Chandigarh, under the Wadhwani Foundation.

**Page Sections:**
- **Hero** — Animated Koch-snowflake fractal canvas background, key stats, floating UI cards
- **Stats Bar** — High-level KPIs (farmers impacted, water saved, yield increase, market size)
- **Problem** — Four cards covering pain points: crop loss, water waste, market access, disease detection
- **Solution** — Six feature tiles (soil sensing, crop health AI, water optimisation, market connect, weather alerts, data analytics)
- **Demo** — Embedded interactive dashboard mockup (live sensor cards, charts, maps)
- **Market** — TAM breakdown and go-to-market strategy with animated orbit diagram
- **Financials** — 3-year P&L projections with SVG bar/line chart (profitable by Year 2)
- **CTA + Footer** — Contact, demo request, social links

---

## Hardware Requirements (Firmware)

| Component | Details |
|---|---|
| Microcontroller | ESP32 (NodeMCU-32S) |
| IMU | MPU6050 (I²C, 3-axis accelerometer + gyroscope) |
| Connectivity | Wi-Fi 2.4 GHz + TLS MQTT (HiveMQ Cloud) |

### Wiring

| ESP32 | MPU6050 |
|---|---|
| 3.3V | VCC |
| GND | GND |
| GPIO 21 (SDA) | SDA |
| GPIO 22 (SCL) | SCL |

---

## Software & Dependencies

### PlatformIO (`platformio.ini`)

| Setting | Value |
|---|---|
| Board | `nodemcu-32s` |
| Platform | `espressif32` |
| Framework | Arduino |

**Libraries:**

| Library | Purpose |
|---|---|
| `electroniccats/MPU6050` | IMU driver (I²C) |
| `knolleary/PubSubClient` | MQTT client |
| `arduino-libraries/SPI` | SPI bus support |
| `adafruit/Adafruit BMP085 Library` | Barometric pressure (optional sensor) |
| `adafruit/Adafruit Unified Sensor` | Adafruit sensor abstraction layer |

### Front-End (`index.html`)

No build tools or dependencies required — pure HTML/CSS/JS, single file.

**Google Fonts used:**
- DM Serif Display
- Syne
- DM Mono

---

## Getting Started

### Firmware Setup

1. Clone the repository:
   ```bash
   git clone https://github.com/VishwaNaresh1409/hsm.git
   cd hsm
   ```

2. Open `main.cpp` and fill in your credentials:
   ```cpp
   const char* ssid     = "YOUR_WIFI";
   const char* password = "YOUR_PASSWORD";

   const char* mqtt_server = "YOUR_CLUSTER.s1.eu.hivemq.cloud";
   const char* mqtt_user   = "YOUR_MQTT_USER";
   const char* mqtt_pass   = "YOUR_MQTT_PASSWORD";
   ```

3. Build and flash using PlatformIO:
   ```bash
   pio run --target upload
   ```

4. Open Serial Monitor at **115200 baud** to verify live output:
   ```bash
   pio device monitor
   ```

5. Monitor incoming MQTT messages on the topic `vehicle/crash/data` using HiveMQ's web client or any MQTT explorer tool.

### Landing Page

Simply open `index.html` in any modern browser — no server required.

---

## Configuration Notes

- `espClient.setInsecure()` is used to skip TLS certificate verification. **For production**, replace this with proper CA certificate pinning.
- The sensor publishes every **1000ms**. Adjust the `delay(1000)` in `loop()` for higher/lower frequency.
- G-force threshold logic for crash detection can be added in `loop()` — compare `gForce` against a threshold (e.g. `> 3.0`) and trigger an alert publish on a separate topic.

---

## Project Structure

```
hsm/
├── main.cpp          # ESP32 firmware — MPU6050 + MQTT crash detection
├── platformio.ini    # PlatformIO build configuration
├── index.html        # Virgo product landing page (self-contained)
└── README.md
```

---

## About Virgo

Virgo is a precision agriculture intelligence platform targeting Indian smallholder farmers. It combines IoT soil sensors, AI-based crop health analysis, and market-linkage tools into a single SaaS platform.

**Key Metrics (from landing page):**
- 58% average reduction in water usage
- 43% increase in crop yield
- $9.5B total addressable market
- Projected profitability by Year 2 (₹43.6L net profit on ₹1.9Cr revenue)

**Revenue Model:** Monthly SaaS subscription per farm + hardware licensing to government agri programs.

**Developed at:** Punjab Engineering College, Chandigarh, under the Wadhwani Foundation.

---

## Author

**VishwaNaresh1409** — [GitHub](https://github.com/VishwaNaresh1409)
