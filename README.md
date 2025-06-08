# 💡 Smart LED Lighting

## 📘 Project Description
The **"Smart LED Lighting"** project enables automatic and manual control of LED lights using an ESP32 microcontroller. The system uses photoresistors (LDRs) to detect ambient light levels and turns LEDs on or off based on predefined thresholds. Users can access a web interface via a phone or computer to change each LED’s mode and adjust the sensitivity threshold.

## 🔧 Features
- Automatic LED activation based on ambient light (via LDR sensors)  
- Three operation modes per LED: **OFF**, **ON**, **AUTO**  
- Web interface for mode selection and threshold adjustment  
- OLED screen displays LED statuses and the device’s IP address  

## 🧰 Components Used
- ESP32 microcontroller (with WiFi)  
- 2× photoresistors (LDR)  
- 2× LEDs (red and yellow)  
- OLED display 128x64 px (SSD1306)  
- 2× 10kΩ resistors (for LDRs)  
- 2× 220Ω resistors (for LEDs)  
- Jumper wires and power supply (USB or battery)

![WhatsApp Slika 2025-06-02 u 17 06 41_212f2a0d](https://github.com/user-attachments/assets/422d2512-0344-429c-ac4d-d1b0aa1660f6)
![WhatsApp Slika 2025-06-02 u 17 06 41_ef96d776](https://github.com/user-attachments/assets/1645edb5-26ff-4187-922f-b8e6105c3b1a)



## 🧠 Libraries Used
- `Wire` – I2C communication  
- `Adafruit_GFX` – text and graphics rendering  
- `Adafruit_SSD1306` – OLED display driver  
- `WiFi` – connects to local WiFi network  
- `WebServer` – handles HTTP routes  

## 🔁 System Workflow
1. The OLED screen and WiFi connection are initialized  
2. A local web server is launched on the network  
3. The loop reads values from LDR sensors  
4. LEDs are turned ON/OFF depending on the selected mode  
5. OLED displays LED status and IP address  
6. Web interface allows mode switching and threshold editing  

## ⚡ Power Consumption
| Component              | Average Current Draw |
|------------------------|----------------------|
| ESP32 (WiFi on)        | ~150 mA              |
| OLED display           | ~25 mA               |
| 2× LEDs (avg 50%)      | ~10 mA               |
| **Total**              | **~185 mA**          |

- **Estimated daily consumption:** ~4440 mAh  
- **Battery capacity used:** 20,000 mAh  
- **Expected battery life:** ≈ 4.5 days  

## 🌐 Web Interface
The user-friendly web interface includes:
- Current mode display for each LED  
- Buttons for changing modes (OFF, ON, AUTO)  
- Input fields for updating LDR thresholds (0–4095)  
- Fully responsive design for mobile and desktop devices  

![WhatsApp Slika 2025-06-02 u 17 06 40_1c680f74](https://github.com/user-attachments/assets/ab7e2ead-4dd0-485f-87d1-30dd839db23c)

## 📺 OLED Display
- **Line 1**: LED1 status (e.g. `AUTO`)  
- **Line 2**: LED2 status (e.g. `OFF`)  
- **Line 3–4**: IP address (e.g. `192.168.1.145`)

![WhatsApp Slika 2025-06-02 u 17 32 54_eb54c650](https://github.com/user-attachments/assets/af728c12-05ad-4ce5-94e6-2877ffeb6383)


## 🚀 Getting Started
1. Wire components according to your setup  
2. Set your WiFi SSID and password in the code  
3. Upload the sketch to ESP32 using Arduino IDE  
4. Open the IP shown on the OLED display in your browser  
5. Control LEDs via the web interface  
