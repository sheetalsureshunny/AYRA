---
publishDate: 2026-05-24

title: AYRA

excerpt: Real time autonomic dysreflexia early warning using a single below knee, multi modal wearable with TinyML on ESP32.

image: cover.jpg

tags:
  - wearable
  - tinyml
  - esp32
---

> Real-time autonomic dysreflexia detection from a single below-knee wearable.

---

## Acknowledgements

We are Aaron, Mathew, Abhay, and Sheetal--the members of Team AYRA. We wish to sincerely thank Mr. Anil Anton, IEEE Sensors Council KS Academic Coordinator, for his invaluable mentorship, technical guidance, and unwavering encouragement throughout the development of this project. His academic oversight, organizational support, and belief in our work proved instrumental at every stage of bringing AYRA to life. We are further deeply grateful to the *IEEE Sensors Council* for providing the MYOSA platform through the hackathon framework, which gave us the opportunity to develop a solution with genuine clinical impact. This has been a privilege we truly cherish.

## Overview

AYRA is a compact below-knee wearable that detects pre-symptomatic autonomic dysreflexia (AD) in people with spinal cord injury at or above T6/T7. It fuses multi-modal signals (GSR, HRV/PPG, ECG, skin temperature, humidity, and spasm motion) directly below the lesion, runs TinyML inference on an ESP32, and issues local + MQTT alerts within seconds.

**Key features:**
* Single below-knee placement with no inter body wiring
* 8-sensor fusion for early AD detection
* On device TinyML inference (int8) with <50 ms latency
* Real time alerting via OLED, buzzer, and Wi-Fi MQTT
* Companion app dashboard for results, alerts, and trends

AYRA includes a companion app for results and alerts; check the app screenshots below to understand the app experience.

## Demo / Examples

### Images

<p align="center">
  <img src="/cover.jpg" width="800"><br/>
  <i>AYRA wearable placement and enclosure overview</i>
</p>

<p align="center">
  <img src="/chest.jpg" width="800"><br/>
  <i>Chest electrode view</i>
</p>

#### App Screenshots

<p align="center">
  <img src="/app-1.png" width="800"><br/>
  <i>App dashboard view</i>
</p>

<p align="center">
  <img src="/app-2.png" width="800"><br/>
  <i>App results and trends view</i>
</p>

<p align="center">
  <img src="/app-3.png" width="800"><br/>
  <i>App alerts view</i>
</p>

### Videos

<video controls width="100%">
  <source src="/ayra-demo.mp4" type="video/mp4">
</video>

[Download the demo video](ayra-demo.mp4)

## Features (Detailed)

### 1. Below-knee autonomic signal capture
The device is positioned on the medial tibia, below the lesion level, where sympathetic activity is unregulated during AD. This placement amplifies the earliest physiological precursors that are typically missed by wrist or upper-body wearables.

### 2. Multi-sensor fusion for early detection
AYRA combines GSR, HRV/PPG, ECG morphology, skin temperature changes, sweat rate, and spasm motion into a single risk score. Fusion enables earlier and more reliable detection than any individual modality.

### 3. TinyML inference on ESP32
A quantized int8 neural network processes a 60-second sliding window of features (10-second step) and outputs a 0–100% AD risk score locally without cloud dependency.

### 4. Alerting and caregiver notification
When risk exceeds threshold, AYRA activates an OLED alert, buzzer, and sends an MQTT SOS to a caregiver dashboard or phone.

### 5. Ambient normalization with BMP180
The BMP180 barometric pressure sensor (MEMS piezo-resistive, I2C, PCB top side) provides ambient normalization to remove environmental artefact from physiological readings.

### 6. Companion app dashboard
The app displays the safety score, risk level, and sensor trends in real time with alert summaries; see the app screenshots above for the interface.

## Sensor Usage

* **MAX30102 (PPG/HRV):** heart rate and HRV features used for HR spike detection and risk scoring.
* **AD8232 (ECG):** ECG waveform and QRS morphology changes to support risk scoring.
* **GSR electrodes (EDA):** sympathetic surge detection and dGSR/dt spikes.
* **MLX90614 (IR skin temp):** skin temperature drop below lesion as an early AD indicator.
* **MPU6050 (IMU):** spasm and jerk detection plus motion artifacts.
* **SHT31 (humidity/temp):** sweat rate changes and ambient context.
* **BMP180 (pressure):** ambient normalization to remove environmental artifacts.
* **APDS9960 (proximity/gesture/light):** user presence, ambient light correction, and gesture-based alert dismiss.

## Usage Instructions

1. Power ON the ESP32 MYOSA board.
2. Connect all sensors properly.
3. Upload firmware using Arduino IDE.
4. Monitor safety score on OLED display.
5. Open dashboard interface.
6. Observe alerts during simulated transport conditions.

## Tech Stack

* **ESP32 MYOSA Board**
* **Arduino IDE**
* **Embedded C/C++**
* **React.js**
* **Vite**
* **HTML/CSS/JavaScript**
* **IoT Sensors**
* **OLED Display**

## Contribution Notes (Optional)

Open an issue or pull request with improvements, validation results, or UX feedback for clinical workflows.

## License

This project is licensed under the MIT License.
