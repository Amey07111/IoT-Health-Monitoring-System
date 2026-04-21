# IoT-Based Emergency Health Monitoring System

## 📌 Project Overview
This project is an IoT-based embedded system designed to monitor a user’s heart rate and location in real-time and send emergency alerts via SMS when needed. It is built using ESP32 and integrates multiple sensors and communication modules for real-world health and safety applications.

---

## 🚀 Features
- Real-time heart rate monitoring using MAX30105 sensor  
- GPS tracking using Neo-6M GPS module  
- Emergency SMS alert using GSM module (SIM800/900)  
- OLED display for live data visualization  
- Panic button to trigger emergency alert  
- Google Maps location link in SMS  

---

## 🛠️ Components Used
- ESP32 Microcontroller  
- MAX30105 Heart Rate Sensor  
- Neo-6M GPS Module  
- GSM Module (SIM800/900)  
- OLED Display (SSD1306)  
- Push Button  
- Jumper Wires & Breadboard  

---

## ⚙️ Working Principle
1. The system continuously reads heart rate data from the sensor.  
2. GPS module fetches real-time location coordinates.  
3. OLED displays live BPM and system status.  
4. When the emergency button is pressed:  
   - System reads current BPM and GPS location  
   - Sends SMS alert with Google Maps link via GSM module  

---

## 📡 Applications
- Emergency health monitoring  
- Elderly safety systems  
- Remote patient monitoring  
- IoT-based healthcare systems  

---

## 📊 Future Improvements
- Cloud integration for real-time monitoring dashboard  
- AI-based abnormal heart rate detection  
- RTOS-based multitasking system  
- Mobile app integration  

---

## 👨‍💻 Author
Amey Pawar  
Electronics & Telecommunication Engineering Student  

---

## 📌 Note
This project demonstrates real-world embedded system integration using IoT technologies.
