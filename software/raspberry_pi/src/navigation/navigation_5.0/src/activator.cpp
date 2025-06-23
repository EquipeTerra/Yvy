#include "activator.h"

Activator::Activator(int pin) : pinNumber(pin) {
    if (gpioInitialise() < 0) throw FailedConnectWiringPi();
    gpioSetMode(pinNumber, PI_INPUT);
}

void Activator::WaitingForActivation() {
    // Esse pino precisa de um resistor de pull-up
    while(gpioRead(pinNumber)){
        sleep_for(milliseconds(100));
    }
}