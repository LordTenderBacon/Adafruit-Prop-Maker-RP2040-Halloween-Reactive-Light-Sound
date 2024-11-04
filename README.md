# Adafruit Propmaker 2040 Reactive sound Neopixel Ring

This project uses an Adafruit RP2040 Prop-Maker Feather with a 3" 4-ohm speaker, a NeoPixel ring, and a Flight Distance Sensor. The project can also be used with a PIR sensor by making minor adjustments. There are reffernce images at the very bottom to show how I set mine up. Feel free to email me if you need help with anything or want to know how to adjust the code for a PIR sensor. 

Email: jordanthecoder719@gmail.com

## Getting Started

1. **Clone the Repository Or download files**:
   
   ```bash
   git clone https://github.com/YourUsername/YourRepoName.git

2. Open the Project Files in Arduino & select the RP2040 Propmaker Feather board:
Note that you will need to follow instructions to set up arduino to use your rp2040 if you have not already done so. [Here](https://learn.adafruit.com/adafruit-rp2040-prop-maker-feather/arduino-ide-setup)
   - Open `main.ino` in the Arduino IDE or your preferred editor. This is the main control file for the project.
   - `audio.h` handles the audio playback functionality.

4. Hardware Setup:
   - Flight Distance Sensor:
     - Signal (GPIO): Connect A3 on the RP2040 Prop-Maker to the sensor’s GPIO.
     - I2C Communication: Connect SDA and SCL on the Prop-Maker to the sensor's SDA and SCL pins, respectively.
     - Power: Connect the sensor’s VCC to 3V and GND to GND on the Prop-Maker.
   
   - Speaker: 
     - Plug the 3" 4-ohm speaker into the onboard solderless terminal on the Prop-Maker, using the designated speaker terminals.
   
   - NeoPixel Ring: (Use the Solderless onboard terminal it should be green and have screws)
     - Data (GPIO 21): Connect the NeoPixel’s data line to GPIO 21 on the Prop-Maker.
     - Power: Connect 5V on the NeoPixel ring to 5V on the Prop-Maker.
     - Ground: Connect GND on the NeoPixel ring to GND on the Prop-Maker.

5. Upload the Code:
   - Set the Board in the Arduino IDE: Set your board in the Arduino IDE to Adafruit Feather RP2040.
   - Upload `main.ino` to your Feather RP2040.

6. Using the Project:
   - Functionality: When powered on, the RP2040 controls playback using the `audio.h` file and interacts with the distance sensor to trigger actions based on proximity.
   - Customization: The distance sensor logic can be modified to work with a PIR sensor by updating relevant sensor input lines in `main.ino`.
  

# NeoPixel Sound-Reactive Lighting Project Explained

This project uses an **Adafruit NeoPixel** strip, **ToF (Time of Flight) distance sensor**, **Adafruit LIS3DH accelerometer**, and an **audio playback system** to create a visually reactive lighting effect based on sound.

## Features
-Plays Green light for sound 1-3 and then RGB for 4th sound
- **Green Light Reaction**: The NeoPixels pulse with a green light in response to the sound's amplitude for certain audio clips.
- **RGB Color Reaction**: For selected audio clips, the NeoPixels cycle through red, green, and blue colors, matching the loudness of the sound.
- **Distance Detection**: The **VL6180X ToF sensor** detects when an object is within a threshold distance to trigger the sound and lighting effects.

## Project Setup

1. **Connect NeoPixels**:
   - Attach the NeoPixel strip to the designated GPIO pin for data and connect it to 5V power.
   
2. **Distance Sensor**:
   - Connect the **Adafruit VL6180X** ToF sensor to the I2C pins and **GPIO 29** for the interrupt pin.
   
3. **Audio Playback and I2S Setup**:
   - Set up the audio playback using the I2S library. Audio samples are stored in the `audio.h` file, which includes five different sound clips.

## Code Structure

### Main Components
- **Distance Threshold Trigger**: When an object is detected within a set threshold, the sound playback and NeoPixel reaction begin.
- **Audio-Triggered NeoPixel Reactions**:
  - **Green Light Reaction** (`play_audio_i2s_with_green`): NeoPixels pulse in green based on the audio's loudness.
  - **RGB Color Reaction** (`play_audio_i2s_with_rgb`): NeoPixels display a dynamic RGB color change corresponding to the sound's amplitude.

### Code Explanation

1. **Distance Detection**:
   - The ToF sensor measures distance, and when the threshold is met, it triggers sound playback and NeoPixel lighting.
   
2. **Sound Amplitude Calculation**:
   - The code calculates the audio's amplitude by summing the absolute values of audio samples over small intervals.
   - Every few samples, it calculates an average amplitude, used to set the NeoPixel brightness.
   
3. **NeoPixel Reactions**:
   - **Green Light**: For audio clips 1-3 and 5, the NeoPixels pulse in green, with brightness matching the loudness of the audio.
   - **RGB Reaction**: For audio clip 4, the NeoPixels switch colors based on brightness:
     - Low amplitude = red
     - Medium amplitude = green
     - High amplitude = blue

## Code

Here is the main part of the code that controls the NeoPixel reactions:

```cpp
void play_audio_i2s_with_green(const uint8_t *audioData, uint32_t numSamples) {
    i2s.flush();

    uint32_t sampleCounter = 0;
    uint32_t amplitudeAccumulator = 0;
    const uint32_t samplesPerUpdate = 100;
    const uint32_t neoPixelUpdateFrequency = 700;

    int16_t prevSample = 0;

    for (uint32_t i = 0; i < numSamples; i++) {
        int16_t sample = ((int16_t)audioData[i] - 128) << 8;
        sample = (sample + prevSample) / 2;
        prevSample = sample;

        sample *= volumeScaling;
        amplitudeAccumulator += abs(sample);
        sampleCounter++;

        i2s.write(sample);
        i2s.write(sample);

        if (i % neoPixelUpdateFrequency == 0 && sampleCounter >= samplesPerUpdate) {
            currentAmplitude = amplitudeAccumulator / sampleCounter;
            sampleCounter = 0;
            amplitudeAccumulator = 0;

            uint8_t brightness = map(currentAmplitude, 0, 32767, 0, 255);
            for (int32_t j = 0; j < strip.numPixels(); j++) {
                strip.setPixelColor(j, strip.Color(0, brightness, 0));
            }
            strip.show();
        }
    }
}
```
# Fritzing Diagram For Refference

![Adafruit RP2040 Prop-Maker Feather Pinout](https://github.com/LordTenderBacon/Adafruit-Prop-Maker-RP2040-Halloween-Reactive-Light-Sound/blob/31043f74ed5e22497179848c656d51a6ff016e36/Images/refference.png)

# RP2040 Pinout For Refference
![Adafruit RP2040 Prop-Maker Feather Pinout](https://github.com/LordTenderBacon/Adafruit-Prop-Maker-RP2040-Halloween-Reactive-Light-Sound/blob/842d7c6e3f693a8c098c2a4269f08d43491ea645/Images/adafruit_products_Adafruit_RP2040_Prop-Maker_Feather_PrettyPins.png)

# Youtube Video Of working Project

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/MXi40_avDd0/0.jpg)](https://www.youtube.com/watch?v=MXi40_avDd0)
