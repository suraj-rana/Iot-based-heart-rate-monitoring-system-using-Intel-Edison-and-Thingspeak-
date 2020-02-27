#include <WiFi.h>

char ssid[] = "newt";     //  your wifi network SSID (name) 
char pass[] = "123456780";  // your wifi network password

WiFiClient client;
int status = WL_IDLE_STATUS;     // the Wifi radio's status

const char* host = "api.thingspeak.com"; //thingspeak cloud
const char* apiKey="TJKLMTHNGOSDLUCJ"; //thingspeak channel API KEY
const char* resource = "/update?api_key=";

const int httpPort = 80;

//  Variables
unsigned long time1=0; // store the initial time
unsigned long time2; // store the current time
int LED13 = 13;   //  The on-board Arduion LED
int count=0;
int state= 0;
int laststate= 0;

int Signal;                // holds the incoming raw data. Signal value can range from 0-1024
int Threshold = 550;            // Determine which Signal to "count as a beat", and which to ingore. 


// The SetUp Function:
void setup() {
  pinMode(LED13,OUTPUT);         // pin that will blink to your heartbeat!
  Serial.begin(9600);         // Set's up Serial Communication at certain speed.   

  String fv = WiFi.firmwareVersion();
  if( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");
  
 // attempt to connect to Wifi network:
 
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    
    // Connect to WPA/WPA2 network:    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
     }
   
  // you're connected now, so print out the data:

  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();
   if (!client.connect(host, httpPort))
    {
      Serial.println("connection to ThingSpeak failed");
      return;
    }
}

// The Main Loop Function
void loop() {
  if(count==0) {
    time1=millis();
  }
    time2=millis();

  Signal = analogRead(A0);  // Read the PulseSensor's value. 
                                              
    if(Signal > Threshold){                          // If the signal is above "550", then "turn-on" Arduino's on-Board LED.  
     digitalWrite(LED13,HIGH);
     state=1;          
   } else {
     digitalWrite(LED13,LOW);
     state=0;//  Else, the sigal must be below "550", so "turn-off" this LED.
   }

   if (state != laststate) {
    // if the state has changed, increment the counter
    if (state == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      count++;
    } 
    else {
      // if the current state is LOW then the button went from on to off:
      //Serial.println("off");
    }
    delay(50);
   }
    laststate=state;

      if(time2>=time1+15000) {
    
  count=count*4;
  Serial.print("Heartbeat rate is ");
  Serial.print(count);
  Serial.print(" per min\n");

    client.print(String("GET ") + resource + apiKey + "&field1=" + String(count) +
                  " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" + 
                  "Connection: open\r\n\r\n");
  
  time1=0;
  time2=0;
  count=0;
  }  
}
void printWifiData() {
  
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
  Serial.println(ip);
  
  // print your MAC address:
  byte mac[6];  
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
 
}

void printCurrentNet() {
  
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);    
  Serial.print("BSSID: ");
  Serial.print(bssid[5],HEX);
  Serial.print(":");
  Serial.print(bssid[4],HEX);
  Serial.print(":");
  Serial.print(bssid[3],HEX);
  Serial.print(":");
  Serial.print(bssid[2],HEX);
  Serial.print(":");
  Serial.print(bssid[1],HEX);
  Serial.print(":");
  Serial.println(bssid[0],HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption,HEX);
  Serial.println();
}
