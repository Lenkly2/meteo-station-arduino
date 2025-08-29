#include <GyverBME280.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,16,2);
GyverBME280 bme;
bool click;
bool click2;
bool backlight = 0;
int arraynumber = 0;
int arraynumberd = 0;
int arraycount = 0;
int count = 0;
int screen = 0;
float Humidity;
float Temperature;
float Pressure;
float TempArrayH[6] = {};
float HumArrayH[6] = {};
float PressArrayH[6] = {};
float TempArrayD[24] = {};
float HumArrayD[24] = {};
float PressArrayD[24] = {};

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  bme.setStandbyTime(STANDBY_1000MS);
  bme.begin();
  pinMode(7,INPUT);
  pinMode(4,INPUT);
  
}

int ButtonRight(){
  click2 = digitalRead(4);
  if (click2 == 1){
    for (int a = 0;a < 8;a++){
      delay(100);
    }
    click2 = digitalRead(4);

    if (click2 == 1){
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

    }else{
      screen = 0;
      SensorRead();
    }
  }else{
    return 0;
  }
}

int Button(){
  click = digitalRead(7);
  if (click == 1){
    for (int a = 0;a < 8;a++){
      delay(100);
    }
    click = digitalRead(7);
    if (click == 1){
      arraycount += 1;
    }else{
      screen +=1;
      if (screen > 6){
        screen = 0;
      }
    }
    
  }else{
    return 0;
  }
}

void SensorRead(){
  Humidity = bme.readHumidity();
  Temperature = bme.readTemperature();
  Pressure = bme.readPressure(); 
  
  lcd.clear();
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
  Humidity = bme.readHumidity();
  Temperature = bme.readTemperature();
  Pressure = bme.readPressure(); 
  TempArrayH[num] = Temperature;
  HumArrayH[num] = Humidity;
  PressArrayH[num] = pressureToMmHg(Pressure);
}

int DataDay(int num){
  Humidity = bme.readHumidity();
  Temperature = bme.readTemperature();
  Pressure = bme.readPressure(); 
  TempArrayD[num] = Temperature;
  HumArrayD[num] = Humidity;
  PressArrayD[num] = pressureToMmHg(Pressure);
}

int DataDisplay(float array[],int maxcount, char text[]){
  if (arraycount >= maxcount){
    arraycount = 0;
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(text);
  lcd.print(arraycount);
  lcd.setCursor(0, 1);
  lcd.print(array[arraycount]);
}

void loop() {
  if (screen == 0){
    SensorRead();
  }
  
  count +=1;

  if (count % 10 == 0){
    DataHour(arraynumber);
    arraynumber +=1;
    if (count % 60 == 0){
      count = 0;
      arraynumber = 0;
      DataDay(arraynumberd);
      arraynumberd +=1;
      if (arraynumberd >24){
        arraynumberd = 0;
      }
    }
  }
  for (int i = 0; i < 60; i++){

    delay(1000);
    Button();
    ButtonRight();
    switch(screen){
      case 0:
        lcd.setCursor(8,1);
        lcd.print(i);
        break;
      case 1: // вивід значень перебираючи дані в списку, графіка небуде
        DataDisplay(TempArrayH,6,"TempH:");
        break;
      case 2:
        DataDisplay(HumArrayH,6,"HumH:");
        break;
      case 3:
        DataDisplay(PressArrayH,6,"PressH:");
        break;
      case 4:
        DataDisplay(TempArrayD,24,"TempD:");
        break;
      case 5:
        DataDisplay(HumArrayD,24,"HumD:");
        break;
      case 6:
        DataDisplay(PressArrayD,24,"PressD:");
        break;
    }
    
  }
  
  
}
