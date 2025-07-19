
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  Serial.begin(9600); // Initialize serial communication
  lcd.init();        // Initialize the LCD
  lcd.backlight();   // Turn on the backlight (if available)
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.setCursor(6, 0);
  lcd.print("I:");
  lcd.setCursor(2, 1);
  lcd.print("POW:");
}

void loop() {
  int sensorValueV = analogRead(A0); // Read voltage sensor value
  double voltage = map(sensorValueV, 0, 1024, 0, 2500);
  voltage /= 100;
  
  float current, Samples = 0.0, A = 0.0, Value = 0.0;

  for (int x = 0; x < 150; x++) {
    current = analogRead(A1);       
    Samples = Samples + current;    
    delay(3);                       
  }
  
  A = Samples / 150.0;
  Value = abs((2.5 - (A * (5.0 / 1024.0))) / 0.185);
   if (voltage > 6.06) {
    voltage = 6.06;
  }

  if (abs(Value) > 0.52) {
    Value = 0.52; 
  }
  float power = Value * voltage;

  Serial.print("current:");
  Serial.println(abs(Value));
  Serial.print("voltage:");
  Serial.println(voltage);
  Serial.print("POWER:");
  Serial.println(power);

  Serial.print(voltage); 
  Serial.print(',');     
  Serial.print(Value);   
  Serial.print(',');     
  Serial.println(power); 

  lcd.setCursor(3, 0);
  lcd.print(voltage);  

  lcd.setCursor(9, 0);
  lcd.print(Value);

  lcd.setCursor(7, 1);
  lcd.print(power);

  delay(1000);  
}
