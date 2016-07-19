#include <LiquidCrystal_I2C.h>
//#include <LiquidCrystal.h>
#include <RCSwitch.h>
#include "BMP280.h"
#include <Wire.h>

#define temperature 1  // BMP(temperature); // vrati teplotu
#define pressure    2  // BMP(pressure); // vrati tlak


#define upPin    6
#define downPin  5
#define enterPin 7
#define backPin  8


//menu piny
int klavesa;
int mZobrazeny = 1;
int keyPressed = 0;
int16_t mPozice = 0;
int mFlip = 0;
bool mZacatek;

uint16_t refreshCount = 0;

uint16_t rgb[4];
int mState = 0;

//definice nazvu polozek v seznamu
String mChar[] = {
  "1>Show IP adress",
  "2>Set color     ",
  "3>Show temp.    ",
  "4>Show Humidity ",
  "5>Show pressure ",
  "6>Restart       "
};

//zjisteni poctu polozek v seznamu
int mPocet = (sizeof(mChar) / sizeof(mChar[0]));


LiquidCrystal_I2C lcd(0x3F, 16, 2);  //pro i2c
//LiquidCrystal lcd(RS, RW, EN, D4, D5, D6, D7);  //pro normalni displej


#define R_led_pin 9
#define G_led_pin 10
#define B_led_pin 11
#define test_led_pin 13
#define transmit_pin 12

RCSwitch mySwitch = RCSwitch();

int Rled = 0;
int Gled = 0;
int Bled = 0;
String LED_address;

const int HTU_ADRESA = 0x40;
const int HTU_TEPLOTA = 0xE3;
const int HTU_VLHKOST = 0xE5;

double BMP_TEPLOTA;
double BMP_TLAK;

String dsc;
String codeToSend;

bool light = false;

BMP280 bmp;

