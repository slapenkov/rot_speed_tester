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
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //LCD instance


#define PIN_A            PB14  //ky-040 clk pin, add 100nF/0.1uF capacitors between pin & ground!!!
#define PIN_B            PB13  //ky-040 dt  pin, add 100nF/0.1uF capacitors between pin & ground!!!
#define BUTTON           PB15  //ky-040 sw  pin, add 100nF/0.1uF capacitors between pin & ground!!!

#define TIMER1_CH1_PIN PA8
#define TIMER2_CH1_PIN PA0
#define TIMER3_CH1_PIN PA6
#define TIMER4_CH1_PIN PB6

#define PRES_FACTOR 1600
#define BASE_PERIOD 40000

uint16_t buttonCounter = 0;
int val = 0;

RotaryEncoderAdvanced<float> encoder(PIN_A, PIN_B, BUTTON, 1.0, 1.0, 300.0);                         //0.1 step per click, minimum value 0, maximum value 3.3

HardwareTimer pwmtimer1(1);
HardwareTimer pwmtimer2(2);
HardwareTimer pwmtimer3(3);
HardwareTimer pwmtimer4(4);

volatile float freqCh1 = 1.0, freqCh2 = 2.0, freqCh3 = 3.0, freqCh4 = 4.0;


//ISR's
void encoderISR()
{
  encoder.readAB();
}

void encoderButtonISR()
{
  encoder.readPushButton();
}

void setFrequency(HardwareTimer *timer, float frequency) {
  if (frequency > 0) {
    int val = int(BASE_PERIOD / frequency);
    timer->setOverflow(val);
    //pwmWrite(TIMER1_CH1_PIN, val / 2);
    if (timer == &pwmtimer1) {
      pwmWrite(TIMER1_CH1_PIN, val / 2);
    } else if (timer == &pwmtimer2) {
      pwmWrite(TIMER2_CH1_PIN, val / 2);
    } else if (timer == &pwmtimer3) {
      pwmWrite(TIMER3_CH1_PIN, val / 2);
    } else if (timer == &pwmtimer4) {
      pwmWrite(TIMER4_CH1_PIN, val / 2);
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Error!");
    };
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  //timers
  //setup pins
  pinMode(TIMER1_CH1_PIN, PWM);
  pinMode(TIMER2_CH1_PIN, PWM);
  pinMode(TIMER3_CH1_PIN, PWM);
  pinMode(TIMER4_CH1_PIN, PWM);

  //setup timers
  //timer1
  pwmtimer1.pause();
  pwmtimer1.setPrescaleFactor(PRES_FACTOR); //
  pwmtimer1.setOverflow(BASE_PERIOD); //set period
  pwmWrite(TIMER1_CH1_PIN, BASE_PERIOD / 2); //set duty cycle
  pwmtimer1.refresh();
  pwmtimer1.resume();

  //timer2
  pwmtimer2.pause();
  pwmtimer2.setPrescaleFactor(PRES_FACTOR); //
  pwmtimer2.setOverflow(BASE_PERIOD); //set period
  pwmWrite(TIMER2_CH1_PIN, BASE_PERIOD / 2); //set duty cycle
  pwmtimer2.refresh();
  pwmtimer2.resume();

  //timer1
  pwmtimer3.pause();
  pwmtimer3.setPrescaleFactor(PRES_FACTOR); //
  pwmtimer3.setOverflow(BASE_PERIOD); //set period
  pwmWrite(TIMER3_CH1_PIN, BASE_PERIOD / 2); //set duty cycle
  pwmtimer3.refresh();
  pwmtimer3.resume();

  //timer1
  pwmtimer4.pause();
  pwmtimer4.setPrescaleFactor(PRES_FACTOR); //
  pwmtimer4.setOverflow(BASE_PERIOD); //set period
  pwmWrite(TIMER4_CH1_PIN, BASE_PERIOD / 2); //set duty cycle
  pwmtimer4.refresh();
  pwmtimer4.resume();



  //encoder
  encoder.begin();                                                           //set encoders pins as input & enable built-in pullup resistors

  attachInterrupt(digitalPinToInterrupt(PIN_B),  encoderISR,       CHANGE);  //call encoderISR()    every high->low or low->high changes
  attachInterrupt(digitalPinToInterrupt(BUTTON), encoderButtonISR, FALLING); //call pushButtonISR() every high->low              changes


  // initialize digital pin PB1 as an output. - status
  pinMode(PC13, OUTPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print(F("hello, world!"));
}

// the loop function runs over and over again forever
void loop() {
  val = int(BASE_PERIOD / encoder.getValue());
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(encoder.getValue());
  lcd.setCursor(10, 1);
  lcd.print(val);
  //  pwmtimer1.setOverflow(val);
  //  pwmWrite(TIMER1_CH1_PIN, val / 2);
  setFrequency(&pwmtimer1, 100.0);
  //  digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(100);              // wait for a second
  //  digitalWrite(PC13, LOW);    // turn the LED off by making the voltage LOW
  //  delay(100);              // wait for a second
}
