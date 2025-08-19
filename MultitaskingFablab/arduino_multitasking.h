#pragma once
#include <arduino.h>
#define MAX_THREADS 20

extern bool isTimeOver(unsigned long startTime, unsigned long waitTime);
extern void _delay(unsigned long ms);


class Component {
  public:
    Component();
    void setInterval(unsigned long interval);
    void active(bool activate);
    bool active() const;
    void update();
    void wait(unsigned long ms);
    void changeState(int newState);

    virtual void loop();

    int state;
    int fromState;

  protected:
    unsigned long _interval;

  private:
    bool _active;
    unsigned long _prevExecute;
    unsigned long _delay;

};


class Scheduler {
public:
    Scheduler();

    bool add(Component& thread);
    void loop();

private:
    Component* _threads[MAX_THREADS];
    int _count;
};

extern Scheduler scheduler;
