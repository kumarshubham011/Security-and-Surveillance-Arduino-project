#include<LiquidCrystal_I2C.h>
#include<Wire.h>
#include <Servo.h>
#include <Keypad.h>
Servo myservo;
int pos=0;
const byte rows=4;
const byte cols=3;
 
char key[rows][cols]={
                        {'1','2','3'},
                        {'4','5','6'},
                        {'7','8','9'},
                        {'*','0','#'}
                     };
byte rowPins[rows] = {A3 , A2 , A1 , A0};
byte colPins[cols] = {11 , 12 , 13};
Keypad keypad= Keypad(makeKeymap(key),rowPins,colPins,rows,cols);
char* password="0123";
int currentposition=0;

 

int ledPin = 6;
int flame_Sensor=5;
int gas_Sensor=4;
int speaker = 8;
int pin = 10;
int term_1 = 3;
int term_2 = 2;

LiquidCrystal_I2C lcd(0x20 , 16 , 2);
LiquidCrystal_I2C lcd_1(0x27 , 16 , 2);

void setup() 
{
  Serial.begin(9600);
  myservo.attach(9);
  lcd.begin(16 ,2);
  lcd.backlight();
  lcd_1.begin(16 , 2);
  lcd.backlight();
  pinMode (ledPin,OUTPUT);
  pinMode (speaker,OUTPUT);
  pinMode (pin,OUTPUT);
  pinMode (flame_Sensor,INPUT);
  pinMode (gas_Sensor,INPUT);
  pinMode (term_1 , OUTPUT);
  pinMode (term_2 , OUTPUT);
}

void loop() 
{
    actpir();
    pirSensor();
    flameSensor();
    gasSensor();
    doorlock();
}

void doorlock()
{
    if( currentposition==0)
      displayscreen();
    char code=keypad.getKey();
    if(code!=NO_KEY)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("PASSWORD:");
      lcd.setCursor(7,1);
      lcd.print(" ");
      lcd.setCursor(7,1);
      for(int i=0; i<=currentposition; ++i)
         lcd.print("*");
      if (code==password[currentposition])
      {
          ++currentposition;
          if(currentposition==4)
          {
              unlockdoor();
              currentposition=0;
          }
        }
       else
       {
          incorrect();
          currentposition=0;
       }
     }
}

void normalmode()
{
   lcd_1.setCursor(0,0);
   lcd_1.print("SYSTEM'S ACTIVE!");
   lcd_1.setCursor(0,1);
   lcd_1.print("All Clear  :)");
   lcd_1.setCursor(17,3);
   lcd_1.print("Good Day ..");
   delay(100);
   digitalWrite(ledPin,LOW);
   digitalWrite(speaker,LOW);
}

void pirSensor()
{    
     int pirstate = digitalRead(7);
     if (pirstate == LOW)
     {
         normalmode();
     }
     else if(pirstate == HIGH)
     {
         lcd_1.clear();
         digitalWrite(ledPin,HIGH);
         digitalWrite(speaker,HIGH);
         lcd_1.setCursor(0,0);
         lcd_1.print("SYSTEM'S ACTIVE!");
         lcd_1.setCursor(0,1);
         lcd_1.print("MOTION DETECTED");
         lcd_1.setCursor(17,3);
         char text[] = "SENDING SMS !!";
         unsigned int i = 0;
         while(text[i] != '\0')
         {
          lcd_1.print(text[i]);
          if(i>=14)
             lcd_1.command(0x18);
          delay(50);
          i++ ;
         }
         Serial.println("CODE RED !!!!! ");
         Serial.println("INTRUDER IN THE HOUSE");
         Serial.println("\n");   
         delay(1000);
         lcd_1.clear();
     }
}

void flameSensor()
{
  flame_Sensor = digitalRead(5);
  if (flame_Sensor == LOW)
     {
         normalmode();
     }
   else if(flame_Sensor == HIGH)
   {
         lcd_1.clear();
         digitalWrite(ledPin,HIGH);
         digitalWrite(speaker,HIGH);
         digitalWrite(flame_Sensor,HIGH);
         lcd_1.setCursor(0,0);
         lcd_1.print("SYSTEM'S ACTIVE!");
         lcd_1.setCursor(0,1);
         lcd_1.print("FLAME WARNING !!");
         lcd_1.setCursor(16,3);
         char text[] = "SENDING SMS !!";
         unsigned int i = 0;
         while(text[i] != '\0')
         {
          lcd_1.print(text[i]);
          if(i>=14)
             lcd_1.command(0x18);
          delay(50);
          i++ ;
         }
         Serial.println("WARNING !!!!! ");
         Serial.println("FIRE IN THE HOUSE");
         Serial.println("\n");
         delay(1000);
         lcd_1.clear();
   }
}

