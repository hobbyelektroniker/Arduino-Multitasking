#include "arduino_multitasking.h"

// Zustandskonstanten, optional, wird nur bei komplexeren Projekten benötigt
enum States {
  AUS
};

class DemoComponent : public Component {
  public:

    // Variablen (optional)

    // Funktionen (optional)
    void start() {}

    // setup() kann auch Parameter besitzen
    void setup() {}  

    // loop() bestimmt das Verhalten
    void loop() {}
};

// Komponenten erstellen
DemoComponent demo;
 
void setup() {
  // Setup der Applikation
  Serial.begin(9600);

  // Setup der Komponenten
  demo.setup();

  // Komponenten dem Scheduler hinzufügen
  scheduler.add(demo);

  // Den Komponenten Aufgaben zuweisen
  demo.start();
}

void loop() {
  scheduler.loop();  // Dieser Aufruf muss immer vorhanden sein!
  // Hier sind weitere Ergänzungen möglich
}
