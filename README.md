# HSM — Hardware Sensor Module (SmartFarm)

A full-stack IoT smart farming system built on the **ESP32** microcontroller. The firmware reads soil NPK levels, temperature, humidity, air quality, and soil moisture — then publishes the data over **MQTT (HiveMQ Cloud)** every 5 seconds. A **Streamlit Python dashboard** subscribes to the same broker, displays all sensor readings live, and lets you trigger an irrigation pump with a single button click.

---

## System Architecture

```
[Sensors] → [ESP32] → [HiveMQ Cloud MQTT] → [Streamlit Dashboard]
                ↑                                      |
                └──────── PUMP_ON command ─────────────┘
```

- **Topic `farm/sensors`** — ESP32 publishes sensor JSON every 5 s
- **Topic `farm/control`** — Streamlit publishes `PUMP_ON` to trigger the relay

---

## Hardware

| Component | Purpose |
|---|---|
| ESP32 (NodeMCU-32S) | Main microcontroller |
| NPK RS-485 Sensor | Soil nitrogen, phosphorus, potassium via Modbus |
| DHT11 | Air temperature & humidity |
| MQ-135 | Air quality (analog) |
| Capacitive Soil Moisture Sensor | Soil moisture % |
| Relay Module | Controls irrigation pump |
| RS-485 to TTL Module | Serial interface for NPK sensor |

### Pin Definitions

| ESP32 Pin | Connected To |
|---|---|
| GPIO 4 (RE) | RS-485 module RE pin |
| GPIO 5 (DE) | RS-485 module DE pin |
| GPIO 16 (RXD2) | RS-485 module RO (receive) |
| GPIO 17 (TXD2) | RS-485 module DI (transmit) |
| GPIO 27 | DHT11 data pin |
| GPIO 34 | MQ-135 analog output |
| GPIO 35 | Soil moisture sensor analog output |
| GPIO 26 | Relay IN (pump control) |

---

## Firmware (`main.cpp`)

### What it does

Every 5 seconds the ESP32:

1. Reads **N, P, K** values from the NPK sensor over RS-485/Modbus
2. Reads **temperature and humidity** from the DHT11
3. Reads **air quality** (raw ADC) from the MQ-135
4. Reads **soil moisture** (mapped to 0–100%) from the capacitive sensor
5. Serialises everything into a JSON payload and publishes to `farm/sensors`
6. Listens on `farm/control` — if `PUMP_ON` is received, activates the relay for 5 seconds then turns it off

### Published JSON payload

```json
{
  "n": 45,
  "p": 30,
  "k": 60,
  "temp": 28.5,
  "hum": 65.0,
  "air": 1200,
  "soil": 72
}
```

### Key libraries

| Library | Purpose |
|---|---|
| `knolleary/PubSubClient` | MQTT client |
| `bblanchon/ArduinoJson` | JSON serialisation |
| `DHT sensor library` | DHT11 temperature & humidity |
| `electroniccats/MPU6050` | (available, not currently used in main loop) |
| `adafruit/Adafruit BMP085` | (available, not currently used in main loop) |

---

## Dashboard (`mqtt.py`)

A **Streamlit** web app that:

- Connects to HiveMQ Cloud over TLS using **paho-mqtt** in a background thread
- Subscribes to `farm/sensors` and updates a shared data store on every message
- Displays live metrics: Temperature, Humidity, Soil Moisture, NPK values, Air Quality
- Shows a **Low Soil Moisture** warning when soil < 30%
- Has a **💧 Start Pump** button that publishes `PUMP_ON` to `farm/control`
- Auto-refreshes every 2 seconds via `st.rerun()`

---

## Getting Started

### 1. Firmware Setup

Clone the repo and open `main.cpp`. Update the credentials at the top:

```cpp
const char* ssid        = "YOUR_WIFI_SSID";
const char* password    = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "YOUR_CLUSTER.s1.eu.hivemq.cloud";
const char* mqtt_user   = "YOUR_MQTT_USER";
const char* mqtt_pass   = "YOUR_MQTT_PASSWORD";
```

Build and flash with PlatformIO:

```bash
pio run --target upload
```

Monitor serial output at **9600 baud**:

```bash
pio device monitor --baud 9600
```

### 2. Dashboard Setup

Install Python dependencies:

```bash
pip install streamlit paho-mqtt
```

Update the MQTT credentials at the top of `mqtt.py` to match your broker, then run:

```bash
streamlit run mqtt.py
```

Open your browser at `http://localhost:8501`.

---

## Configuration Notes

- `espClient.setInsecure()` skips TLS certificate validation. Fine for prototyping; use proper CA pinning for production.
- Soil moisture mapping assumes raw ADC range of **4095 (dry) → 1200 (wet)**. Recalibrate the `map()` call in `loop()` for your specific sensor.
- Pump runs for a fixed **5 seconds** on each `PUMP_ON` command — adjust the `delay(5000)` in `callback()` as needed.
- Publish interval is **5000 ms** — change `> 5000` in the `millis()` check in `loop()`.

---

## Project Structure

```
hsm/
├── main.cpp          # ESP32 firmware — sensors + MQTT + pump control
├── platformio.ini    # PlatformIO build config (ESP32 Arduino)
├── mqtt.py           # Streamlit dashboard — live data + pump button
├── index.html        # Virgo product landing page (self-contained HTML)
└── README.md
```

---

## About Virgo

`index.html` is a standalone animated landing page for **Virgo**, the AI precision agriculture platform this hardware is part of — developed at **Punjab Engineering College, Chandigarh** under the **Wadhwani Foundation**.

---

## Author

**VishwaNaresh1409** — [GitHub](https://github.com/VishwaNaresh1409)
