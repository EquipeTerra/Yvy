#ifndef THRUSTERSCTRL_H
#define THRUSTERSCTRL_H

#include "utils.h"

// Defines the power maximum that thrusters can receive (in %)
const double POWER_MAX = 25;

// Frequency used for the PWM signal
const double FREQUENCY = 200;

/** Thrusters pins
 * 0 - Front left 
 * 1 - Front right
 * 2 - Middle right
 * 3 - Middle left
 * 4 - Back right
 * 5 - Back left 
 */
const array<int, 6> PINS = {17, 18, 22, 23, 24, 27};

/**
 * @brief Struct representing a decision for thruster action
 */
struct Decision{
    Action action = Action::NONE;
    int value = -1;
};

/**
 * @brief Class representing a single thruster
 */
class Thruster{
    private:
        int pin, stablePower, currentPower;
        int minPWMus = 1000, maxPWMus = 2000;
        int pwmRange = 1000;

    public:
        /**
         * @brief Constructs a new Thruster object.
         * 
         * @param pin Pin connected to the thruster.
         * @param stablePower Power for this thruster to stabilize the AUV (Vx: 0, Vy: 0, Vz: 0).
         */
        Thruster(int pin, int stablePower);

        /**
         * @brief Sets a PWM value into dutycycle value from a percentage value.
         * 
         * @param value A percentage value from -100 to 100.
         */
        void move(double value);

        /**
         * @brief Converts a percentage value to a dutycycle value.
         * 
         * @param value A percentage value from -100 to 100.
         * 
         * @return Corresponding dutycycle value.
         */
        int percentageToDutycycle(double value);

        /**
         * @brief Turns off the thruster.
         * 
         * @details Sets the duty cycle to 0.
         */
        void finishesThruster();
};

/**
 * @brief Class representing the control of multiple thrusters
 */
class ThrustersControl{
    private:
        vector<Thruster> thrusters;
        Action action = Action::NONE;

        bool stabilizeVert = true, stabilizeHori = true;

    public:
        /**
         * @brief Constructs a new ThrustersControl object.
         * 
         * Activates the wiringPi service and initializes each thruster.
         */
        ThrustersControl();

        /**
         * @brief Initializes each AUV thruster.
         * 
         * Creates each AUV thruster, adds each one to the thrusters vector, and waits 7 seconds for the thrusters to start.
         */
        void initializeThrusters();

        /**
         * @brief Defines the action for the thrusters based on the given decision.
         * 
         * @param decision Struct with the action decision that will be executed by the AUV and with what power.
         */
        void defineAction(Decision decision);

        bool getStabilizeVert();

        bool getStabilizeHori();

        /**
         * @brief Gets the current action of the thrusters.
         * 
         * @return The current action of the thrusters.
         */
        Action getAction();

        /**
         * @brief Finish the thrusters.
         */
        void finish();
};

#endif //THRUSTERSCTRL_H