# M5Stack OneShot125 Thruster Control

This project demonstrates how to control an ESC (Electronic Speed Controller) using the **OneShot125** protocol with an M5Stack device. The program allows users to adjust the thruster's speed and direction using the M5Stack buttons.

---

## Features
- **OneShot125 Protocol**:
  - Frequency: 1 kHz.
  - Pulse Width Range: 125 µs (min) to 250 µs (max).
  - Neutral Pulse: 190 µs.
- **Thruster Control**:
  - `BtnA`: Increase speed (forward).
  - `BtnB`: Decrease speed (reverse).
  - `BtnC`: Set to neutral (stop).
- **Real-Time Display**:
  - Current thruster value.
  - Pulse width in microseconds.
  - Duty cycle for ESC.

---

## Hardware Requirements
- **M5Stack Core** or compatible device.
- ESC (Electronic Speed Controller) supporting OneShot125.
- Thruster or motor connected to the ESC.
- Pin 26 configured for PWM output.

---

## Software Requirements
- **PlatformIO** with the following configuration:
  - Framework: Arduino.
  - Board: M5Stack Core ESP32.
  - Library Dependencies:
    - `M5Stack` or `M5Unified`.

---

## How to Use
1. **Connect the Hardware**:
   - Connect the ESC signal wire to pin 26 on the M5Stack.
   - Ensure the ESC is powered and connected to the thruster/motor.

2. **Upload the Code**:
   - Clone this repository:
     ```bash
     git clone https://github.com/your-username/m5stack-oneshot125.git
     ```
   - Open the project in PlatformIO.
   - Build and upload the code to your M5Stack device.

3. **Control the Thruster**:
   - Use the M5Stack buttons:
     - `BtnA`: Increase speed (forward).
     - `BtnB`: Decrease speed (reverse).
     - `BtnC`: Set to neutral (stop).
   - View real-time information on the M5Stack display.

---

## Code Overview
### [mapThrusterToMicros(int value)](http://_vscodecontentref_/0)
Maps the thruster value (`-127` to `127`) to a pulse width in microseconds (`125 µs` to `250 µs`) with a neutral value of `190 µs`.

### [setup()](http://_vscodecontentref_/1)
- Initializes the M5Stack and configures pin 26 for PWM output at 1 kHz.

### [loop()](http://_vscodecontentref_/2)
- Reads button inputs to adjust the thruster value.
- Converts the thruster value to a PWM duty cycle and sends it to the ESC.
- Displays the current thruster state on the M5Stack screen.

---

## Example Output
