#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}
int in1 = 7;
void setup()
{
  
  pinMode(in1, OUTPUT);
  digitalWrite(in1, HIGH);

  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  // setDS3231time(30,42,21,4,26,11,14);
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}

//Loops to check time and actuate water valve
void waterLoop() {
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    waterTime(hour, minute, 7, 15); //On Sunday, at 7 am, water for 20 minutes
    waterTime(hour, minute, 20, 10); //Water at 8pm for 10 minutes
    break;
  case 2:
    Serial.println("Monday");
    waterTime(hour, minute, 7, 15);
    waterTime(hour, minute, 20, 10);
    break;
  case 3:
    Serial.println("Tuesday");
    waterTime(hour, minute, 7, 15);
    waterTime(hour, minute, 20, 10);
    
    break;
  case 4:
    Serial.println("Wednesday");
    waterTime(hour, minute, 20, 10);
    waterTime(hour, minute, 7, 15);
    break;
  case 5:
    Serial.println("Thursday");
    waterTime(hour, minute, 20, 10);
    waterTime(hour, minute, 7, 15);
    break;
  case 6:
    Serial.println("Friday");
    waterTime(hour, minute, 20, 10);
    waterTime(hour, minute, 7, 15);
    break;
  case 7:
    Serial.println("Saturday");
    waterTime(hour, minute, 20, 10);
    waterTime(hour, minute, 7, 15);
    break;
  }

  
}

//set a "water time" on a specified hour for a specified duration
void waterTime(byte hour, byte minute, int theHour, int dur) {
  
  if (hour == theHour && minute == 0) { 
          digitalWrite(in1, LOW);
          Serial.println("Water turned ON");

          delay(dur * 60000);
          digitalWrite(in1, HIGH);
          Serial.println("Water turned OFF");
          } else {
            Serial.println("Water OFF");
          }
}
void loop()
{
  waterLoop();
//  displayTime(); // display the real-time clock data on the Serial Monitor,
  delay(1000); // every second
}
