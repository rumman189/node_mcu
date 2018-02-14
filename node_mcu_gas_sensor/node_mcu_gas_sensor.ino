
#include <ESP8266WiFi.h>
//#include <OneWire.h>
//#include <PubSubClient.h>

const char *ssid =  "DataSoft_WiFi";        //Your Access Point or Personal Hotspot, cannot be longer than 32 characters!
const char *pass =  "support123";    //Your Access Point or Personal Hotspot password
const char* serverTS = "api.thingspeak.com";
String apiKey = "6TFFNWZ6T97FB2DC";          //Insert your Channel API Key here
//const int pingPin = 2;          //Ultrasonic connected to GPIO0
int TRIGGER = 5; //Pin D1 = TRIGGER
int ECHO = 4; //Pin D2 = ECHO


void setup() 
{
  //pinMode(0,OUTPUT);            //LED connected to GPIO2
  Serial.begin(115200);         //Recommended speed is 115200
  pinMode(TRIGGER,OUTPUT);
  pinMode(ECHO,INPUT);
  connectWifi();
  }

void loop() 
{
  long duration, inches, cm;

  duration = pulseIn(ECHO, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);


//---Take analog readings
  float sensorVoltage; 
  float sensorValue;
  sensorValue = analogRead(A0);
  sensorVoltage = sensorValue/1024*3.3;
 
  Serial.print("sensor voltage = ");
  Serial.print(sensorVoltage);
  Serial.println(" V");
  delay(1000);

//
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(1000);
 sendHeight(sensorVoltage);
}
void connectWifi()
{ 
  Serial.print("Connecting to "+*ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");  
}//end connect

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

 void sendHeight(float sensorVoltage)
{  
  WiFiClient tclient;//not to be confused with "client" in PubSub{}, and wclient for mqtt
   if (tclient.connect(serverTS, 80)) { // use ip 184.106.153.149 or api.thingspeak.com
   //Serial.println("WiFi Client connected ");
   
   String postStr = apiKey;
   postStr += "&field1=";
   postStr += String(sensorVoltage);
    postStr += "&field2=";
    postStr += String(sensorVoltage);
   postStr += "\r\n\r\n";
   
   tclient.print("POST /update HTTP/1.1\n");
   tclient.print("Host: api.thingspeak.com\n");
   tclient.print("Connection: close\n");
   tclient.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   tclient.print("Content-Type: application/x-www-form-urlencoded\n");
   tclient.print("Content-Length: ");
   tclient.print(postStr.length());
   tclient.print("\n\n");
   tclient.print(postStr);
   delay(1000);
   
   }//end if

 tclient.stop();
}//end send to ts

