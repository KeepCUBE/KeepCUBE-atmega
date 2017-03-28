struct rgb {
  byte r;
  byte g;
  byte b;
};


class KeepCube {
  public:
    KeepCube(RF24 &_radio, RF24Network &_network);
    bool begin();

    void setLEDpins(byte r, byte g, byte b);

    void updateLED();
    void setTransitionParameters(int L, int T, int D, String P);

    void led(byte r, byte g, byte b, int time);
    void led(rgb color, int time);



    rgb black   = {0, 0, 0};
    rgb white   = {255, 255, 255};
    rgb red     = {255, 0, 0};
    rgb green   = {0, 255, 0};
    rgb blue    = {0, 0, 255};
    rgb yellow  = {128, 128, 0};
    rgb cyan    = {0, 128, 128};
    rgb magenta = {128, 0, 128};



  private:
    RF24 radio;
    RF24Network network;

    const uint16_t this_node = 00;     // Address of our node in Octal format ( 04,031, etc)
    const uint16_t other_node = 01;   // Address of the other node in Octal format



    // Color transitions

    int calculateStep(int startVal, int endVal);
    int calculateVal(int step, int val, int i);



    rgb pin = {9, 5, 6};

    rgb val;
    rgb prev;




    long wait = 2000;  // millis, if loops = 1000
    int hold = 0;

    long loops = 1000;


    int numberOfPoints;
    int transitionDuration;
    int litDuration;
    String colorString;



};

KeepCube::KeepCube(RF24 &_radio, RF24Network &_network):
  radio(_radio),
  network(_network) {}


void KeepCube::setLEDpins(byte r, byte g, byte b) {
  pin.r = r;
  pin.g = g;
  pin.b = b;
}


bool KeepCube::begin() {
  pinMode(pin.r, OUTPUT);
  pinMode(pin.g, OUTPUT);
  pinMode(pin.b, OUTPUT);

  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();


}




// void addToLEDpattern() {

// }


void KeepCube::updateLED() {
  // if (numberOfPoints == 0) {

  //     return;
  // }


  //rgb colorArray[numberOfPoints];

  for (int i = 0; i < numberOfPoints; i++) {
    char hex[6];
    String a;
    i == 0 ? a = colorString.substring(0, 6) : a = colorString.substring(6 * i, 6 * i + 6);

    for (int i = 0; i < 6; i++) {
      hex[i] = a.charAt(i);
    }

    rgb color;
    long number = strtol(&hex[0], NULL, 16);
    color.r = number >> 16;
    color.g = number >> 8 & 0xFF;
    color.b = number & 0xFF;

    led(color, transitionDuration);
    delay(litDuration);


  }

}




void KeepCube::setTransitionParameters(int L, int T, int D, String P) {
  numberOfPoints = L;
  transitionDuration = T;
  litDuration = D;
  colorString = P;
}



int KeepCube::calculateStep(int startVal, int endVal) {
  int step = endVal - startVal;
  if (step) {
    step = loops / step;
  }
  return step;
}


int KeepCube::calculateVal(int step, int val, int i) {
  if ((step) && i % step == 0) {
    if (step > 0) {
      val += 1;
    }
    else if (step < 0) {
      val -= 1;
    }
  }

  if (val > 255) {
    val = 255;
  }
  else if (val < 0) {
    val = 0;
  }

  return val;
}


void KeepCube::led(byte r, byte g, byte b, int time) {
  rgb color;
  color.r = r;
  color.g = g;
  color.b = b;

  led(color, time);
}

void KeepCube::led(rgb color, int time) {

  if (time == 0) {
    analogWrite(pin.r, color.r);
    analogWrite(pin.g, color.g);
    analogWrite(pin.b, color.b);

    return;
  }
  else if (time < 160) {
    time = 1;
  }
  else {
    // sem prijde neco na upravu toho delaye, zatim se na to sere
    // time /= 1.075; // bere se v potaz pomalost atmegy
    time -= 155;
  }

  struct step_t {
    int r;
    int g;
    int b;
  } step;

  step.r = calculateStep(prev.r, color.r);
  step.g = calculateStep(prev.g, color.g);
  step.b = calculateStep(prev.b, color.b);

  for (int i = 0; i <= loops; i++) {
    val.r = calculateVal(step.r, val.r, i);
    val.g = calculateVal(step.g, val.g, i);
    val.b = calculateVal(step.b, val.b, i);

    analogWrite(pin.r, val.r);
    analogWrite(pin.g, val.g);
    analogWrite(pin.b, val.b);

    delayMicroseconds(time);
  }

  prev.r = val.r;
  prev.g = val.g;
  prev.b = val.b;

  delay(hold);

}

