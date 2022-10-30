#include <WiFi.h>
#include <PubSubClient.h>

// PARAMETER WAJIB
#define SSID "wifi_rumah"
#define PASS "mora170894"
#define MQTT_BROKER "192.168.1.7"
#define MQTT_PORT 1883
#define ID "PUBLISHER"

// ROBOT TOPIC COM = COMMAND | DEB = DEBUG
#define TOPIC_1_COM "r1/c"
#define TOPIC_1_DEB "r1/d"
#define TOPIC_2_COM "r2/c"
#define TOPIC_2_DEB "r2/d"
#define TOPIC_3_COM "r3/c"
#define TOPIC_3_DEB "r3/d"

WiFiClient wifiClient;
PubSubClient client(wifiClient);

TaskHandle_t watch_midi_input_handle;
TaskHandle_t send_midi_output_handle;
QueueHandle_t xQueue;

long lastMsg = 0;
int rx_state = 0;


byte cc_type1;
byte cc_type2;
byte cc_val1;
byte cc_val2;

void setup_wifi() {
  vTaskDelay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(LED_BUILTIN, LOW);
    }
  // }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(ID)) {
      Serial.println("connected");
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


int bytesToInt(int l_highByte, int l_lowByte) {
  return ((unsigned int)l_highByte << 8) + l_lowByte;
}


void send_midi_output( void *pvParameters ){
  vTaskDelay(5000);
  int ReceivedValue[] = {0,0};
  for( ;; ) {
    if (xQueueReceive( xQueue, &ReceivedValue, portMAX_DELAY ) == pdPASS){
      Serial.print(255);
      Serial.print(" ");
      Serial.print(ReceivedValue[0]);  
      Serial.print(" ");
      Serial.print(highByte(ReceivedValue[1]));
      Serial.print(" ");
      Serial.println(lowByte(ReceivedValue[1]));
    
    }
    
    taskYIELD();
  }
}

void mqtt( void *pvParameters ){
  setup_wifi();
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(callback);
  for( ;; ) {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
    taskYIELD();
  }
}

void setup() {
  Serial.begin(38400);
  
  pinMode(LED_BUILTIN, OUTPUT);

  xQueue = xQueueCreate(100, sizeof(int[2]));
  if(xQueue != NULL){
    xTaskCreatePinnedToCore(send_midi_output, "SEND MIDI OUTPUT", 1024*2, NULL, 1, NULL, -1);
    xTaskCreatePinnedToCore(mqtt, "mqtt", 1024*2, NULL, 1, NULL, -1);
    vTaskStartScheduler();
  }
}


void loop() {
  if (Serial.available()) {
      client.publish("tes","hello world");
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

        // Serial.print(255);
        // Serial.print(" ");
        // Serial.print(control);  
        // Serial.print(" ");
        // Serial.print(highByte(value));
        // Serial.print(" ");
        // Serial.println(lowByte(value));
        int a[] = {control, value};

        xQueueSend( xQueue, &a, portMAX_DELAY);
        break;
    }
  }
  
}