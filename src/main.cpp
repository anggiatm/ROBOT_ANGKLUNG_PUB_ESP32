#include <Arduino.h>
#include <ESP32Servo.h>
#include <Adafruit_PWMServoDriver.h>
// #include <servos.h>

#include <U8g2lib.h>
#include <Wire.h>

#include "configuration.h"

#include <esp_now.h>
#include <WiFi.h>

// Deklarasi variabel
uint8_t ROBOT_18[] = {0x94, 0xB9, 0x7E, 0xD9, 0x14, 0x50};

typedef struct data_esp_now
{
      uint16_t val;
      uint8_t chan;
} data_esp_now;

data_esp_now data;

esp_now_peer_info_t peerInfo;
// #include "midi_config.h"
// #include "channel.h"

// DRIVER & CHANNEL VARIABLE
Adafruit_PWMServoDriver DRIVER[] = {
    Adafruit_PWMServoDriver(0x40),
    Adafruit_PWMServoDriver(0x41),
    Adafruit_PWMServoDriver(0x42),
    Adafruit_PWMServoDriver(0x43)};

const uint8_t midi_numbers[] = {M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11,
                                M12, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10,
                                R11, R12, R13, R14, R15, R16, R17, R18, R19,
                                R20, R21, R22, R23, R24, R25, S1, S2, S3, S4,
                                S5, S6, S7, S8, S9, S10, S11, S12, S13, S14,
                                S15, S16, S17, S18, S19, S20, S21, S22, S23,
                                S24, S25};

struct PIN_CH
{
      Adafruit_PWMServoDriver DRIVER_NUM;
      uint8_t PIN_NUM;
      bool isMotor;
      uint16_t MIN;
      uint16_t MAX;
      uint16_t CENTER;
      uint8_t prev = 64;
      uint8_t target = 64;
      uint16_t x = 0;
      uint8_t vel = 0;
      uint8_t current = 64;
};
PIN_CH CH[72];

// channel c[72];
// const uint8_t CHANNEL[] = {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
//                            48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
//                            61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72,

//                            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
//                            12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
//                            24};

const String NOTES[] = {"C-2", "C#-2", "D-2", "D#-2", "E-2", "F-2", "F#-2", "G-2", "G#-2", "A-2", "A#-2", "B-2",
                        "C-1", "C#-1", "D-1", "D#-1", "E-1", "F-1", "F#-1", "G-1", "G#-1", "A-1", "A#-1", "B-1",
                        "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A0", "A#0", "B0",
                        "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1",
                        "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
                        "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
                        "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
                        "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5",
                        "C6", "C#6", "D6", "D#6", "E6", "F6", "F#6", "G6", "G#6", "A6", "A#6", "B6",
                        "C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "A#7", "B7",
                        "C8", "C#8", "D8", "D#8", "E8", "F8", "F#8", "G8", "G#8", "A8", "A#8", "B8"};

// QUEUE & SERIAL MONITOR
QueueHandle_t xQueue;
uint8_t rx_state = 0;
byte cc_type1;
byte cc_type2;
byte cc_val1;
byte cc_val2;
uint8_t S2_vel = 0;
uint8_t S1_vel = 0;

int velocityToPwm(int velocity)
{
      return round(map(velocity, 0, 127, 0, 255));
}

void execute(uint8_t ch, uint8_t vel)
{
      if (ch == R6)
      {
            data.val = (uint16_t)vel;
            data.chan = 0;
            esp_now_send(0, (uint8_t *)&data, sizeof(data_esp_now));
      }

      if (ch == S5)
      {
            data.val = (uint16_t)vel;
            data.chan = 1;
            esp_now_send(0, (uint8_t *)&data, sizeof(data_esp_now));
      }

      if (CH[ch].isMotor)
      {
            // noteToSend = map(vel, 0, 127, 0, 4095);

            CH[ch].DRIVER_NUM.setPWM(CH[ch].PIN_NUM, 0, map(vel, 0, 127, 0, 4095));
      }
      else
      {

            if (vel > 0 && vel <= 64)
            {
                  CH[ch].DRIVER_NUM.writeMicroseconds(CH[ch].PIN_NUM, map(vel, 0, 64, CH[ch].MIN, CH[ch].CENTER - 1));
            }
            else if (vel > 64)
            {
                  CH[ch].DRIVER_NUM.writeMicroseconds(CH[ch].PIN_NUM, map(vel, 65, 127, CH[ch].CENTER + 1, CH[ch].MAX));
            }
            else
            {
                  CH[ch].DRIVER_NUM.writeMicroseconds(CH[ch].PIN_NUM, CH[ch].CENTER);
            }
      }
}

