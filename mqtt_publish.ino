/*
 * MQTT and AskSensors IoT Platform
 * Description: Arduino Ethernet publishes data to AskSensors using MQTT
 *  Author: https://asksensors.com, 2019
 *  github: https://github.com/asksensors
 */
 
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// TODO: Ethernet MQTT user config
const char* username = "................."; // my AskSensors username
const char* pubTopic = "................../.................."; // username/apiKeyIn
const unsigned int writeInterval = 25000;   // write interval (in ms)

//AskSensors MQTT config
const char* mqtt_server = "mqtt.asksensors.com";
unsigned int mqtt_port = 1883;

// ETHERNET config.
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(..., ..., ..., ...); // TODO: Add the IP address

EthernetClient askClient;
PubSubClient client(askClient);

void setup() {
  Serial.begin(115200);
  Serial.println("*****************************************************");
  Serial.println("********** Program Start : Arduino Ethernet publishes data to AskSensors over MQTT");
  Serial.print("********** connecting to Ethernet : ");
    // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) 
    reconnect();
  client.loop();
  Serial.println("********** Publish MQTT data to ASKSENSORS");
  char mqtt_payload[30] = "";
  snprintf (mqtt_payload, 30, "module1=%ld", random(10,100));
  Serial.print("Publish message: ");
  Serial.println(mqtt_payload);
  client.publish(pubTopic, mqtt_payload);
  Serial.println("> MQTT data published");
  Serial.println("********** End ");
  Serial.println("*****************************************************");
  
 delay(writeInterval);// delay
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("********** Attempting MQTT connection...");
    // Attempt to connect
   if (client.connect("ethClient", username, "")) { 
      Serial.println("-> MQTT client connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("-> try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
