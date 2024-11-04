// SPDX-License-Identifier: MIT

#include <Adafruit_NeoPixel.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <Servo.h>
#include <I2S.h>
#include <Adafruit_VL6180X.h>
#include "audio.h"

#define SENSOR_GPIO_PIN 29  // Set to A3 for proximity sensor
#define PIR_PIN 15          // PIR sensor on GPIO 15
#define DISTANCE_THRESHOLD 100  // Adjust threshold distance in mm

Adafruit_NeoPixel strip(60, PIN_EXTERNAL_NEOPIXELS, NEO_GRB + NEO_KHZ800);
Adafruit_LIS3DH lis = Adafruit_LIS3DH();
Adafruit_VL6180X vl = Adafruit_VL6180X();
Servo servo_0;

volatile bool tonePlaying = false;
volatile uint16_t currentAmplitude = 0;
bool motionDetected = false;
bool startPlayback = false;
int currentClip = 1;  // Start with the first clip

// Audio sample rate and volume scaling
const uint32_t LAUGH_SAMPLE_RATE = 22050;
const float volumeScaling = 1.0;

void setup() {
    Serial.begin(115200);

    if (!lis.begin(0x18)) {
        Serial.println("Could not start LIS3DH");
        while (1) yield();
    }

    lis.setRange(LIS3DH_RANGE_2_G);

    pinMode(PIN_EXTERNAL_POWER, OUTPUT);
    digitalWrite(PIN_EXTERNAL_POWER, HIGH);

    pinMode(SENSOR_GPIO_PIN, INPUT);

    strip.begin();
    strip.setBrightness(255);
    strip.clear();
    strip.show();

    pinMode(PIN_EXTERNAL_BUTTON, INPUT_PULLUP);

    servo_0.attach(PIN_EXTERNAL_SERVO);

    if (!vl.begin()) {
        Serial.println("Failed to find VL6180X sensor");
        while (1) yield();
    }
}

void loop() {
    int range = vl.readRange();
    uint8_t status = vl.readRangeStatus();

    if (status == VL6180X_ERROR_NONE && range <= DISTANCE_THRESHOLD && !motionDetected) {
        motionDetected = true;
        startPlayback = true;
        Serial.println("Object detected within threshold distance.");
    } else if (range > DISTANCE_THRESHOLD) {
        motionDetected = false;
    }

    if (startPlayback && !tonePlaying) {
        tonePlaying = true;

        switch (currentClip) {
            case 1:
                play_audio_i2s_with_green(audio1Data, AUDIO1_NUM_SAMPLES);
                break;
            case 2:
                play_audio_i2s_with_green(audio2Data, AUDIO2_NUM_SAMPLES);
                break;
            case 3:
                play_audio_i2s_with_green(audio3Data, AUDIO3_NUM_SAMPLES);
                break;
            case 4:
                play_audio_i2s_with_rgb(audio4Data, AUDIO4_NUM_SAMPLES);
                break;
            case 5:
                play_audio_i2s_with_green(audio5Data, AUDIO5_NUM_SAMPLES);
                break;
        }

        currentClip = (currentClip % 5) + 1;  // Cycle through clips
        tonePlaying = false;
        startPlayback = false;
    }

    if (!tonePlaying) {
        strip.clear();
        strip.show();
    }
}

#include <I2S.h>
I2S i2s(OUTPUT);

void setup1() {
    i2s.setBCLK(PIN_I2S_BIT_CLOCK);
    i2s.setDATA(PIN_I2S_DATA);
    i2s.setBitsPerSample(16);

    uint32_t sampleRate = LAUGH_SAMPLE_RATE;

    if (!i2s.begin(sampleRate)) {
        Serial.println("Failed to initialize I2S!");
        i2s.end();
        return;
    }

    i2s.flush();
}

// Play audio with green light reaction
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
                strip.setPixelColor(j, strip.Color(0, brightness, 0));  // Green color
            }
            strip.show();
        }
    }
}

// Play audio with RGB color reaction
void play_audio_i2s_with_rgb(const uint8_t *audioData, uint32_t numSamples) {
    i2s.flush();

    uint32_t sampleCounter = 0;
    uint32_t amplitudeAccumulator = 0;
    const uint32_t samplesPerUpdate = 100;

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

        if (i % samplesPerUpdate == 0 && sampleCounter >= samplesPerUpdate) {
            uint8_t brightness = map(amplitudeAccumulator / sampleCounter, 0, 32767, 0, 255);

            for (int32_t j = 0; j < strip.numPixels(); j++) {
                if (brightness < 85) {
                    strip.setPixelColor(j, strip.Color(brightness * 3, 0, 0));  // Red
                } else if (brightness < 170) {
                    strip.setPixelColor(j, strip.Color(0, (brightness - 85) * 3, 0));  // Green
                } else {
                    strip.setPixelColor(j, strip.Color(0, 0, (brightness - 170) * 3));  // Blue
                }
            }
            strip.show();

            sampleCounter = 0;
            amplitudeAccumulator = 0;
        }
    }
}
