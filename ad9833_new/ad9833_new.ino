#include <AD9833.h>
#include  <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2);

//REF - NC
//VCC - 5V
//GND - GND
//DAT - 11
//CLK - 13
//FNC - 9

AD9833 gen(9);// Defaults to 25MHz internal reference frequency

volatile int frequency = 1; //frequency (in Hz)
int WAVE = SQUARE_WAVE;

int button1 = 2;
int wmode = 0;

void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Frequency");
  lcd.setCursor(0, 1);
  lcd.print("Generator");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(String(frequency) + String("Hz"));
  delay(100);
  gen.Begin();
  gen.ApplySignal(WAVE, REG0, frequency);
  gen.EnableOutput(true);
  pinMode(button1, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  if (!digitalRead(button1)) {
    if (wmode >= 2) {
      wmode = 0;
    }
    wmode++;
    if (wmode == 1) {
      WAVE = SQUARE_WAVE;
      gen.EnableOutput(false);
      gen.ApplySignal(WAVE, REG0, frequency);
      gen.EnableOutput(true);
    } else if (wmode == 2) {
      WAVE = SINE_WAVE;
      gen.EnableOutput(false);
      gen.ApplySignal(WAVE, REG0, frequency);
      gen.EnableOutput(true);
    }
    delay(100);
    Serial.println(wmode);
  }
  frequency = map(analogRead(A0), 0, 1023, 1, 1000);// 1Hz to 1000Hz
  //Serial.println(frequency);
  gen.ApplySignal(WAVE, REG0, frequency);
  lcd.print(String("Frequency :") + String(frequency) + String("Hz"));
  if (wmode == 1) {
    lcd.setCursor(0, 1);
    lcd.print("SQUARE_WAVE");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("SINE_WAVE");
  }
  delay(300);
  lcd.clear();
}
