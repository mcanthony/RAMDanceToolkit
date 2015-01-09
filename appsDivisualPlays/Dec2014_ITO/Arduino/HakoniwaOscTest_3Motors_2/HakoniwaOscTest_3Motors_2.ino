#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>    

#include <OSCBundle.h>
#include <OSCBoards.h>

#include <Servo.h>

Servo sv[4];
int myservo_speed[4]; // how fast the servo moves, 1 = very fast, 10 = very slow
int myservo_movetime[4]; // next time in millis servo next moves
int myservo_gPos[4]; // target position to move towards
int myservo_cPos[4]; // current postion of servo
int cPos[4];       // current position
int gPos[4];       // goal position
int tDelay[4]; // delay between moves, gives appearance of smooth motion

EthernetUDP _udp;
byte _mac[] = { 0x90, 0xA2, 0xDA, 0x85, 0x28, 0x04 };
IPAddress _ip(192, 168, 20, 54);
const unsigned int _inPort = 12345;

void setup() 
{
  
  Ethernet.begin(_mac, _ip);
  _udp.begin(_inPort);
  
  for (int i = 0; i < 3; i++) {  
    pinMode(6+i, OUTPUT);
    sv[i].attach(i+6);  
    myservo_speed[i] = 1; // how fast the servo moves, 1 = very fast, 10 = very slow
    myservo_movetime[i] = 0; // next time in millis servo next moves
    myservo_gPos[i] = 0; // target position to move towards
    myservo_cPos[i] = 0; // current postion of servo
    tDelay[i] = 3; // delay between moves, gives appearance of smooth motion
  }
  
  Serial.begin(9600);

}

void loop()
{ 
  for (int i = 0; i < 3; i++) {
    cPos[i] = sv[i].read();
    
    if (i == 0) {
      Serial.println(myservo_movetime[i]);
    }
    
    //myservo_movetime[i]がオーバーフローしてます。マイナスの値になってしまうので、millis()を下回り停止していた模様。
    //おそらく起動から30秒過ぎたあとにmoveServoを呼び出すとビット溢れで止まります。
//  if (cPos[i] != gPos[i] && millis() >= myservo_movetime[i]) {
    //これで動作に問題なければ上の条件はいらないんじゃないかと思います。
    if (cPos[i] != gPos[i]) {
      moveServo(i);
    }
  }
  
   OSCMessage message;
   int size = _udp.parsePacket();
   
   if (size > 0) {
     while (size--) {
       message.fill(_udp.read());
     }
     
     if(!message.hasError() && message.match("/dp/hakoniwa/oscTest")) {
 //          digitalWrite(9, message.getInt(0) ? HIGH:LOW);
        for (int i = 0; i < 3; i++) {
          gPos[i] = message.getInt(i);
        }
//        Serial.println(pos[0]);
//
//        for (int i = 0; i < 3; i++) {
//          sv[i].write(pos[i]);
//        }
//      delay(5);
     }
   }

}

void moveServo(int which) {  
  if (cPos[which] < gPos[which]) sv[which].write(cPos[which]+1);
  if (cPos[which] > gPos[which]) sv[which].write(cPos[which]-1);
  //if (cPos == gPos) // nothing
  myservo_movetime[which] = millis() + tDelay[which];
}

