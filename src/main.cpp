#include <Arduino.h>
#include <ESP32Servo.h>

#include <U8g2lib.h>
#include <Wire.h>

#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  400 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  3900 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates


// DRIVER & CHANNEL VARIABLE
Adafruit_PWMServoDriver PWM_DRIVER[] = {
      Adafruit_PWMServoDriver(0x40),
      Adafruit_PWMServoDriver(0x41),
      Adafruit_PWMServoDriver(0x42),
      Adafruit_PWMServoDriver(0x43)
};
struct PIN_CH {
  Adafruit_PWMServoDriver DRIVER_NUM; 
  uint8_t PIN_NUM;
  bool isMotor;
};
PIN_CH CH[65];
const uint8_t CHANNEL[] = {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
                            48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
                            61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
                            74,
                            
                            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                            12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                            24};



const String NOTES[] = {"C-2", "C#-2", "D-2","D#-2", "E-2", "F-2", "F#-2", "G-2", "G#-2", "A-2", "A#-2", "B-2", 
                        "C-1", "C#-1", "D-1","D#-1", "E-1", "F-1", "F#-1", "G-1", "G#-1", "A-1", "A#-1", "B-1",
                        "C0", "C#0", "D0","D#0", "E0", "F0", "F#0", "G0", "G#0", "A0", "A#0", "B0",
                        "C1", "C#1", "D1","D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1",
                        "C2", "C#2", "D2","D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
                        "C3", "C#3", "D3","D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
                        "C4", "C#4", "D4","D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
                        "C5", "C#5", "D5","D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5",
                        "C6", "C#6", "D6","D#6", "E6", "F6", "F#6", "G6", "G#6", "A6", "A#6", "B6",
                        "C7", "C#7", "D7","D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "A#7", "B7",
                        "C8", "C#8", "D8","D#8", "E8", "F8", "F#8", "G8", "G#8", "A8", "A#8", "B8"};


// QUEUE & SERIAL MONITOR
QueueHandle_t xQueue;
uint8_t rx_state = 0;
byte cc_type1;
byte cc_type2;
byte cc_val1;
byte cc_val2;


// OLED DISPLAY VARIABLE
String DISPLAY_DATA[5][3]= {
                  {"","",""},
                  {"","",""},
                  {"","",""},
                  {"","",""},
                  {"","",""}};
const String MENU[] = {"Monitor", "Pin Config", "About"};
uint8_t SELECTED = 0;
bool MIDI_MONITOR = false;
u8g2_uint_t ROW_X = 10;
u8g2_uint_t ROW_Y = 10;
uint8_t currentState;
uint8_t lastState = HIGH;



int velocityToPwm(int velocity){
  return round(map(velocity, 0, 127, 0, 255));
}

void execute(uint8_t channel, uint8_t velocity){
  for(uint8_t i = 0; i < (sizeof(CHANNEL)/sizeof(CHANNEL[0])); i++){
    if(CHANNEL[i] == channel){
      if(CH[i].isMotor){
        CH[i].DRIVER_NUM.setPWM(CH[i].PIN_NUM, 0, map(velocity, 0, 127, 0, 4095));
      }
      else{
        if (velocity > 0 && velocity <= 64){
          CH[i].DRIVER_NUM.writeMicroseconds(CH[i].PIN_NUM, map(velocity, 0, 64, 1300, 1499));
        }
        else if (velocity > 64){
          CH[i].DRIVER_NUM.writeMicroseconds(CH[i].PIN_NUM, map(velocity, 65, 127, 1501, 1700));
        }
        else{
          CH[i].DRIVER_NUM.writeMicroseconds(CH[i].PIN_NUM, 1500);
        }
      }
    }
  }
}

int bytesToInt(int l_highByte, int l_lowByte) {
  return ((unsigned int)l_highByte << 8) + l_lowByte;
}