byte degree[8] = {
  0b00010,
  0b00101,
  0b00010,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

void setup()
{
   /*
    ////////piny pro lcd////////
    pinMode(54, OUTPUT);      //
    pinMode(55, OUTPUT);      //
    pinMode(67, OUTPUT);      //
    pinMode(68, OUTPUT);      //
    pinMode(13, OUTPUT);      //
    digitalWrite(54, LOW);    //
    digitalWrite(55, HIGH);   //
    digitalWrite(67, HIGH);   //
    digitalWrite(68, LOW);    //
    analogWrite(13, 130);//jas//
    ////////////////////////////
  */
  pinMode(upPin,    INPUT);  //up
  pinMode(downPin,  INPUT);  //down
  pinMode(enterPin, INPUT);  //enter
  pinMode(backPin,  INPUT);  //back

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  //lcd.begin(16, 2);
  lcd.createChar(0, degree);

  lcd.setCursor(0, 1); lcd.print(F("Loading...      "));

  pinMode(R_led_pin, OUTPUT);
  pinMode(G_led_pin, OUTPUT);
  pinMode(B_led_pin, OUTPUT);

  mySwitch.enableTransmit(transmit_pin);


  Wire.begin();
  if (!bmp.begin())
  {
    //Serial.println(F("BMP nebylo nacteno!"));
  }
  else
  {
    //Serial.println(F("BMP pripojeno!"));
  }
  bmp.setOversampling(4);

}

String SerialRead(int count)
{
  String dsc;
  for (int i = 0; i < count; i++)
  {
    dsc += char(Serial.read());
  }
  return dsc;
}

//resetovaci funkce, nevim presne jak to funguje, nasel jsem to na netu
//void(* reset) (void) = 0;

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\


void loop()
{
  HandleSerial();
  refresh_lcd(); //bacha, tohle je na displej!
  refresh_led(); //bacha, tohle je na RGB led pasek!
  mMenu();
  analogWrite(R_led_pin, 0);
  analogWrite(G_led_pin, 0);
  analogWrite(B_led_pin, 255);
}

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

void zobrazitIP()
{
  lcd.setCursor(0, 0); lcd.print(F("IP:             "));
  lcd.setCursor(0, 1); lcd.print(F("10.0.0.106      ")); //sem se doplni kod na zjisteni pravdive IP
}


void nastavitBarvu()
{
  int colorNow = 1;
  lcd.setCursor(0, 0); lcd.print(F("Nastav postupne "));
  lcd.setCursor(0, 1); lcd.print(F("hodnoty barev:  "));
  delay(1800);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////

  lcd.setCursor(0, 0); lcd.print(F("Hodnota cervene "));
  lcd.setCursor(0, 1); lcd.print(F("barvy:          "));


  do
  {
    keyPressed = wait4key();
    wait4antikey(keyPressed);
    switch (keyPressed) {
      case 1:
        if (rgb[1] == 99) {
          rgb[1]--;
        }
        rgb[1]++;
        break;

      case 2:
        if (rgb[1] == 0) {
          rgb[1]++;
        }
        rgb[1]--;
        break;

      case 3:
        lcd.setCursor(0, 0); lcd.print(F("Cervena barva   "));
        lcd.setCursor(0, 1); lcd.print(F("                "));
        colorNow = 2;
        delay(500);
        goto colorEnter1;
        break;

      case 4:

        break;

      default:

        break;
    }

    lcd.setCursor(7, 1); lcd.print(rgb[1]); lcd.print(F("    "));
  }
  while (true);
colorEnter1:
  lcd.setCursor(0, 1); lcd.print(F("nastavena!      "));
  delay(500);


  lcd.setCursor(0, 0); lcd.print(F("Hodnota zelene  "));
  lcd.setCursor(0, 1); lcd.print(F("barvy:          "));


  do
  {
    keyPressed = wait4key();
    wait4antikey(keyPressed);
    switch (keyPressed) {
      case 1:
        if (rgb[2] == 99) {
          rgb[2]--;
        }
        rgb[2]++;
        break;

      case 2:
        if (rgb[2] == 0) {
          rgb[2]++;
        }
        rgb[2]--;
        break;

      case 3:
        lcd.setCursor(0, 0); lcd.print(F("Zelena barva    "));
        lcd.setCursor(0, 1); lcd.print(F("                "));
        colorNow = 2;
        delay(500);
        goto colorEnter2;
        break;

      case 4:

        break;

      default:

        break;
    }

    lcd.setCursor(7, 1); lcd.print(rgb[2]); lcd.print("    ");
  }
  while (true);
colorEnter2:
  lcd.setCursor(0, 1); lcd.print(F("nastavena!      "));
  delay(500);


  lcd.setCursor(0, 0); lcd.print(F("Hodnota modre   "));
  lcd.setCursor(0, 1); lcd.print(F("barvy:          "));


  do
  {
    keyPressed = wait4key();
    wait4antikey(keyPressed);
    switch (keyPressed) {
      case 1:
        if (rgb[3] == 99) {
          rgb[3]--;
        }
        rgb[3]++;
        break;

      case 2:
        if (rgb[3] == 0) {
          rgb[3]++;
        }
        rgb[3]--;
        break;

      case 3:
        lcd.setCursor(0, 0); lcd.print(F("Modra barva     "));
        lcd.setCursor(0, 1); lcd.print(F("                "));
        colorNow = 2;
        delay(500);
        goto colorEnter3;
        break;

      case 4:

        break;

      default:

        break;
    }

    lcd.setCursor(7, 1); lcd.print(rgb[3]); lcd.print(F("    "));
  }
  while (true);
colorEnter3:
  lcd.setCursor(0, 1); lcd.print(F("nastavena!      "));
  delay(1000);


  lcd.setCursor(0, 0); lcd.print(F("Hodnoty barev   "));
  lcd.setCursor(0, 1); lcd.print(F("nastaveny!      "));


}


void ukazatTeplotu()
{
  lcd.setCursor(0, 0); lcd.print(F("                "));
  lcd.setCursor(0, 1); lcd.print(F("Temp: 21 C   ")); //sem se doplni kod na zjisteni pravdive teploty
}


void ukazatVlhkost()
{
  lcd.setCursor(0, 0); lcd.print(F("                "));
  lcd.setCursor(0, 1); lcd.print(F("Humi: 59 %   ")); //sem se doplni kod na zjisteni pravdive vlhkosti
}


void ukazatTlak()
{
  lcd.setCursor(0, 0); lcd.print(F("                "));
  lcd.setCursor(0, 1); lcd.print(F("Press: 1000 hPa ")); //sem se doplni kod na zjisteni pravdiveho tlaku
}


void restartovatKC()
{
  lcd.setCursor(0, 0); lcd.print(F("                "));

  lcd.setCursor(0, 1); lcd.print(F("Restarting      "));
  delay(250);
  lcd.setCursor(0, 1); lcd.print(F("Restarting.     "));
  delay(250);
  lcd.setCursor(0, 1); lcd.print(F("Restarting..    "));
  delay(250);
  lcd.setCursor(0, 1); lcd.print(F("Restarting...   "));
  delay(500);


  digitalWrite(11, LOW);

  //reset();//zatim nefunguje, bude priste
}


void refresh_lcd()
{
  //preicteni pocitadla
  refreshCount += 1;



  //IFy casove spinane, v poslednim je refreshCount = 0;
  if (refreshCount == 150)
  {
    lcd.setCursor(0, 0); lcd.print(F("Temperature:    "));
    //lcd.setCursor(0, 1); lcd.print(String(BMP(temperature)));
    lcd.setCursor(0, 1); lcd.print(F("26.18"));
    lcd.write(byte(0)); //stupen
    lcd.print("C  ");
    lcd.print("79.12"); //vypise hodnotu ve fahrenheitech
    lcd.write(byte(0)); //stupen
    lcd.write('F');
  }

  if (refreshCount == 300)
  {
    lcd.setCursor(0, 0); lcd.print(F("Pressure:       "));
    //lcd.setCursor(0, 1); lcd.print(String(BMP(pressure)) + " hPa     ");
    lcd.setCursor(0, 1); lcd.print(F("1012.73 hPa     "));
  }

  if (refreshCount == 450)
  {
    lcd.setCursor(0, 0); lcd.print(F("Humidity:       "));
    lcd.setCursor(0, 1); lcd.print(String(HTU_convHumi()) + " %         ");
    //lcd.setCursor(0, 1); lcd.print(F("46.96 %         "));
  }

  if (refreshCount == 600)
  {
    lcd.setCursor(0, 0); lcd.print(F("Users:          "));
    lcd.setCursor(0, 1); lcd.print(F("1               ")); //sem se doplni kod na zjisteni poctu lidi
    refreshCount = 0;
  }
}


void refresh_led()
{
  static byte r;
  static byte g;
  static byte b;
  static int x;
  int rychlost = 3;

  delay(10); //celkove cekani mezi vsemi operacemi (proste snizovani taktu CPU :D)

  if (b >= 200)
  {
    x = 1;
  }
  if (b <= 50)
  {
    x = 2;
  }

  switch (x)
  {
    case 0:
      b = 161;
      x = 2;
      break;
    case 1:
      b -= rychlost;
      break;
    case 2:
      b += rychlost;
      break;
  }


  analogWrite(R_led_pin, r);
  analogWrite(G_led_pin, g);
  analogWrite(B_led_pin, b);
}

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

void HandleSerial()
{
  if (Serial.available() > 0)
  {
    if (Serial.peek() == '#')
    {
      delay(100);
      Serial.read();
      dsc = "";
      dsc = SerialRead(1);
      if (dsc == "D")
      {
        dsc = SerialRead(1);
        if (dsc == "S")
        {
          //sending info
          dsc = SerialRead(1);
          if (dsc == "T")
          {
            //sending temperature
            dsc = SerialRead(3);
            if (dsc == "00E")
            {
              //Serial.println("RRT" + String(HTU_convTemp()*100, 0) + "E");
              Serial.println("#RRT-212E");
            }
          }
          else if (dsc == "H")
          {
            dsc = SerialRead(3);
            if (dsc == "00E")
            {
              //HERE SERIAL PRINT HUMIDITY
              Serial.println("#RRH99E");
            }
          }
          else if (dsc == "P")
          {
            dsc = SerialRead(3);
            if (dsc == "00E")
            {
              //HERE SERIAL PRINT PRESSURE
              Serial.println("#RRP1024321E");
            }
          }
        }
        else if (dsc == "C")
        {
          //DO CHANGE
          if (Serial.read() == 'C')
          {
            //color change
            LED_address = SerialRead(2);
            Rled = SerialRead(2).toInt();
            Gled = SerialRead(2).toInt();
            Bled = SerialRead(2).toInt();
            if (Serial.read() == 'E')
            {
              Serial.println("Changing color on: " + LED_address + " to:  B" + String(Rled) + " G" + String(Gled) + " B" + String(Bled));
//9 10 11
              



















              
            }
          }
        }
        else if (dsc == "B")
        {
          //DO BROADCAST
          dsc = SerialRead(1);
          if (dsc == "C")
          {
            if (SerialRead(1) == ";")
            {
              codeToSend = "";
              while (Serial.peek() != ';')
              {
                codeToSend += SerialRead(1);
              }
              Serial.read();
              if (SerialRead(1) == "E")
              {
                //HERE DIRECTLY BROADCAST CODE (v promenne codeToSend)
                //Serial.println("Broadcasting code: " + codeToSend);
                mySwitch.send(codeToSend.toInt(), 24);
              }
            }
          }
        }
        else if (dsc == "R")
        {
          //response receiving
        }
      }
      else
      {
        Serial.read();
      }
    }
    dsc == "";
  }
}


long htu(int adresa, int kod)
{
  Wire.beginTransmission(adresa);
  Wire.write(kod);
  Wire.endTransmission();
  byte DataHIGH;
  byte DataLOW;
  byte CRC;
  long data;
  Wire.requestFrom(adresa, 3);
  while (Wire.available())
  {
    DataHIGH = Wire.read();
    DataLOW = Wire.read();
    CRC = Wire.read();
  }
  data = DataHIGH;
  data = data << 8;
  data = data + DataLOW;
  return data;
}


float HTU_convTemp()
{
  return -46.85 + 175.72 * ((htu(HTU_ADRESA, HTU_TEPLOTA)) / pow(2, 16));
}


float HTU_convHumi()
{
  return -6 + 125 * ((htu(HTU_ADRESA, HTU_VLHKOST)) / pow(2, 16));
}


float BMP(int in)
{
  const int P0 = 1013.25;
  double T, P;
  char result = bmp.startMeasurment();

  if (result != 0) {
    delay(result);
    result = bmp.getTemperatureAndPressure(T, P);

    if (result != 0)
    {
      double A = bmp.altitude(P, P0);
      switch (in) {
        case 1:
          return T;

        case 2:
          P += 40;
          return P;

        case 3:
          return A;
      }
    }
  }
}

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

int mMenu()
{
  mPozice = 1;
  mZacatek = true;
  mZobrazeny = 1;




  refresh_led();
  if (digitalRead(upPin)    == 1) {
    wait4antikey(1);
    goto preskoc;

  }
  if (digitalRead(downPin)  == 1) {
    wait4antikey(2);
    goto preskoc;

  }
  if (digitalRead(enterPin) == 1) {
    wait4antikey(3);
    goto preskoc;
  }
  if (digitalRead(backPin)  == 1) {
    wait4antikey(4);
    goto preskoc;
  }


  return 0;

preskoc:

  lcd.setCursor(0, 0); lcd.print(F("KeepCUBE setup  "));
  lcd.setCursor(0, 1); lcd.print(mChar[0]);

  mStart();
}


int mStart()
{

  while (true) {

    lcd.setCursor(0, 0); lcd.print(F("KeepCUBE setup  "));
    keyPressed = wait4key();
    wait4antikey(keyPressed);

    switch (keyPressed) {
      case 1:


        if (mFlip == 2) {
          mPozice -= 2;
        }


        //if (mFlip == 1 && mPozice == mPocet) {
        //  mPozice = (mPocet - 1);
        //}

        //if (mZacatek == true) {mPozice -= 1; mZacatek = false;}
        mUp();
        break;

      case 2:
        if (mFlip == 1) {
          mPozice += 2;
        }
        mDown();
        break;

      case 3:
        mEnter();
        break;

      case 4:
        return 0;
        break;
    }
  }
}


void mUp()
{
  if (mPozice < 0) {
    mPozice = 0;
  }

  if (mPozice == mPocet) {
    mPozice = mPocet;
    mPozice--;
  }
  lcd.setCursor(0, 1); lcd.print(mChar[mPozice]);
  mZobrazeny = mPozice;
  mZobrazeny++;
  mPozice--;
  mFlip = 1;
}


void mDown()
{
  lcd.setCursor(0, 1); lcd.print(mChar[mPozice]);
  mZobrazeny = mPozice;
  mZobrazeny++;
  mPozice++;

  if (mPozice == mPocet) {
    mPozice = mPozice - 1;
  }
  mFlip = 2;
}


void mEnter()
{
  switch (mZobrazeny) { //Tady se zpousti ty funkce z menu
    case 1:
      zobrazitIP();
      mBack();
      break;

    case 2:
      nastavitBarvu();
      mBack();
      break;

    case 3:
      ukazatTeplotu();
      mBack();
      break;

    case 4:
      ukazatVlhkost();
      mBack();
      break;

    case 5:
      ukazatTlak();
      mBack();
      break;

    case 6:
      restartovatKC();
      mBack();
      break;
  }
}


void mBack()
{
zacatek:
  keyPressed = wait4key();
  wait4antikey(keyPressed);
  if (keyPressed == 4) {
    goto konec;
  }
  goto zacatek;
konec:
  int mToCoSeMaZobrazit = mZobrazeny;
  mToCoSeMaZobrazit--;
  lcd.setCursor(0, 1); lcd.print(mChar[mToCoSeMaZobrazit]);
}


int wait4key()
{
  while (true)
  {
    refresh_led();
    if (digitalRead(upPin)    == 1) {
      return 1;
    }
    if (digitalRead(downPin)  == 1) {
      return 2;
    }
    if (digitalRead(enterPin) == 1) {
      return 3;
    }
    if (digitalRead(backPin)  == 1) {
      return 4;
    }
  }
}


void wait4antikey(int key)
{
  switch (key) {
    case 1:
      while (digitalRead(upPin)    != 0) {
        refresh_led();
      }
      break;
    case 2:
      while (digitalRead(downPin)  != 0) {
        refresh_led();
      }
      break;
    case 3:
      while (digitalRead(enterPin) != 0) {
        refresh_led();
      }
      break;
    case 4:
      while (digitalRead(backPin)  != 0) {
        refresh_led();
      }
      break;
  }
}


void wait4randomkey()
{
  int key = wait4key();

  switch (key) {
    case 1:
      while (digitalRead(upPin)    != 0) {
        refresh_led();
      }
      break;
    case 2:
      while (digitalRead(downPin)  != 0) {
        refresh_led();
      }
      break;
    case 3:
      while (digitalRead(enterPin) != 0) {
        refresh_led();
      }
      break;
    case 4:
      while (digitalRead(backPin)  != 0) {
        refresh_led();
      }
      break;
  }
}


