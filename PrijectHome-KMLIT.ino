#include <LiquidCrystal_I2C.h>
#include <Password.h>
#include <Keypad.h>
#include <Wire.h> 

const byte ROWS = 4; // key map rows
const byte COLS = 4; //  key map columns
const int statusLock = A0;
const int status_unlock = A1;
const int led_bed = 11;
const int led_bath = 12;
int motion = A2; 
int flame = A3;
int buzzer = 10;
int alarmValue = 0;  
int fl = 0;
int state = 0;
int state1 = 0;
int state2 = 0;
int Alarm_fl = 0;
int colPassA = 8, colPassB = 0;
String txtPass = "                    ";

// set lcd address 0x3f or 0x27 for a 20/4
#define I2C_ADDR 0x3f
LiquidCrystal_I2C lcd(I2C_ADDR,20,4);


// set keymap
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = { 2,3,4,5 };// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte colPins[COLS] = { 6,7,8,9 };// Connect keypad COL0, COL1, COL2 and COL3 to these Arduino pins.

// set keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// set password
Password password = Password("2356");

void setup()
{
    Serial.begin(9600);
    pinMode(statusLock, OUTPUT);
    pinMode(status_unlock, OUTPUT);
    pinMode(led_bed, OUTPUT); 
    pinMode(led_bath, OUTPUT);
    keypad.addEventListener(keypadEvent);
    // Sets First Screen LCD 
    lcd.begin();
    setFirstScreen();

}

void loop()
{
  keypad.getKey();
  onMotionAction();// Open motion Motion Sensor
  onFireAction();// Open Flame Detector Sensor
  setReserSystem();// Close Light1-2 and Set statusLock
}

void setFirstScreen(){
    lcd.backlight();
    lcd.print("* Hello Welcome To *");
    lcd.setCursor(0, 1);
    lcd.print("** PRUDENT n SAFE **");
    lcd.setCursor(0, 2);
    lcd.print("*Project KMITL-IT16*");
    lcd.setCursor(2, 3);
    lcd.print("** Home  Lock **");


}

void alarmOn() // Function Alarm.
{
  tone(buzzer, 10, 100); // Alarm at 10 Hz 100 milliseconds.
  delay(100); // delay 100 milliseconds
  tone(buzzer, 300, 100);// Alarm at 300 Hz 100 milliseconds.
  delay(100); // delay 100 milliseconds
}

void alarmOff() // Function Alarm off.
{
   digitalWrite(buzzer,HIGH); //Set buzzer pin is HIGH for stop alarm
}

void onMotionAction() // Function Open motion Motion Sensor.
{
  alarmValue = analogRead(motion); // Read analog sensor 0-1023.
  //Serial.println (alarmValue); // Debug Sensor value.
  if(alarmValue >= 600) // Check the value 
  {
    alarmOn();
  }
  if(alarmValue < 700)
  {
     alarmOff();
  }
}

void onFireAction() // Function Open Flame Detector Sensor.
{
  alarmValue = digitalRead(flame); // Read analog sensor 0-1023.
  //Serial.println (alarmValue);  // Debug Sensor value.
  if(alarmValue == 0)   // Check the value
  {
    alarmOn();
  }
  if(alarmValue == 1)
  {
    alarmOff();
  }
}

//take care of some special events
void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
    case PRESSED:
  Serial.print("Pressed: ");
  Serial.println(eKey);
        lcd.setCursor(colPassB, 3);
        lcd.print(txtPass);
        txtPass = "*";
        lcd.setCursor(colPassA, 3);
        lcd.print(txtPass);
        colPassB = colPassA;
  switch (eKey)
        {
               case 'A':
                 if(fl == 1)
                 {
                   if(state == 0)
                   {
                     digitalWrite(led_bed, HIGH); // Open the light 2
                     state = 1;
                   }
                   else if(state == 1)
                   {
                     digitalWrite(led_bed, LOW); // Close the light 2
                     state = 0;
                   }  
                 }
                break;
                case 'B':
                  if(fl == 1)
                 {
                   if(state1 == 0)
                   {
                     digitalWrite(led_bath, HIGH); // Open the light 1
                     state1 = 1;
                   }
                   else if(state1 == 1)
                   {
                     digitalWrite(led_bath, LOW); // Close the light 2
                     state1 = 0;
                   }  
                 }
                break;
                case 'C':
                  if(fl == 1)
                 {
                     state2 = 1;
                     lcd.setCursor(2, 3);
                     lcd.print("** Home  Lock **");
                     colPassA  = 8;
                     colPassB = 0;
                     txtPass = "                    ";
                 }
                break;
              case '#': 
                 state2 = 0;
                 checkPassword(); // Call the function CheckPassword
                 password.reset(); //Reset Key 
                 colPassA  = 8;
                 colPassB = 0;
                 txtPass = "                    ";
                 lcd.setCursor(0, 3);
                 lcd.print(txtPass);
              break;
              default: password.append(eKey);
              colPassA++;
     }
  }
}

void checkPassword(){
  if (password.evaluate()){
  //Add code to run if it works
    while(state2 == 0)
    { 
        //Serial.println("Success");
        fl = 1;
        digitalWrite(statusLock, LOW);
        digitalWrite(status_unlock, HIGH);
        lcd.setCursor(0, 3);
        lcd.print(" ** Home Un-Lock ** ");
        keypad.getKey();
        alarmOff();
        password.reset();
    }
  }
  else{
    Serial.println("Wrong");
    //add code to run if it did not work
    lcd.setCursor(0, 3);
    lcd.print("**Password Wrong **");
    delay(1000);
  }
}

void setReserSystem(){
  digitalWrite(statusLock, HIGH);
  digitalWrite(status_unlock, LOW);
  digitalWrite(led_bed, LOW);
  digitalWrite(led_bath, LOW);
}
