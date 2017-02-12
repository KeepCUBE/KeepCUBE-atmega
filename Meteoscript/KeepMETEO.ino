#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include "SparkFunBME280.h"

class Command {
  public:
    Command(char starting, char ending);
    Command(String m);
    Command(char &m);
    Command();
    void readSerial();
    String getIdentifier();
    String getParameter(int nth);
    String getParameterValue(const char parameterIdentifier);
    String toString();
    String msg = "";
  private:
    bool isUppercaseLetter(char m);
    bool ok = false;
    bool multiple = false;
    bool incorrect = false;
    char startChar = '#';
    char endChar = ';';
};

Adafruit_HTU21DF htu = Adafruit_HTU21DF();
BME280 bme;

void setup() {
  Serial.begin(115200);
  Serial.println("[ OK ] Starting meteoscript V1 by DomiZ");
  if (!htu.begin()) {
    Serial.println("[FAIL] HTU21D not connected!");
    while (1);
  }
  else {
    Serial.println("[ OK ] HTU21D connected");
    bme.settings.commInterface = I2C_MODE;
    bme.settings.I2CAddress = 0x76;
    bme.settings.runMode = 3;
    bme.settings.tStandby = 5;
    bme.settings.filter = 0;
    bme.settings.tempOverSample = 1;
    bme.settings.pressOverSample = 1;
    bme.settings.humidOverSample = 1;
    bme.begin();
    Serial.println("[ OK ] BME280 connected");
    delay(10);
    Serial.println("=======================================");
  }
}
void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readString();
    Serial.print("[INFO] Received: ");
    Serial.println(input);
    Command command(input);
    String header = command.getIdentifier();
    String requestid = command.getParameterValue('R');
    /*=================================================
      Serial.print("[DUMP] Header: ");
      Serial.println(header);
      Serial.print("[DUMP] Request ID: ");
      Serial.println(requestid);
      ==================================================*/
    if (header == "RVT") {
      get_temperature(requestid);
    }
    else {
      if (header == "RVP") {
        get_pressure(requestid);
      }
      else {
        if (header == "RVH") {
          get_humidity(requestid);
        }
        else {
          Serial.println("[WARN] Unknown request");
        }
      }
    }
  }
}
