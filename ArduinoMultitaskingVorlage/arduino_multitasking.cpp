#include "arduino_multitasking.h"

Scheduler scheduler;

// Zeitvergleichsfunktion ohne Überlaufproblem
bool isTimeOver(unsigned long startTime, unsigned long waitTime) {
  return ((unsigned long)(millis() - startTime) >= waitTime);
}

// nicht blockierender delay
void _delay(unsigned long ms) {
  unsigned long end = millis() + ms;
  while ((long)(millis() - end) < 0) {
    scheduler.loop();
  }
}

Component::Component() 
  : _interval(50), _active(true), _prevExecute(0), _delay(0), 
    state(0) {}

void Component::setInterval(unsigned long interval) {
  _interval = interval;
}

void Component::active(bool activate) {
  _active = activate;
  if (activate) {
    _prevExecute = 0;
    _delay = 0;
  }
}

void Component::loop() {}

bool Component::active() const {
  return _active;
}

void Component::changeState(int newState) {
  fromState = state;
  state = newState;
}

void Component::update() {
  if (!_active) return;

  if (_delay && !isTimeOver(_prevExecute, _delay)) {
      return;
  }
  _delay = 0;

  if (isTimeOver(_prevExecute, _interval)) {
    _prevExecute = millis();
    loop();
  }
}

void Component::wait(unsigned long ms) {
  _delay = ms;
}


Scheduler::Scheduler() : _count(0) {}

bool Scheduler::add(Component& thread) {
  if (_count >= MAX_THREADS) return false;
  _threads[_count++] = &thread;
  return true;
}

void Scheduler::loop() {
  for (int i = 0; i < _count; i++) {
      if (_threads[i]->active()) {
          _threads[i]->update();
      }
  }
}

