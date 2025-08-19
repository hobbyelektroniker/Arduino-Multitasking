#include "arduino_multitasking.h"

enum States {
  AUS, GRUEN, GELB, ROT, ROT_GELB
};

class Ampel : public Component {
  public:
    
    int _rot, _gelb, _gruen;

    void setup(int rot, int gelb, int gruen) {
      _rot = rot;
      _gelb = gelb;
      _gruen = gruen;
      pinMode(_rot, OUTPUT);
      pinMode(_gelb, OUTPUT);
      pinMode(_gruen, OUTPUT);
      state = AUS;
    }  

    void loop() {
      switch (state) {

        case AUS:
          digitalWrite(_gruen, LOW);
          digitalWrite(_gelb, LOW);
          digitalWrite(_rot, LOW);
          break;

        case GRUEN:
          digitalWrite(_gruen, HIGH);
          digitalWrite(_gelb, LOW);
          digitalWrite(_rot, LOW);
          wait(6000);
          state = GELB;
          break;

        case GELB:
          digitalWrite(_gruen, LOW);
          digitalWrite(_gelb, HIGH);
          digitalWrite(_rot, LOW);
          wait(3000);
          state = ROT;
          break;

        case ROT:
          digitalWrite(_gruen, LOW);
          digitalWrite(_gelb, LOW);
          digitalWrite(_rot, HIGH);
          wait(8000);
          state = ROT_GELB;
          break;

        case ROT_GELB:
          digitalWrite(_gruen, LOW);
          digitalWrite(_gelb, HIGH);
          digitalWrite(_rot, HIGH);
          wait(2000);
          state = GRUEN;
          break;
      }
    }
};

class BlinkLed : public Component {
  public:

    int blinkDelay;
    int _pin;

    void setup(int pin) {
      _pin = pin;
      pinMode(_pin, OUTPUT);
      digitalWrite(_pin, LOW);
      blinkDelay = 0;
    }  

    void loop() {
      if (blinkDelay) {
        wait(blinkDelay);
        digitalWrite(_pin, !digitalRead(_pin));
      } else {
        digitalWrite(_pin, LOW);  
      }
    }
};

BlinkLed roteLed;
BlinkLed blaueLed;
Ampel ampel;
 
void setup() {
  // setup der Komponenten
  roteLed.setup(2);
  blaueLed.setup(3);
  ampel.setup(11, 12, 13);

  // Komponenten zum Scheduler hinzufügen
  scheduler.add(roteLed);
  scheduler.add(blaueLed);
  scheduler.add(ampel);

  // Aufgaben verteilen
  roteLed.blinkDelay = 500;
  blaueLed.blinkDelay = 1200;
  ampel.state = ROT;
}

void loop() {
  scheduler.loop();
}