int bytesToInt(int l_highByte, int l_lowByte)
{
      return ((unsigned int)l_highByte << 8) + l_lowByte;
}

void serial(void *pvParameters)
{
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      for (;;)
      {
            if (Serial.available())
            {
                  // Serial.println(Serial.read());
                  rx_state++;
                  switch (rx_state)
                  {
                  case 1:
                        cc_type1 = Serial.read();
                        if (cc_type1 != 255)
                        {
                              rx_state = 0;
                        }
                        break;
                  case 2:
                        cc_type2 = Serial.read();
                        break;
                  case 3:
                        cc_val1 = Serial.read();
                        break;
                  case 4:
                        cc_val2 = Serial.read();
                        rx_state = 0;

                        int control = cc_type2;
                        int value = bytesToInt(cc_val1, cc_val2);
                        // int DATA_QUEUE[] = {control, value};
                        // xQueueSend(xQueue, &DATA_QUEUE, portMAX_DELAY);
                        if (control <= 72)
                        {

                              if (control <= 24)
                              {
                                    CH[control].vel = value;
                              }
                              else
                              {
                                    execute(control, value);
                              }
                        }

                        Serial.print(255);
                        Serial.print(" ");
                        Serial.print(control);
                        Serial.print(" ");
                        Serial.print(highByte(value));
                        Serial.print(" ");
                        Serial.println(lowByte(value));
                  }
            }
      }
      vTaskDelete(NULL);
}

void run_S2(void *pvParameters)
{
      vTaskDelay(2000 / portTICK_PERIOD_MS);

      uint8_t ReceivedValue[] = {0, 0};

      uint8_t loop = 0;
      for (;;)
      {
            // if (xQueueReceive(xQueue, &ReceivedValue, portMAX_DELAY) == pdPASS)
            // {
            // }

            if (CH[loop].vel == 0)
            {
                  CH[loop].target = 64;
            }
            else
            {
                  CH[loop].target = CH[loop].vel;
            }

            if (CH[loop].current != CH[loop].target)
            {
                  if (CH[loop].vel == 0)
                  {
                        CH[loop].target = 64;
                        CH[loop].prev = CH[loop].current;
                        CH[loop].x = 0;
                  }

                  if (CH[loop].vel != CH[loop].target && CH[loop].vel != 0)
                  {
                        CH[loop].prev = CH[loop].current;
                        CH[loop].x = 0;
                        CH[loop].target = CH[loop].vel;
                  }
                  CH[loop].x = CH[loop].x + 3;

                  // CH[loop].current = round(CH[loop].prev + (CH[loop].target - CH[loop].prev) / (1 + pow(0.9, (CH[loop].x - 40))));
                  CH[loop].current = CH[loop].prev + (CH[loop].x * ((CH[loop].target - CH[loop].prev) / abs(CH[loop].target - CH[loop].prev)));
                  CH[loop].DRIVER_NUM.writeMicroseconds(CH[loop].PIN_NUM, map(CH[loop].current, 1, 127, CH[loop].MIN, CH[loop].MAX));
                  // vTaskDelay(5 / portTICK_PERIOD_MS);
            }

            CH[loop].prev = CH[loop].current;
            CH[loop].x = 0;

            loop++;

            if (loop >= 24)
            {
                  loop = 0;
            }

            // if (S2_vel == 0)
            // {
            //       target = 64;
            // }
            // else
            // {
            //       target = S2_vel;
            // }
            // while (current != target)
            // {
            //       if (S2_vel == 0)
            //       {
            //             target = 64;
            //             prev = current;
            //             x = 0;
            //       }
            //       if (S2_vel != target && S2_vel != 0)
            //       {
            //             prev = current;
            //             x = 0;
            //             target = S2_vel;
            //       }

            //       x++;
            //       current = round(prev + (target - prev) / (1 + pow(0.9, (x - 40))));
            //       CH[S2].DRIVER_NUM.writeMicroseconds(CH[S2].PIN_NUM, map(current, 1, 127, CH[S2].MIN, CH[S2].MAX));
            //       vTaskDelay(10 / portTICK_PERIOD_MS);
            // }
            // prev = current;
            // x = 0;
      }
      vTaskDelete(NULL);
}

