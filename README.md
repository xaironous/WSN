# 🌱 Wireless Sensor Network (WSN) for Soil Monitoring using ESP-NOW

This project implements a **Wireless Sensor Network (WSN)** using **ESP-NOW** to monitor soil conditions. The sensor node collects **temperature, humidity, and soil moisture data** and transmits it wirelessly to a receiver node. The system is designed to be **low-power and energy-efficient**, using **ESP32 deep sleep mode** to extend battery life.

---

## 📌 Project Overview
- ✅ **Technology Used:** ESP32, ESP-NOW, DHT11, Soil Moisture Sensor.
- 📡 **Communication:** Uses **ESP-NOW** protocol for efficient wireless data transfer.
- 🔋 **Power Efficiency:** Implements **deep sleep mode** to reduce power consumption.
- 🌐 **Time Synchronization:** Uses **NTP (Network Time Protocol)** to ensure data is sent at regular intervals.
- 📊 **Data Collected:**
  - 🌡️ Temperature (°C)
  - 💧 Humidity (%)
  - 🌱 Soil Moisture (ADC Value)

---

## 🛠️ Hardware Components
| Component               | Description                       |
|-------------------------|----------------------------------|
| **ESP32**              | Microcontroller for sensor node  |
| **DHT11 Sensor**       | Measures temperature & humidity |
| **Soil Moisture Sensor** | Detects soil moisture levels   |
| **ESP-NOW Protocol**    | Low-power wireless communication |

---

## 🔄 System Workflow
1️⃣ **ESP32 wakes up from deep sleep.**

2️⃣ **Reads data** from **DHT11** (temperature & humidity) and soil moisture sensor.

3️⃣ **Transmits data** using **ESP-NOW** to the receiver ESP32.

4️⃣ **Deep sleep is activated**, optimizing power consumption.

5️⃣ **Repeats every 10 minutes**, synchronized via **NTP time server**.

---
