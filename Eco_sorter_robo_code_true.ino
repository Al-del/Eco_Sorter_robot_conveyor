#include "WiFiS3.h"
#include <Servo.h>
#include <ArduinoJson.h>
#include <ArduinoMqttClient.h>
#include <Servo.h>
Servo myservo;  // create servo object to control a servo

char ssid[] = "ok";    // your network SSID (name)
char pass[] = "sajp1434";    // your network password 

char mqtt_user[] = "Eco";
char mqtt_pass[] = "Sorter";


Servo servo;
const char* host = "192.168.251.226";         // server address
const int httpPort = 5000;                  // server port
 String line;
 int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 3;
int in3 = 5;
int in4 = 4;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.emqx.io";
int        port     = 1883;
const char subscribe_topic[]  = "/belt";
const char publish_topic[]  = "/hello/world";

void setup() {
  Serial.begin(115200);
  	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  delay(10);
servo.attach(6);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

    while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("You're connected to the network");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mqttClient.setUsernamePassword(mqtt_user, mqtt_pass);

  Serial.print("Attempting to connect to the MQTT broker.");

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }


  Serial.println("You're connected to the MQTT broker!");

  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(subscribe_topic);

  // subscribe to a topic
  mqttClient.subscribe(subscribe_topic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(subscribe_topic);
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object

}

void loop() {

  mqttClient.poll();

  // send message, the Print interface can be used to set the message contents
  delay(3000);

}
String conn(String path){

  WiFiClient client;
  while (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    Serial.println(WiFi.status());

  }

  // We now create a URI for the request
  String url = path;

  // This will send the request to the server
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return "err";
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    line = client.readStringUntil('\r');
    
  }
return line;
}
void in_spate() {
	// Set motors to maximum speed
	// For PWM maximum possible values are 0 to 255
	analogWrite(enA, 255);
	analogWrite(enB, 255);

	// Turn on motor A & B
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	delay(1850);
	

	
	// Turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}
void in_fata() {
		// Set motors to maximum speed
	// For PWM maximum possible values are 0 to 255
	analogWrite(enA, 240);
	analogWrite(enB, 240);
Serial.println("spate");
	// Turn on motor A & B
	digitalWrite(in1, HIGH); 
	digitalWrite(in2, LOW); 
	digitalWrite(in3, LOW); 
	digitalWrite(in4, HIGH); 
	delay(1850);
	

	
	// Turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}

void onMqttMessage(int messageSize) {
  Serial.print("Received a message with topic '");
  Serial.println(mqttClient.messageTopic());
  StaticJsonDocument<256> doc;
  deserializeJson(doc, mqttClient);
  const char* message = doc["message"];
  Serial.print("Message: '");
  Serial.println(message);
  Serial.println((int)message);
  if ((int) message >= 536880000){
    handle_belt();
  }
}
void handle_belt(){

 //GO FORWARD
 int sum;
      Serial.println("FORWARD");
       // in_fata();
    
      line = conn("/detect");
      Serial.println(line);
      sum = 0;
      for (int i = 0; i < line.length(); i++) {
        sum+=(int)line[i];
      }
      Serial.println(sum);
      if(sum == 762){
        //PL  ASTIC
        myservo.write(0);
        in_spate();
        in_spate();
        in_spate();
        in_spate();
        in_spate();
        in_spate();
        in_spate();
        in_spate();
        myservo.write(90);
        Serial.println("FHHJSF");
        line = conn("/move_brat");
    //CONECT MANA
      }else{
        //PAPER

        Serial.println("FORWARD PAPER");
       myservo.write(180);
      in_spate();
      in_spate();
      in_spate();
      in_spate();
       myservo.write(90);
      }

}