void setup()
{
      Serial.begin(38400);

      Wire.begin(SDA, SCL);
      Wire.setClock(400000);

      pinMode(LED_BUILTIN, OUTPUT);

      // MIDI_CONFIG.init_driver();

      for (uint8_t i = 0; i < sizeof(DRIVER) / sizeof(DRIVER[0]); i++)
      {
            DRIVER[i].begin();
            DRIVER[i].setOscillatorFrequency(25000000);
            DRIVER[i].setPWMFreq(50);
      }

      // for (uint8_t i = 0; i < 4; i++)
      // {
      //       PWM_DRIVER[i].begin();
      //       PWM_DRIVER[i].setOscillatorFrequency(25000000);
      //       PWM_DRIVER[i].setPWMFreq(SERVO_FREQ);
      // }

      // channel C[72];

      // c = MIDI_CONFIG.getConfiguration();

      // DEFAULT VALUE
      uint8_t driver_num = 0;
      uint8_t pin_num = 0;

      for (uint8_t i = 0; i < sizeof(midi_numbers) / sizeof(midi_numbers[0]); i++)
      {
            CH[midi_numbers[i]].isMotor = true;
            CH[midi_numbers[i]].DRIVER_NUM = DRIVER[driver_num];
            CH[midi_numbers[i]].PIN_NUM = pin_num;
            CH[midi_numbers[i]].MAX = 0;
            CH[midi_numbers[i]].MIN = 0;
            CH[midi_numbers[i]].CENTER = 0;
            // Serial.print(pin_num);
            // Serial.print("  ");
            // Serial.println(driver_num);

            pin_num++;
            if (pin_num == 16)
            {
                  pin_num = 0;
                  driver_num++;
            }
      }

      CH[S1].isMotor = false;
      CH[S1].MAX = 1800;
      CH[S1].MIN = 1200;
      CH[S1].CENTER = 1500;
      CH[S2].isMotor = false;
      CH[S2].MAX = 1800;
      CH[S2].MIN = 1200;
      CH[S2].CENTER = 1500;
      CH[S3].isMotor = false;
      CH[S3].MAX = 1800;
      CH[S3].MIN = 1200;
      CH[S3].CENTER = 1500;
      CH[S4].isMotor = false;
      CH[S4].MAX = 1800;
      CH[S4].MIN = 1200;
      CH[S4].CENTER = 1500;
      CH[S5].isMotor = false;
      CH[S5].MAX = 1800;
      CH[S5].MIN = 1200;
      CH[S5].CENTER = 1500;

      CH[S6].isMotor = false;
      CH[S6].MAX = 1000;
      CH[S6].MIN = 2000;
      CH[S6].CENTER = 1500;

      CH[S7].isMotor = false;
      CH[S7].MAX = 1800;
      CH[S7].MIN = 1200;
      CH[S7].CENTER = 1500;

      CH[S8].isMotor = false;
      CH[S8].MAX = 1950;
      CH[S8].MIN = 1450;
      CH[S8].CENTER = 1500;

      CH[S9].isMotor = false;
      CH[S9].MAX = 1750;
      CH[S9].MIN = 1250;
      CH[S9].CENTER = 1500;

      CH[S10].isMotor = false;
      CH[S10].MAX = 1600;
      CH[S10].MIN = 1150;
      CH[S10].CENTER = 1500;

      CH[S11].isMotor = false;
      CH[S11].MAX = 1800;
      CH[S11].MIN = 1200;
      CH[S11].CENTER = 1500;
      CH[S12].isMotor = false;
      CH[S12].MAX = 2000;
      CH[S12].MIN = 1250;
      CH[S12].CENTER = 1500;

      CH[S13].isMotor = false;
      CH[S13].MAX = 2000;
      CH[S13].MIN = 1000;
      CH[S13].CENTER = 1500;

      CH[S14].isMotor = false;
      CH[S14].MAX = 2100;
      CH[S14].MIN = 1100;
      CH[S14].CENTER = 1500;

      CH[S15].isMotor = false;
      CH[S15].MAX = 1800;
      CH[S15].MIN = 1200;
      CH[S15].CENTER = 1500;
      CH[S16].isMotor = false;
      CH[S16].MAX = 1800;
      CH[S16].MIN = 1200;
      CH[S16].CENTER = 1500;

      CH[S17].isMotor = false;
      CH[S17].MAX = 2200;
      CH[S17].MIN = 1300;
      CH[S17].CENTER = 1500;

      CH[S18].isMotor = false;
      CH[S18].MAX = 2100;
      CH[S18].MIN = 1300;
      CH[S18].CENTER = 1500;

      CH[S19].isMotor = false;
      CH[S19].MAX = 1900;
      CH[S19].MIN = 1300;

      CH[S19].CENTER = 1500;
      CH[S20].isMotor = false;
      CH[S20].MAX = 1800;
      CH[S20].MIN = 1200;
      CH[S20].CENTER = 1500;
      CH[S21].isMotor = false;
      CH[S21].MAX = 1800;
      CH[S21].MIN = 1200;
      CH[S21].CENTER = 1500;
      CH[S22].isMotor = false;
      CH[S22].MAX = 1800;
      CH[S22].MIN = 1200;
      CH[S22].CENTER = 1500;

      CH[S23].isMotor = false;
      CH[S23].MAX = 2000;
      CH[S23].MIN = 1100;
      CH[S23].CENTER = 1500;

      CH[S24].isMotor = false;
      CH[S24].MAX = 1200;
      CH[S24].MIN = 1900;
      CH[S24].CENTER = 1500;

      CH[S25].isMotor = false;
      CH[S25].MAX = 1800;
      CH[S25].MIN = 1200;
      CH[S25].CENTER = 1500;

      WiFi.mode(WIFI_STA);

      if (esp_now_init() != ESP_OK)
      {
            Serial.println("Error initializing ESP-NOW");
            return;
      }

      // esp_now_register_send_cb(OnDataSent);

      // register peer
      peerInfo.channel = 0;
      peerInfo.encrypt = false;
      // register first peer
      memcpy(peerInfo.peer_addr, ROBOT_18, 6);
      if (esp_now_add_peer(&peerInfo) != ESP_OK)
      {
            Serial.println("Failed to add peer");
            return;
      }

      // for (uint8_t driver_index = 0; driver_index < 4; driver_index++)
      // {
      //       for (uint8_t pin_index = 0; pin_index < 17; pin_index++)
      //       {
      //             CH[(16 * driver_index) + pin_index] = {PWM_DRIVER[driver_index], pin_index, true};
      //       }
      // }

      // for (uint8_t i = 37; i < sizeof(CH) / sizeof(CH[0]); i++)
      // {
      //       CH[i].isMotor = false;
      //       // CH[i].MIN = false;
      //       // CH[i].MAX = false;
      //       // CH[i].CENTER = false;
      // }

      // xQueue = xQueueCreate(100, sizeof(uint8_t[2]));
      // if (xQueue != NULL)
      // {
      // xTaskCreatePinnedToCore(lcd_display, "lcd_display", 1024*6, NULL, 3, NULL, 1);
      xTaskCreatePinnedToCore(serial, "Serial", 1024 * 8, NULL, 4, NULL, 1);
      // xTaskCreatePinnedToCore(broadcast_command, "broadcast_command", 1024 * 15, NULL, 4, NULL, 1);
      // xTaskCreatePinnedToCore(run_S1, "S1", 1024 * 2, NULL, 4, NULL, 1);
      xTaskCreatePinnedToCore(run_S2, "S2", 1024 * 8, NULL, 4, NULL, 1);
      // }
      // vTaskDelete(NULL);
}

void loop()
{
}