void lcd_display( void *pvParameters){
  // vTaskDelay(2000/portTICK_PERIOD_MS);
  pinMode(25, INPUT_PULLUP);
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
  u8g2.begin();
 
  for (;;){
    currentState = digitalRead(25);
    if(lastState == LOW && currentState == HIGH){
      // MIDI_MONITOR = !MIDI_MONITOR;
      SELECTED = SELECTED + 1;
      if (SELECTED > 2){
        SELECTED = 0;
      }
    }
    lastState = currentState;

    u8g2.clearBuffer();					// clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
    if(MIDI_MONITOR){
      // u8g2.drawStr(15, 10, "LONELY BINARY");	// write something to the internal memory
      u8g2.drawStr(10, 10, "Val");
      u8g2.drawStr(40, 10, "Not");
      u8g2.drawStr(70, 10, "Vel");
      for(int data = 0; data<5; data++){
          for(int row = 0; row<3; row++){
            String a = String(DISPLAY_DATA[data][row]);
             u8g2.drawStr(ROW_Y + (30 * row), ROW_X + (12 * (data+1)), a.c_str());
          }
        }
    }

    else {
      u8g2.drawStr(20, 10, "MENU");
      for(uint8_t i=0; i<3; i++){
        if (i == SELECTED){
          u8g2.drawStr(10,  10 * (i+2), ">");
        }
        u8g2.drawStr(20,  10 * (i+2), MENU[i].c_str());
      }
    }
    
    u8g2.sendBuffer();
    // vTaskDelay(1000);
  }
}

void broadcast_command( void *pvParameters ) {
  int ReceivedValue[] = { 0, 0 };
  String command = "";
  
  for (;;){
    if (xQueueReceive( xQueue, &ReceivedValue, portMAX_DELAY ) == pdPASS) {
      
      if (sizeof(DISPLAY_DATA) > 4){
        // int b[] = {count+1, count+5, count+5};
        for(uint8_t i = 4; i>0; i--){
          DISPLAY_DATA[i][0] = DISPLAY_DATA[i-1][0];
          DISPLAY_DATA[i][1] = DISPLAY_DATA[i-1][1];
          DISPLAY_DATA[i][2] = DISPLAY_DATA[i-1][2];
        }
        DISPLAY_DATA[0][0] = String(ReceivedValue[0]);
        DISPLAY_DATA[0][1] = NOTES[ReceivedValue[0]];
        DISPLAY_DATA[0][2] = String(ReceivedValue[1]);
      }

      Serial.print(255);
      Serial.print(" ");
      Serial.print(ReceivedValue[0]);  
      Serial.print(" ");
      Serial.print(highByte(ReceivedValue[1]));
      Serial.print(" ");
      Serial.println(lowByte(ReceivedValue[1]));

      execute(ReceivedValue[0], ReceivedValue[1]);
    }
  }
  vTaskDelete ( NULL );
}

void setup() {
  Serial.begin(38400);

  Wire.begin(SDA, SCL);
  Wire.setClock(400000);

  pinMode(LED_BUILTIN,OUTPUT);

  for(uint8_t i = 0; i<4; i++){
    PWM_DRIVER[i].begin();
    PWM_DRIVER[i].setOscillatorFrequency(25000000);
    PWM_DRIVER[i].setPWMFreq(SERVO_FREQ);
  }

  for (uint8_t driver_index = 0; driver_index < 4; driver_index++){
    for(uint8_t pin_index = 0; pin_index < 17; pin_index++){
      CH[(16 * driver_index) + pin_index] = { PWM_DRIVER[driver_index], pin_index, true};
    }
  }

  for (uint8_t i = 37; i < sizeof(CH)/sizeof(CH[0]); i++){
    CH[i].isMotor = false;
  }

  xQueue = xQueueCreate(100, sizeof(int[2]));
  if(xQueue != NULL){
    // xTaskCreatePinnedToCore(lcd_display, "lcd_display", 1024*6, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(broadcast_command, "broadcast_command", 1024*15, NULL, 4, NULL, 1);
  }
  // vTaskDelete(NULL);
}

void loop() {
  if (Serial.available()) {
    rx_state++;
    switch (rx_state) {
      case 1: 
        cc_type1 = Serial.read();
        if(cc_type1 != 255) {  
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
        int DATA_QUEUE[] = {control, value};
        xQueueSend( xQueue, &DATA_QUEUE, portMAX_DELAY);
    }
  }
}