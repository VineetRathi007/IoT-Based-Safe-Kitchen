#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <espnow.h>
#include <Servo.h>
#include "NodeMcu_ESP8266_Gmail_Sender_by_FMT.h" 
#pragma region Globals
#pragma endregion Globals
WiFiClient client;
Servo servo;

String recipientID="vineet2018cs181abesit@gmail.com";
String subject="Alert Kitchen Fire";
String body="There is a gas leakage in your kitchen";

int LED = D1,smokeA0=A0,sensorThres=600;
int M2P1 =D5 ;  //GPIO0
int M2P2 = D6;  //GPIO2; 
int buzzer=D2;

//Thingspeak api
long myChannelNumber = 1669782;
const char myWriteAPIKey[] = "JHVKRSIGTOOL26O1";

void CustomInfo(){ // function to set custom settings
  //replace the below three lines with your information
  recipientID = (String)"vineet2018cs181abesit@gmail.com"; // enter the email ID of the recipient
  subject = (String)"Alert Kitchen Fire"; // enter the subject of the email
  body = (String)"There is a gas leakage in your kitchen"; // enter the body of the email
  /*--------------------------------------------------------------*/
}

void setup() {
  // put your setup code here, to run once:
  pinMode(smokeA0,INPUT);
   
  pinMode (M2P1,OUTPUT);
  pinMode (M2P2,OUTPUT);
  pinMode (buzzer,OUTPUT);

  
  Serial.begin(9600);
  WiFi.begin("Rathi", "rathi007");
  if(WiFi.status() == WL_CONNECTED)
  {
     delay(200);
     Serial.println();
     Serial.println("NodeMCU is connected!");
  }
 
  Serial.println(WiFi.localIP());

  //servo
  servo.attach(5); //D1
   
  ThingSpeak.begin(client);
   // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
}

void loop() {
  if(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("Connecting To Network");
  }
  // put your main code here, to run repeatedly:
  float h = analogRead(smokeA0);
  Serial.println(h);
  if(h>550)
  {
    digitalWrite(M2P1,HIGH);
    digitalWrite(buzzer,HIGH);
      digitalWrite(M2P2,LOW);
    servo.write(90);
     Serial.println("Window is Open");
      Gsender *gsender = Gsender::Instance();
    
    //InputInfo(); //inputs receiver's gmail id, subject and body of the email
    CustomInfo(); // uncomment this line if you want to send the email using the custom information which are already set in the code; and comment the above line.
    
    if(gsender->Subject(subject)->Send(recipientID, body)) { // sends the email using a single line function
        Serial.println("Email sent"); // message confirmation
        pinMode(LED_BUILTIN, OUTPUT);
    }
    delay(2000);
  }
  else{
    digitalWrite(buzzer,LOW);
    digitalWrite(M2P1,LOW);
    digitalWrite(M2P2,LOW);
    servo.write(0);
    Serial.println("Window is Closed");
    delay(2000);
    
  }
  ThingSpeak.writeField(myChannelNumber, 1, h, myWriteAPIKey);
  delay(2000);
}
