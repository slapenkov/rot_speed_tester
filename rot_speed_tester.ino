/*


  The circuit:
   LCD RS pin to digital pin PB11
   LCD Enable pin to digital pin PB10
   LCD D4 pin to digital pin PB0
   LCD D5 pin to digital pin PB1
   LCD D6 pin to digital pin PC14
   LCD D7 pin to digital pin PC15
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin

   //Encoder
   PB13 - PinA - DT
   PB14 - PinB - CLK - or flipped, don't care
   PB15 -pushbutton

*/

// include the library code:
#include <LiquidCrystal.h>
#include <RotaryEncoderAdvanced.h>
#include <RotaryEncoderAdvanced.cpp> //for some reason linker can't find the *.cpp :(


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = PB11, en = PB10, d4 = PB0, d5 = PB1, d6 = PC14, d7 = PC15;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


#define PIN_A            PB14  //ky-040 clk pin, add 100nF/0.1uF capacitors between pin & ground!!!
#define PIN_B            PB13  //ky-040 dt  pin, add 100nF/0.1uF capacitors between pin & ground!!!
#define BUTTON           PB15  //ky-040 sw  pin, add 100nF/0.1uF capacitors between pin & ground!!!

uint16_t buttonCounter = 0;


RotaryEncoderAdvanced<float> encoder(PIN_A, PIN_B, BUTTON, 0.1, 0.0, 3.3);                         //0.1 step per click, minimum value 0, maximum value 3.3

void encoderISR()
{
  encoder.readAB();
}

void encoderButtonISR()
{
  encoder.readPushButton();
}

// the setup function runs once when you press reset or power the board
void setup() {
  encoder.begin();                                                           //set encoders pins as input & enable built-in pullup resistors

  attachInterrupt(digitalPinToInterrupt(PIN_A),  encoderISR,       CHANGE);  //call encoderISR()    every high->low or low->high changes
  attachInterrupt(digitalPinToInterrupt(BUTTON), encoderButtonISR, FALLING); //call pushButtonISR() every high->low              changes


  // initialize digital pin PB1 as an output.
  pinMode(PC13, OUTPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print(F("hello, world!"));
}

// the loop function runs over and over again forever
void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(encoder.getValue());
  lcd.setCursor(10, 1);
  lcd.print(encoder.getPushButton());
  digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);              // wait for a second
  digitalWrite(PC13, LOW);    // turn the LED off by making the voltage LOW
  delay(100);              // wait for a second
}
