#include <WiFi.h>
#include <PubSubClient.h>

int sensitivity = 5; //lower values will make it more sensitive and higher values will make it less sensitive
//---------------------------------------------------

//MQTT
const char* NETWORK = "NETWORK";
const char* PASSWORD = "PASSWORD";
const char* MQTT_SERVER = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

int currentPeople = 0;

int sensor1[] = {18,19};
int sensor2[] = {2,0};
int sensor1Initial;
int sensor2Initial;

String sequence = "";

int timeoutCounter = 0;

void setup() {
  //Setup code
  Serial.begin(115200);
  
  WiFi.begin(NETWORK, PASSWORD);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.println("Couldn't connect to WiFi");
    delay(500);  
  }

  client.setServer(MQTT_SERVER, 1883);
  client.connect("espClient");

  Serial.println(WiFi.localIP());
  
  delay(500);
  sensor1Initial = measureDistance(sensor1);
  sensor2Initial = measureDistance(sensor2);
}

void loop() {
  //Read ultrasonic sensors
  int sensor1Val = measureDistance(sensor1);
  int sensor2Val = measureDistance(sensor2);

  client.setServer(MQTT_SERVER, 1883);
  client.connect("espClient");
  
  //Process the data
  if(sensor1Val < sensor1Initial - 30 && sequence.charAt(0) != '1'){
    sequence += "1";
  }else if(sensor2Val < sensor2Initial - 30 && sequence.charAt(0) != '2'){
    sequence += "2";
  }
  
  if(sequence.equals("12")){
    currentPeople++;  
    sequence="";
    delay(550);
  }else if(sequence.equals("21") && currentPeople > 0){
    currentPeople--;  
    sequence="";
    delay(550);
  }

  //Resets the sequence if it is invalid or timeouts
  if(sequence.length() > 2 || sequence.equals("11") || sequence.equals("22") || timeoutCounter > 200){
    sequence="";  
  }

  if(sequence.length() == 1){ //
    timeoutCounter++;
  }else{
    timeoutCounter=0;
  }

  //Print values to serial
  //Serial.print("Seq: ");
  //Serial.print(sequence);
  //Serial.print(" S1: ");
  //Serial.print(sensor1Val);
  //Serial.print(" S2: ");
  //jSerial.println(sensor2Val);
  client.publish("tsc/counter", String(currentPeople).c_str());
  Serial.println(currentPeople);
}
//Returns the distance of the ultrasonic sensor that is passed in
//a[0] = echo, a[1] = trig
int measureDistance(int a[]) {
  pinMode(a[1], OUTPUT);
  digitalWrite(a[1], LOW);
  delayMicroseconds(2);
  digitalWrite(a[1], HIGH);
  delayMicroseconds(10);
  digitalWrite(a[1], LOW);
  pinMode(a[0], INPUT);
  long duration = pulseIn(a[0], HIGH, 100000);
  return duration / 29 / 2;
}
