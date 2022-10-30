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


void setup()
{
  Serial.begin(38400);
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
  
        if(value > 90 ){
          digitalWrite(LED_BUILTIN, HIGH);
          myservo.write(180);   
        }
        else {
          digitalWrite(LED_BUILTIN, LOW);
          myservo.write(0);
        }
    }
    }
}