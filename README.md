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

## 🔄 Project Workflow

1. **Driver Interaction**  
The driver attempts to start the vehicle. The MQ-3 alcohol sensor is placed near the steering wheel to detect alcohol from the driver's breath.

2. **Alcohol Detection**  
The MQ-3 sensor measures alcohol concentration and sends the data to the microcontroller.

3. **Microcontroller Processing**  
The Arduino / NodeMCU reads the sensor values and processes the alcohol level data.

4. **IoT Data Transmission**  
The processed data is transmitted to the cloud platform (ThingSpeak) using WiFi connectivity.

5. **Cloud Data Monitoring**  
ThingSpeak stores and visualizes the alcohol sensor data using graphs.

6. **Machine Learning Analysis**  
The collected data is used by a machine learning model to classify the driver's condition.

7. **Decision Making**  
If alcohol level exceeds the threshold:
- Vehicle ignition is disabled
- Alert system is activated

If alcohol is not detected:
- Vehicle ignition is allowed.

8. **Alert System**  
A buzzer alert is triggered and the data is stored in the cloud for monitoring.

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
