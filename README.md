# Finger Gesture LED Controller

A computer-vision project that uses MediaPipe hand-tracking to recognize finger gestures and wirelessly control LEDs on an ESP32 through a webserver.

### 🔧 What it does
- Tracks hand and finger positions using MediaPipe + OpenCV
- Counts visible fingers and maps them to LED states
- Sends commands wirelessly to ESP32 web server
- LED reacts in real time

### 🧰 Tech Stack
- Python (MediaPipe, OpenCV)
- ESP32 microcontroller
- WiFi communication (HTTP requests)
- LED strip or single LEDs

### 🎯 Purpose
This project was a stepping stone to learning CV-to-hardware integration. It became my test platform for the Blind Navigation system.

### 📎 Media
Demo video and setup images included in `media/`
