#include "activator.h"

Activator::Activator(int pin) : pinNumber(pin) {
    if (gpioInitialise() < 0) throw FailedConnectWiringPi();
    gpioSetMode(pinNumber, PI_INPUT);
}

void Activator::WaitingForActivation() {
    // Esse pino precisa de um resistor de pull-up
    int value = gpioRead(pinNumber);
    while(value){
        int value = gpioRead(pinNumber);
        cout << "Waiting for activation: " << value << endl;
        sleep_for(milliseconds(100));
    }
}