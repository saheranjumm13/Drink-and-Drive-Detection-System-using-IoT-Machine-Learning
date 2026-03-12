# 🚗 Drink and Drive Detection System using IoT & Machine Learning

## 📌 Project Overview
This project detects whether a driver has consumed alcohol using an alcohol sensor and machine learning model. 
The system uses IoT technology to monitor sensor data and prevent the vehicle from starting if alcohol levels exceed a safe limit.

## 🎯 Objectives
- Detect alcohol in driver's breath
- Prevent vehicle ignition when alcohol is detected
- Use IoT to send sensor data
- Apply machine learning for classification

## ⚙️ Technologies Used

### Hardware
- Arduino UNO
- MQ3 Alcohol Sensor
- Relay Module
- Buzzer
- Jumper Wires

### Software
- Arduino IDE
- Python
- Machine Learning (Scikit-learn)
- IoT Communication

## 🧠 System Architecture
Driver Breath → MQ3 Sensor → Arduino → IoT Communication → ML Model → Decision → Vehicle Control

## 🌐 IoT Cloud Integration

This project uses ThingSpeak to store and monitor real-time sensor data from the alcohol detection system.

The MQ3 alcohol sensor sends data from the Arduino/NodeMCU to the ThingSpeak cloud platform where the readings are stored and visualized as graphs.

### Features
- Real-time alcohol detection data upload
- Cloud-based monitoring
- Graph visualization of sensor values

### Data Flow
Driver Breath → MQ3 Sensor → Arduino/NodeMCU → WiFi → ThingSpeak Cloud → Machine Learning Analysis

### ThingSpeak Channel
You can view the sensor data here:

https://thingspeak.com/channels/3170982

## 📂 Project Structure
Drink-and-Drive-Detection-System/
│
├── FINAL-MINI-PROJECT.ino
├── Mini-Pro/
├── README.md

## ▶️ How to Run
1. Connect MQ3 sensor to Arduino.
2. Upload the Arduino code.
3. Run the ML model.
4. Monitor alcohol detection output.

## 🚀 Future Improvements
- Mobile app integration
- GPS location tracking
- Cloud monitoring dashboard
- Driver face recognition

## 👨‍💻 Author
Saheranjumm
