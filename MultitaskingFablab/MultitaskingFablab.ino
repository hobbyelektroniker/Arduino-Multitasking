#include "arduino_multitasking.h"

// Zustandskonstanten, optional, wird nur bei komplexeren Projekten benötigt
enum States {
  AUS, GRUEN, GELB, ROT, ROT_GELB
};

class BlinkLed : public Component {
  public:
    int _pin;
    int blinkDelay;

    void on() {
      digitalWrite(_pin, HIGH);
    }

    void off() {
      digitalWrite(_pin, LOW);
    }

    void blink(int delayMs) {
      blinkDelay= delayMs;
    }

    void setup(int pin) {
      _pin = pin;
      pinMode(_pin, OUTPUT);
      off();
    }  

    void loop() {
      if (blinkDelay) {
        digitalWrite(_pin, !digitalRead(_pin));
        wait(blinkDelay);
      }
    }
};

class Ampel : public Component {
  public:
    BlinkLed rot, gelb, gruen;

    void setup(int rotPin, int gelbPin, int gruenPin) {
      rot.setup(rotPin);
      gelb.setup(gelbPin);
      gruen.setup(gruenPin);
      scheduler.add(rot);
      scheduler.add(gelb);
      scheduler.add(gruen);
      state = AUS;
    }  

    void loop() {
      switch (state) {

        case AUS:
          gruen.off();
          gelb.off();
          rot.off();
          break;

        case ROT:
          gruen.off();
          gelb.off();
          rot.on();
          state = ROT_GELB;
          wait(8000);
          break;     

        case ROT_GELB:
          gruen.off();
          gelb.on();
          rot.on();
          wait(2000);
          state = GRUEN;
          break;

        case GRUEN:
          gruen.on();
          gelb.off();
          rot.off();
          wait(6000);
          state = GELB;
          break;

        case GELB:
          gruen.off();
          gelb.on();
          rot.off();
          wait(3000);
          state = ROT;
          break;      
      }
    }
};    


// Komponenten erstellen
BlinkLed roteLed;
BlinkLed blaueLed;
Ampel ampel;

void setup() {
  // Setup der Applikation
  Serial.begin(9600);

  // Setup der Komponenten
  roteLed.setup(2);
  blaueLed.setup(3);
  ampel.setup(11, 12, 13);

  // Komponenten dem Scheduler hinzufügen
  scheduler.add(roteLed);
  scheduler.add(blaueLed);
  scheduler.add(ampel);

  // Den Komponenten Aufgaben zuweisen
  roteLed.blinkDelay = 500;
  blaueLed.blinkDelay = 1200;
  ampel.state = ROT;
}

void loop() {
  scheduler.loop();  // Dieser Aufruf muss immer vorhanden sein!
  // Hier sind weitere Ergänzungen möglich
}
