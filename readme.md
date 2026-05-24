---
publishDate: 2026-05-24
title: AYRA
excerpt: Real-time autonomic dysreflexia early warning using a single below-knee, multi-modal wearable with TinyML on ESP32.
image: cover.jpg
tags:
  - wearable
  - tinyml
  - esp32
---

> Real-time autonomic dysreflexia detection from a single below-knee wearable.

---

## Acknowledgements

None.

## Overview

AYRA is a compact below-knee wearable that detects pre-symptomatic autonomic dysreflexia (AD) in people with spinal cord injury at or above T6/T7. It fuses multi-modal signals (GSR, HRV/PPG, ECG, skin temperature, humidity, and spasm motion) directly below the lesion, runs TinyML inference on an ESP32, and issues local + MQTT alerts within seconds.

**Key features:**
* Single below-knee placement with no inter-body wiring
* 8-sensor fusion for early AD detection
* On-device TinyML inference (int8) with <50 ms latency
* Real-time alerting via OLED, buzzer, and Wi-Fi MQTT

## Demo / Examples

### Images

<p align="center">
  <img src="/cover.jpg" width="800"><br/>
  <i>AYRA wearable placement and enclosure overview</i>
</p>

<p align="center">
  <img src="/leg.jpg" width="800"><br/>
  <i>Leg placement view</i>
</p>

<p align="center">
  <img src="/chest.jpg" width="800"><br/>
  <i>Chest electrode view</i>
</p>

### Videos

<video controls width="100%">
  <source src="/ayra-demo.mp4" type="video/mp4">
</video>

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

## Usage Instructions

1. Charge the device via USB-C and ensure the battery is full.
2. Attach the wearable on the medial tibia, 3 cm below the kneecap, with firm contact on the skin-facing sensors.
3. Power on and remain still for the 30-second baseline calibration.
4. Monitor the OLED risk score. If risk exceeds the threshold, acknowledge the alert locally or via caregiver dashboard.

## Tech Stack

* **ESP32 Dev Module** (Wi-Fi + TinyML)
* **TensorFlow Lite Micro** (int8 inference)
* **Sensors:** MAX30102, AD8232, GSR electrodes, MLX90614, MPU6050, SHT31, BMP180, APDS9960
* **Display/Alert:** SSD1306 OLED, buzzer/vibration, status LED
* **Messaging:** MQTT over Wi-Fi

## Requirements / Installation

For firmware development (if building from source):

```bash
# Example ESP-IDF workflow
idf.py set-target esp32
idf.py build
idf.py flash monitor
```

## File Structure (Optional)

```
/ayra
  ├─ readme.md
  ├─ cover.jpg
  ├─ ayra-demo.mp4
  └─ esp.ino
```

## License (Optional)

TBD.

## Contribution Notes (Optional)

Open an issue or pull request with improvements, validation results, or UX feedback for clinical workflows.
