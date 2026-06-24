#include <WiFi.h>
#include <PubSubClient.h>
#include <devices/Led.h>
#include <devices/Sonar.h>
#define MSG_BUFFER_SIZE 50

Led* greenLed = new Led(2);
Led* redLed = new Led(3);
Sonar* pSonar = new Sonar(12,11,150000);

/* wifi network info */

const char* ssid = "OPPO A73 5G";
const char* password = "daniele04";

/* MQTT server address */
const char* mqtt_server = "broker.mqtt-dashboard.com";

/* MQTT topic */
const char* topic = "ESP-Project";

/* MQTT client management */

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsgTime = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
    delay(10);
    Serial.println(String("Connecting to ") + ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

/* MQTT subscribing callback */
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.println(String("Message arrived on [") + topic + "] len: " + length + " txt: " + String((char*)payload, length) );
}

void reconnect() {

    //Loop until we're reconnected

    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");

        //Create a random client ID
        String clientId = String("ESP-Project-") + String(random(0xffff), HEX);

        //Attempt to connect
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            client.subscribe(topic);
        } else {
            Serial.println("failed, rc=");
            Serial.println(client.state());
            Serial.println(" try again in 5 seconds");
            //wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    setup_wifi();
    randomSeed(micros());
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop() {

    if (!client.connected()) {
        greenLed->switchOff();
        redLed->switchOn();
        reconnect();
    } else {
        redLed->switchOff();
        greenLed->switchOn();
    }
    client.loop();

    unsigned long now = millis();
    if (now - lastMsgTime > 10000) {
        lastMsgTime = now;
        value = pSonar->getDistance() || "10";

        /* creating a msg in the buffer */
        snprintf(msg, MSG_BUFFER_SIZE, "Valore acqua: %d", value);

        Serial.println(String("Publishing message: ") + msg);

        /* publishing the msg*/
        client.publish(topic, msg);
    }
}