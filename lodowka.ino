#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/io.h>


//INPUTS
#define CONTACTRON_PIN 2
#define LED_ENABLE_PIN 9


//OUTPUTS
#define BUZZER 4
#define LED_PIN 13

//OTHER 
#define BUZZER_BEEP_ENABLE 10


//outputs
bool buzzer_beep = 0;
bool led_enable = 0;

volatile boolean flag;
byte buzzer_count = 0;
byte serial_counter = 0;
int voltage;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(CONTACTRON_PIN, INPUT);
  pinMode(LED_ENABLE_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  goToSleep();
}
void loop()
{
  led_enable = digitalRead(LED_ENABLE_PIN);
  
  if (serial_counter == 0)
  {
    Serial.println("------  Waking up!  ------");
  }

  if (serial_counter >= 60)
    digitalWrite(BUZZER, 1);
  else
    digitalWrite(BUZZER, 0);

  if (serial_counter % 2 == 0)
    digitalWrite(LED_PIN, 0);
  else
    digitalWrite(LED_PIN, led_enable);

  doSomething();
  if (digitalRead(CONTACTRON_PIN) == 1)
  {
    serial_counter = 0;
    goToSleep();
  }
}
void setFlag()
{
  flag = true;
}

void goToSleep()
{
  digitalWrite(LED_PIN, 0);
  digitalWrite(BUZZER, 0);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(0, setFlag, LOW); //pin D2
  sleep_mode();//uśpienie aż do podania sygnału niskiego
  sleep_disable();
  detachInterrupt(0);
}

void doSomething()
{
  //timerToBuzzer();
  Serial.print(serial_counter);
  Serial.println(". Hello!");

  if (digitalRead(CONTACTRON_PIN) == 0)
  {
    serial_counter++;
  }
  delay(500);
}

void timerToBuzzer()
{

  if (digitalRead(CONTACTRON_PIN) == 1)
  {
    buzzer_count++;
    printSerial();
  }

  else
  {
    buzzer_count = 0;
    printSerial();
  }
}

void beep()
{
  if (buzzer_count > BUZZER_BEEP_ENABLE)
  {
    buzzer_beep = !buzzer_beep;
  }
  else
  {
    buzzer_beep = 0;
  }
}

void printSerial()
{
  String toSerial = "";
  toSerial += serial_counter;
  toSerial += ". ";
  toSerial += "contactron: ";
  toSerial += digitalRead(CONTACTRON_PIN);
  toSerial += " buzzer: ";
  toSerial += buzzer_beep;
  toSerial += " count: ";
  toSerial += buzzer_count;
  Serial.println(toSerial);
  serial_counter++;
}
