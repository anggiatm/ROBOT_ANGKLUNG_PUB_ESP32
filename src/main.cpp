#include <Arduino.h>
#include <ESP32Servo.h>

#include <U8g2lib.h>
#include <Wire.h>

#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm0 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x42);

Adafruit_PWMServoDriver PWM_DRIVER[] = {pwm0, pwm1, pwm2};


struct PIN_CH
{
  Adafruit_PWMServoDriver DRIVER_NUM; 
  int PIN_NUM;
};

PIN_CH CH[50];

PIN_CH CH0 = {pwm0, 0};




#define SERVOMIN  400 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  3900 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;


bool MIDI_MONITOR = false;

int demoMode = 0;
int counter = 1;

int ROW_X = 10;
int ROW_Y = 10;
int count = 0;

long timeSinceLastModeSwitch = 0;

QueueHandle_t xQueue;

long lastMsg = 0;
int rx_state = 0;

byte cc_type1;
byte cc_type2;
byte cc_val1;
byte cc_val2;


class Channel {
  private:
  /* data */
  public:
  u_int8_t note_number;
  u_int8_t channel_number;
  String note_string;

  Channel(/* args */);
  ~Channel();

  void setNoteNumber(u_int8_t note_number){
    Channel::note_number = note_number;
  }
  void setChannelNumber(u_int8_t channel_number){
    Channel::channel_number = channel_number;
  }

  uint8_t getNoteNumber(){
    return Channel::note_number;
  }
  uint8_t getChannelNumber(){
    return Channel::channel_number;
  }
};

Channel::Channel(/* args */)
{
}

Channel::~Channel()
{
}


int servoPin1 = 12;
int servoPin2 = 13;

String DISPLAY_DATA[5][3]= {
                  {"","",""},
                  {"","",""},
                  {"","",""},
                  {"","",""},
                  {"","",""}};

const String MENU[] = {"Monitor", "Pin Config", "About"};
uint8_t SELECTED = 0;

// Servo CHANNEL[] = {myservo};

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

int velocityToPwm(int velocity){
  return round(map(velocity, 0, 127, 0, 255));
}

void execute(int channel, int velocity){
  // myservo.write(velocityToPwm(velocity));
}


int bytesToInt(int l_highByte, int l_lowByte) {
  return ((unsigned int)l_highByte << 8) + l_lowByte;
}
int currentState;
int lastState = HIGH;
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
      for(int i=0; i<3; i++){
        if (i == SELECTED){
          u8g2.drawStr(10,  10 * (i+2), ">");
        }
        u8g2.drawStr(20,  10 * (i+2), MENU[i].c_str());
      }
    }
    
    u8g2.sendBuffer();					// transfer internal memory to the display
    // vTaskDelay(1000);
  }
}

void broadcast_command( void *pvParameters ) {
  int ReceivedValue[] = {0,0};
  String command = "";
  
  for (;;){
    if (xQueueReceive( xQueue, &ReceivedValue, portMAX_DELAY ) == pdPASS) {
      count = count + 1;
      
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

      Serial.print(255);
      Serial.print(" ");
      Serial.print(ReceivedValue[0]);  
      Serial.print(" ");
      Serial.print(highByte(ReceivedValue[1]));
      Serial.print(" ");
      Serial.println(lowByte(ReceivedValue[1]));

      if (ReceivedValue[0] == 47){
        execute(0, ReceivedValue[1]);
      }
    }
  }
  vTaskDelete ( NULL );
}

void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000000;  // convert input seconds to us
  pulse /= pulselength;
  Serial.println(pulse);
  pwm0.setPWM(n, 0, pulse);
}

void setup() {
  Serial.begin(38400);

  Wire.begin(SDA, SCL);
  Wire.setClock(400000);

  pinMode(LED_BUILTIN,OUTPUT);
  pwm0.begin();
  pwm0.setOscillatorFrequency(25000000);
  pwm0.setPWMFreq(SERVO_FREQ);

  pwm1.begin();
  pwm1.setOscillatorFrequency(25000000);
  pwm1.setPWMFreq(SERVO_FREQ);

  pwm2.begin();
  pwm2.setOscillatorFrequency(25000000);
  pwm2.setPWMFreq(SERVO_FREQ);

 

 

 

  xQueue = xQueueCreate(100, sizeof(int[2]));
  if(xQueue != NULL){
    // xTaskCreatePinnedToCore(lcd_display, "lcd_display", 1024*6, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(broadcast_command, "broadcast_command", 1024*15, NULL, 4, NULL, 1);
  }
  // vTaskDelete(NULL);
}

bool initial = false;

void loop() {

  // if (!initial){
    for(int i = 0; i<3; i++){
       for(int j = 0; j<16; j++){
         PIN_CH CH[(15*i)+j] = {PWM_DRIVER[i], j};
       }
     }
     initial = true;
  // }

  if (Serial.available()) {
    String n =Serial.readStringUntil('\n');
    int a = n.toInt();
    Serial.println(a);

    CH[0].DRIVER_NUM.writeMicroseconds(CH[0].PIN_NUM, a);

    // CH0.DRIVER_NUM.writeMicroseconds(CH0.PIN_NUM, a);

    // for (int i = 0; i<16; i++){
        // pwm0.writeMicroseconds(i, a);  //400 to 2700
        // pwm1.writeMicroseconds(i, a);  //400 to 2700
        // pwm2.writeMicroseconds(i, a);  //400 to 2700

        // CH[0].DRIVER_NUM.writeMicroseconds(CH[0].PIN_NUM, a);
        
      // }

    // pwm++;

    // if (pwm = 0){
    //   for (int i = 0; i<16; i++){
    //     pwm0.writeMicroseconds(i, a);  //400 to 2700
    //     // pwm1.writeMicroseconds(i, a);  //400 to 2700
    //     // pwm2.writeMicroseconds(i, a);  //400 to 2700
    //   }
    // }
    // else if (pwm = 1){
    //   for (int i = 0; i<16; i++){
    //     // pwm0.writeMicroseconds(i, a);  //400 to 2700
    //     pwm1.writeMicroseconds(i, a);  //400 to 2700
    //     // pwm2.writeMicroseconds(i, a);  //400 to 2700
    //   }
    // }

    // else if (pwm = 2){
    //   for (int i = 0; i<16; i++){
    //     // pwm0.writeMicroseconds(i, a);  //400 to 2700
    //     // pwm1.writeMicroseconds(i, a);  //400 to 2700
    //     pwm2.writeMicroseconds(i, a);  //400 to 2700
    //   }
    // }

    // else {
    //   pwm = 0;
    // }
    

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
    }
  }
}