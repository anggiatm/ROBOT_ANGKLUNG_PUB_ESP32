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

long lastMsg = 0;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
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
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(callback);
  pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
     
    char message[8];
    dtostrf(99, 1, 2, message);
    Serial.print("Data: ");
    Serial.println(message);
    client.publish("tes", message);
  }
}