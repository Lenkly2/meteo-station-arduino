#include <GyverBME280.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,16,2);
GyverBME280 bme;
bool ab;
bool backlight = 0;
int arraynumber = 0;
int count = 0;
float TempArrayH[6] = {};
float HumArrayH[6] = {};
float PressArrayH[6] = {};

int aoo = 0;
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  bme.setStandbyTime(STANDBY_1000MS);
  bme.begin();
  pinMode(7,INPUT);
  
}

int button(){
  ab = digitalRead(7);
  if (ab == 1){
    for (int a = 0;a < 8;a++){
      delay(100);
    }
    ab = digitalRead(7);

    if (ab == 1){
      if (backlight == 1){
        lcd.noBacklight();
      }else{
        lcd.backlight();
      }
      if (backlight == 1){
        backlight = 0;
      }else{
        backlight = 1;

      }
      return 1;
    }else{
      return 2;
    }
  }else{
    return 0;
  }

}

void SensorRead(){

  float Humidity = bme.readHumidity();
  float Temperature = bme.readTemperature();
  float Pressure = bme.readPressure(); 

  lcd.setCursor(0, 0);
  lcd.print(Temperature);
  lcd.print("C");

  lcd.setCursor(0,1);
  lcd.print(int(Humidity));
  lcd.print("%");

  lcd.setCursor(8,0);
  lcd.print(pressureToMmHg(Pressure));
  lcd.print("mm");

}

int DataHour(int num){
  float Humidity = bme.readHumidity();
  float Temperature = bme.readTemperature();
  float Pressure = bme.readPressure(); 
  Serial.println(pressureToMmHg(Pressure));
  TempArrayH[num] = Temperature;
  HumArrayH[num] = Humidity;
  PressArrayH[num] = pressureToMmHg(Pressure);
}

void loop() {
  
  SensorRead();
  count +=1;

  if (count % 60 == 0){
    DataHour(arraynumber);
    arraynumber +=1;
  }
  for (int i = 0; i < 60; i++){

    delay(1000);
    int but = button();
    lcd.setCursor(8,1);
    lcd.print(i);

    // if (skrentime < 5){
    //   skrentime +=1;
    // }else{
    //   lcd.noBacklight();
    // }

    if (but == 1){
      // skrentime = 0;
      // lcd.backlight();
      SensorRead();
      delay(1);
    }
  }
  
  
}
