#include <NewPing.h>
#include <SoftwareSerial.h>
#define TRIGGER_PIN 8
#define ECHO_PIN  7
#define MAX_DISTANCE 200
NewPing sonar(TRIGGER_PIN,ECHO_PIN,MAX_DISTANCE);
SoftwareSerial esp8266(2, 3); //Pin 2 & 3 of Arduino as RX and TX. Connect TX and RX of ESP8266 respectively.
#define DEBUG true




void setup() {
  //Motor A pin 9 y 10
  Serial.begin(9600);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  //Motor B pin 11 y 12
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);



  esp8266.begin(115200); //Baud rate for communicating with ESP8266. Your's might be different.
  esp8266Serial("AT+RST\r\n", 5000, DEBUG); // Reset the ESP8266
  esp8266Serial("AT+CWMODE=1\r\n", 5000, DEBUG); //Set station mode Operation
  esp8266Serial("AT+CWJAP=\"name of network\",\"password\"\r\n", 5000, DEBUG);//Enter your WiFi network's SSID and Password.

  while (!esp8266.find("OK"))
  {
  }
  esp8266Serial("AT+CIFSR\r\n", 5000, DEBUG);//You will get the IP Address of the ESP8266 from this command.
  esp8266Serial("AT+CIPMUX=1\r\n", 5000, DEBUG);
  esp8266Serial("AT+CIPSERVER=1,80\r\n", 5000, DEBUG);

}
void avanzar(){
  //Motor A
  digitalWrite(9,HIGH);
  digitalWrite(10,LOW);

  //Motor B
  digitalWrite(12,HIGH);
  digitalWrite(11,LOW);
  
}
void reversa(){
  //Motor A
  digitalWrite(9,LOW);
  digitalWrite(10,HIGH);

  //Motor B
  digitalWrite(12,LOW);
  digitalWrite(11,HIGH);
  
}
void detener(){
  //Motor A
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);

  //Motor B
  digitalWrite(11,LOW);
  digitalWrite(12,LOW);
}
void izquierda(){
  //Motor A
  digitalWrite(9,HIGH);
  digitalWrite(10,LOW);

  //Motor B
  digitalWrite(11,LOW);
  digitalWrite(12,LOW);
}
void derecha(){
  //Motor A
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);

  //Motor B
  digitalWrite(11,LOW);
  digitalWrite(12,HIGH);
}

void loop() {
    if (esp8266.available())
    {
      while(true){
        Serial.println("disponible");
        int ms = sonar.ping_median();
        Serial.println("Distancia");
        int distancia= (ms/US_ROUNDTRIP_CM);
        Serial.println(distancia);
        Serial.println(" cm");
        if (distancia<=30 and distancia>=0){
          detener();
          delay(1000);
          reversa();
          delay(1000);
          detener();
          delay(1000);
        }
        else
          if (esp8266.find("+IPD,"))
          {
            Serial.println("encontrado");
            String msg;   
             
            delay(100);
            esp8266.find("?");     
            msg = esp8266.readStringUntil(' ');
            Serial.println(msg);
            String command1 = msg.substring(0, 3);
            String command2 = msg.substring(4);
            Serial.println(command2);
            
            if ((command2=="ALTO") or (command2.substring(0,1)=="A") or(command2.substring(0,1)=="@")){
                avanzar();
                
            }
            else if ((command2=="BAJO") or (command2.substring(0,1)=="B") or(command2.substring(0,1)=="C")){
                reversa();
              
            }
            else if ((command2=="IZQUIERDA") or (command2.substring(0,1)=="I") or(command2.substring(0,1)=="i")){
                izquierda();
             
            }
            else if ((command2=="DERECHA") or (command2.substring(0,1)=="D") or(command2.substring(0,1)=="d")){
                derecha();
             
            }
            else if ((command2=="STOP") or (command2.substring(0,1)=="S") or(command2.substring(0,1)=="Z") or(command2.substring(0,1)=="") ){
                detener();
            
            }
         }    
      }
    }
}



String esp8266Serial(String command, const int timeout, boolean debug)
{
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      char c = esp8266.read();
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
//void rgb_col(int red, int green, int blue){
  //analogWrite(pin_red,red);//para recibir valores analogos
  //analogWrite(pin_green,green);
  //analogWrite(pin_blue,blue);
//}
