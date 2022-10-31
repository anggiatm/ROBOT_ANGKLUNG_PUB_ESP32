// #include <WiFi.h>
#include <Arduino.h>
#include <ESP32Servo.h>
// #include <PubSubClient.h>

// WiFiClient   wifiClient;
// #define MQTT_BROKER "192.168.1.7"
// #define MQTT_PORT 1883
// #define SSID "wifi_rumah"
// #define PASS "mora170894"
// #define ID "ROBOT_PUBLISHER"

// IPAddress IP = {192,168,1,7};
// PubSubClient MQTTclient( IP, MQTT_PORT, wifiClient );
// SemaphoreHandle_t sema_MQTT_KeepAlive;

#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
// OR #include "SH1106Wire.h"   // legacy: #include "SH1106.h"

// For a connection via I2C using brzo_i2c (must be installed) include:
// #include <brzo_i2c.h>        // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Brzo.h"
// OR #include "SH1106Brzo.h"

// For a connection via SPI include:
// #include <SPI.h>             // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"
// OR #include "SH1106SPi.h"

// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
// SSD1306Wire display(0x3c, D3, D5);  // ADDRESS, SDA, SCL  -  If not, they can be specified manually.
// SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_32);  // ADDRESS, SDA, SCL, OLEDDISPLAY_GEOMETRY  -  Extra param required for 128x32 displays.
// SH1106Wire display(0x3c, SDA, SCL);     // ADDRESS, SDA, SCL

// Initialize the OLED display using brzo_i2c:
// SSD1306Brzo display(0x3c, D3, D5);  // ADDRESS, SDA, SCL
// or
// SH1106Brzo display(0x3c, D3, D5);   // ADDRESS, SDA, SCL

// Initialize the OLED display using SPI:
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi display(D0, D2, D8);  // RES, DC, CS
// or
// SH1106Spi display(D0, D2);       // RES, DC


#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;

int ROW_X = 10;
int ROW_Y = 10;
int count = 0;
String DISPLAY_DATA[5][3]= {
                  {"","",""},
                  {"","",""},
                  {"","",""},
                  {"","",""},
                  {"","",""}};

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




void drawFontFaceDemo() {
  // Font Demo1
  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Hello world");
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "Hello world");
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 26, "Hello world");
}

void drawTextFlowDemo() {
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawStringMaxWidth(0, 0, 128,
                             "Lorem ipsum\n dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore." );
}

void drawTextAlignmentDemo() {
  // Text alignment demo
  display.setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 10, "Left aligned (0,10)");

  // The coordinates define the center of the text
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 22, "Center aligned (64,22)");

  // The coordinates define the right end of the text
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 33, "Right aligned (128,33)");
}

void drawRectDemo() {
  // Draw a pixel at given position
  for (int i = 0; i < 10; i++) {
    display.setPixel(i, i);
    display.setPixel(10 - i, i);
  }
  display.drawRect(12, 12, 20, 20);

  // Fill the rectangle
  display.fillRect(14, 14, 17, 17);

  // Draw a line horizontally
  display.drawHorizontalLine(0, 40, 20);

  // Draw a line horizontally
  display.drawVerticalLine(40, 0, 20);
}

void drawCircleDemo() {
  for (int i = 1; i < 8; i++) {
    display.setColor(WHITE);
    display.drawCircle(32, 32, i * 3);
    if (i % 2 == 0) {
      display.setColor(BLACK);
    }
    display.fillCircle(96, 32, 32 - i * 3);
  }
}

void drawProgressBarDemo() {
  int progress = (counter / 5) % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");
}


Demo demos[] = {drawFontFaceDemo, drawTextFlowDemo, drawTextAlignmentDemo, drawRectDemo, drawCircleDemo, drawProgressBarDemo};
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;

QueueHandle_t xQueue;

long lastMsg = 0;
int rx_state = 0;


byte cc_type1;
byte cc_type2;
byte cc_val1;
byte cc_val2;

Servo myservo;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32

// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
// Possible PWM GPIO pins on the ESP32-S2: 0(used by on-board button),1-17,18(used by on-board LED),19-21,26,33-42

int servoPin = 12;

int bytesToInt(int l_highByte, int l_lowByte) {
  return ((unsigned int)l_highByte << 8) + l_lowByte;
}

void broadcast_command( void *pvParameters )
{
  int ReceivedValue[] = {0,0};
  String command = "";
  // while ( !MQTTclient.connected() )
  // {
  //   vTaskDelay( 250 );
  // }
  for (;;)
  {
    if (xQueueReceive( xQueue, &ReceivedValue, portMAX_DELAY ) == pdPASS){
      
      count = count + 1;
      display.clear();
      display.setFont(ArialMT_Plain_10);
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.drawString(10, 0, "Val");
      display.drawString(40, 0, "Not");
      display.drawString(70, 0, "Vel");
      
      if (sizeof(DISPLAY_DATA) > 4){
        // int b[] = {count+1, count+5, count+5};
        for(int i = 4; i>0; i--){
          DISPLAY_DATA[i][0] = DISPLAY_DATA[i-1][0];
          DISPLAY_DATA[i][1] = DISPLAY_DATA[i-1][1];
          DISPLAY_DATA[i][2] = DISPLAY_DATA[i-1][2];
        }
        DISPLAY_DATA[0][0] = String(ReceivedValue[0]);
        DISPLAY_DATA[0][1] = NOTES[ReceivedValue[0]];
        DISPLAY_DATA[0][2] = String(ReceivedValue[1]);
  }


  for(int data = 0; data<5; data++){
    for(int row = 0; row<3; row++){
      display.drawString(ROW_Y + (30 * row), ROW_X + (10 * data), String(DISPLAY_DATA[data][row]));
    }
  }

  // display.drawString(60, 50, String(count));
  display.display();

      Serial.print(255);
      Serial.print(" ");
      Serial.print(ReceivedValue[0]);  
      Serial.print(" ");
      Serial.print(highByte(ReceivedValue[1]));
      Serial.print(" ");
      Serial.println(lowByte(ReceivedValue[1]));
      // command = String(ReceivedValue[0]) + " " + String(ReceivedValue[1]);

      // xSemaphoreTake( sema_MQTT_KeepAlive, portMAX_DELAY );
      // if ( MQTTclient.connected() )
      // {
      //   MQTTclient.publish( "tes", command.c_str());
      // }
      // xSemaphoreGive( sema_MQTT_KeepAlive );
    }
  }
  vTaskDelete ( NULL );
}


void setup() {
  Serial.begin(38400);
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  pinMode(LED_BUILTIN,OUTPUT);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
  delay(500);
  myservo.write(0);
  delay(500);
  // sema_MQTT_KeepAlive   = xSemaphoreCreateBinary();
  // xSemaphoreGive( sema_MQTT_KeepAlive ); // found keep alive can mess with a publish, stop keep alive during publish

  xQueue = xQueueCreate(100, sizeof(int[2]));
  if(xQueue != NULL){
    // xTaskCreatePinnedToCore( MQTTkeepalive, "MQTTkeepalive", 1024*15, NULL, 3, NULL, 0 );
    xTaskCreatePinnedToCore( broadcast_command, "broadcast_command", 1024*15, NULL, 4, NULL, 1);
  }
}




void loop() {
  

  // delay(10);
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
        int a[] = {control, value};
        xQueueSend( xQueue, &a, portMAX_DELAY);

        // Serial.print(255);
        // Serial.print(" ");
        // Serial.print(control);  
        // Serial.print(" ");
        // Serial.print(highByte(value));
        // Serial.print(" ");
        // Serial.println(lowByte(value));
  
        // if(value > 90 ){
        //   digitalWrite(LED_BUILTIN, HIGH);
        //   myservo.write(180);   
        // }
        // else {
        //   digitalWrite(LED_BUILTIN, LOW);
        //   myservo.write(0);
        // }
    }
    }
}