void gasSensor()
{
  gas_Sensor=digitalRead(4);
  if (gas_Sensor == LOW)
     {
         normalmode();
     }
   else if(gas_Sensor == 1)
   {
         lcd_1.clear();
         digitalWrite(ledPin,HIGH);
         digitalWrite(speaker,HIGH);
         digitalWrite(gas_Sensor,HIGH);
         digitalWrite(3 , HIGH);
         digitalWrite(2 , LOW);
         lcd_1.setCursor(0,0);
         lcd_1.print("SYSTEM'S ACTIVE!");
         lcd_1.setCursor(0,1);
         lcd_1.print("GAS LEAKAGE !!");
         lcd_1.setCursor(16,3);
         lcd_1.print("SENDING SMS !!");
         /*char text[] = "SENDING SMS !!";
         unsigned int i = 0;
         while(text[i] != '\0')
         {
          lcd_1.print(text[i]);
          if(i>=14)
             lcd_1.command(0x18);
          delay(50);
          i++ ;
         }*/
         Serial.println("WARNING !!!!! ");
         Serial.println("GAS LEAKAGE");
         Serial.println("Turning ON exaust FAN ");
         Serial.println("\n");
         delay(1000);
         digitalWrite(3 , HIGH);
         digitalWrite(2 , HIGH);
         lcd_1.clear();
   }
}
 
void unlockdoor()
{
    digitalWrite(pin , HIGH); 
    lcd.setCursor(0,0);
    lcd.println(" ");
    lcd.setCursor(1,0);
    lcd.print("Access Granted");
    lcd.setCursor(4,1);
    lcd.println("WELCOME!!");
    lcd.setCursor(15,1);
    lcd.println(" ");
    lcd.setCursor(16,1);
    lcd.println(" ");
    lcd.setCursor(14,1);
    lcd.println(" ");
    lcd.setCursor(13,1);
    lcd.println(" ");
    lcd_1.clear();
    lcd_1.setCursor(0 , 0);
    lcd_1.print("PIR SYSTEM");
    lcd_1.setCursor( 17, 1);
    lcd_1.print("DEACTIVATED ..");
    Serial.print("\n");
    Serial.print("PIR system deactivated !!\n");
    for(pos = 180; pos>=0; pos-=5) 
    {
        myservo.write(pos); 
        delay(5); 
    }
    delay(500);
    counterbeep();
    displayscreen();
    delay(1000);
    normalmode();
}
 
void incorrect()
{
    lcd.clear();
    digitalWrite(pin , LOW);
    lcd.setCursor(1,0);
    lcd.print("CODE");
    lcd.setCursor(6,0);
    lcd.print("INCORRECT");
    lcd.setCursor(15,1);
    lcd.println(" ");
    lcd.setCursor(4,1);
    lcd.println("/////////////!!!");
    delay(2000);
    lcd.clear();
    displayscreen();
}

void displayscreen()
{
    lcd.setCursor(0,0);
    lcd.println("*ENTER THE CODE*");
    lcd.setCursor(1 ,1); 
    lcd.println("TO OPEN DOOR!!");
}

void counterbeep()
{   
    lcd.clear();
    lcd.setCursor(2,15);
    lcd.println(" ");
    lcd.setCursor(2,14);
    lcd.println(" ");
    delay(500);
    
    lcd.setCursor(2,0);
    lcd.println("GET IN WITHIN:::");
    lcd.setCursor(4,1);
    lcd.print("5");
    delay(500);
   
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.println("GET IN WITHIN:");
    lcd.setCursor(4,1);
    lcd.print("4");
    delay(500);
    
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.println("GET IN WITHIN:");
    lcd.setCursor(4,1); 
    lcd.print("3");
    delay(500);
    
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.println("GET IN WITHIN:");
    lcd.setCursor(4,1); 
    lcd.print("2");
    delay(500);
    
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.println("GET IN WITHIN:");
    lcd.setCursor(4,1);
    lcd.print("1");
    delay(500);
    
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.println("GET IN WITHIN::");
    lcd.setCursor(4,1);
    lcd.print("0");
    delay(40);
    
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("RE-LOCKING");
    delay(100);
    lcd.setCursor(12,0);
    lcd.print(".");
    delay(100);
    lcd.setCursor(13,0);
    lcd.print(".");
    delay(100);
    lcd.setCursor(14,0);
    lcd.print(".");
    for(pos = 0; pos <= 180; pos +=5) 
    { 
        myservo.write(pos); 
        delay(15);
        currentposition=0;
        lcd.clear();
    }
    delay(100);
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("LOCKED!");
    delay(440);
}

void actpir()
{
  if(Serial.available() > 0)
   {
      char data = Serial.read();
      switch(data)
      {
         case 'a': {digitalWrite(pin , LOW);
                    lcd_1.clear();
                    lcd_1.setCursor(0 , 0);
                    lcd_1.print("PIR SYSTEM");
                    lcd_1.setCursor( 17, 3);
                    lcd_1.print("ACTIVATED ..");
                    Serial.print("\n");
                    Serial.print("PIR system activated !!\n");
                    delay(1000);
                    normalmode();}
                    break;
      }
   }
}

