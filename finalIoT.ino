/*GAS detector security system.
 */

#include <LiquidCrystal_I2C.h>  //The library allows to control I2C displays
#define BLYNK_PRINT Serial //It is just a redirection of built in Blynk “status prints” #include <ESP8266WiFi.h>  //This library provides ESP8266 specific Wi-Fi routines 
#include <BlynkSimpleEsp8266.h> 
LiquidCrystal_I2C lcd(0x27, 16, 2); //set address I2C and the size of lcd 16×2 char

//char auth[] = " ";// Enter your Auth token
char auth[] = "…";
char ssid[] = "…";   //Wi-Fi name
char pass[] = "…"; //Wi-Fi password
BlynkTimer timer;  //to send data in intervals and keep the void loop()
int pinValue = 0;  

#define Buzzer D5
#define Green D6
#define Red D7
#define Sensor A0

void setup() {
  Serial.begin(115200);//Sets the data rate in bits per second (baud) for serial data transmission. 
  lcd.backlight();  //turn on the backlight
  lcd.begin();  //Initializes the interface to the LCD screen
  pinMode(Green, OUTPUT);  
  pinMode(Red, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Sensor, INPUT);
  Blynk.begin(auth, ssid, pass); //connect to set auth token, Wi-Fi
  timer.setInterval(100L, notifiaction);
}
BLYNK_WRITE(V0) { //The device can send data to the App using Blynk.
  pinValue = param.asInt();
}

void notifiaction() { //function for notification
  int sensor = analogRead(Sensor);
  Serial.println(sensor);
  sensor = map(sensor, 0, 1024, 0, 100);
  if (pinValue == 1) { //if button on
    if (sensor <= 50) { //gas level below 50
      digitalWrite(Green, HIGH);  //green led on
      digitalWrite(Red, LOW);  //red led off
      digitalWrite(Buzzer, LOW);  //buzzer off
      lcd.setCursor(0, 1);   //set character in second row
      lcd.print("Gas value:Normal"); //display in lcd
    } else if (sensor > 50) {
      Blynk.notify("Warning! Gas leak detected");
      digitalWrite(Green, LOW);
      digitalWrite(Red, HIGH);
      digitalWrite(Buzzer, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Gas value:High  ");
    }
    lcd.setCursor(0, 0);   //set character in first row
    lcd.print("Value    :");
    lcd.print(sensor); //call the sensor value and display 
    Blynk.virtualWrite(V1, sensor);
  } else { //if the button off
    digitalWrite(Red, LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(Green, LOW);
    lcd.clear();
  }
}

void loop() {
  Blynk.run();
  timer.run();
}
