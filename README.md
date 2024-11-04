# Adafruit Propmaker 2040 Reactive sound Neopixel Ring

This project uses an Adafruit RP2040 Prop-Maker Feather with a 3" 4-ohm speaker, a NeoPixel ring, and a Flight Distance Sensor. The project can also be used with a PIR sensor by making minor adjustments.

## Getting Started

1. **Clone the Repository Or download files**:
   
   ```bash
   git clone https://github.com/YourUsername/YourRepoName.git

2. Open the Project Files in Arduino & select the RP2040 Propmaker Feather:
   - Open `main.ino` in the Arduino IDE or your preferred editor. This is the main control file for the project.
   - `audio.h` handles the audio playback functionality.

![Adafruit RP2040 Prop-Maker Feather Pinout](https://github.com/LordTenderBacon/Adafruit-Prop-Maker-RP2040-Halloween-Reactive-Light-Sound/blob/842d7c6e3f693a8c098c2a4269f08d43491ea645/Images/adafruit_products_Adafruit_RP2040_Prop-Maker_Feather_PrettyPins.png)

# Fritzing Diagram For Refference

![Adafruit RP2040 Prop-Maker Feather Pinout](https://github.com/LordTenderBacon/Adafruit-Prop-Maker-RP2040-Halloween-Reactive-Light-Sound/blob/88aaac8bb561947d840b6064d47570c3e6d90e98/Images/propmaker2040_bb.png)

3. Hardware Setup:
   - Flight Distance Sensor:
     - Signal (GPIO): Connect A3 on the RP2040 Prop-Maker to the sensor’s GPIO.
     - I2C Communication: Connect SDA and SCL on the Prop-Maker to the sensor's SDA and SCL pins, respectively.
     - Power: Connect the sensor’s VCC to 3V and GND to GND on the Prop-Maker.
   
   - Speaker: 
     - Plug the 3" 4-ohm speaker into the onboard solderless terminal on the Prop-Maker, using the designated speaker terminals.
   
   - NeoPixel Ring:
     - Data (GPIO 21): Connect the NeoPixel’s data line to GPIO 21 on the Prop-Maker.
     - Power: Connect 5V on the NeoPixel ring to 5V on the Prop-Maker.
     - Ground: Connect GND on the NeoPixel ring to GND on the Prop-Maker.

4. Upload the Code:
   - Set the Board in the Arduino IDE: Set your board in the Arduino IDE to Adafruit Feather RP2040.
   - Upload `main.ino` to your Feather RP2040.

5. Using the Project:
   - Functionality: When powered on, the RP2040 controls playback using the `audio.h` file and interacts with the distance sensor to trigger actions based on proximity.
   - Customization: The distance sensor logic can be modified to work with a PIR sensor by updating relevant sensor input lines in `main.ino`.
