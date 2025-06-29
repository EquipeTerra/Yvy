#ifndef UTILS_H
#define UTILS_H

#include <pigpio.h>
#include <csignal>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <mutex>
#include "auverror.h"

/**
 * Enum representing the actions the AUV can perform.
 * 
 * Actions:
 * 
 * - UP         : Move the AUV up, turning on the thrusters 3 and 6 in the forward direction.
 * - DOWN       : Move the AUV down, turning on the thrusters 3 and 6 in the reverse direction.
 * - FORWARD    : Move the AUV forward, turning on the thrusters 1 and 2 in the reverse direction and the thrusters 4 and 5 in the forward direction.
 * - BACKWARD   : Move the AUV backward, turning on the thrusters 1 and 2 in the forward direction and the thrusters 4 and 5 in the reverse direction.
 * - RIGHT      : Move the AUV right, turning on the thrusters 2 and 4 in the forward direction and the thrusters 1 and 5 in the reverse direction.
 * - LEFT       : Move the AUV left, turning on the thrusters 1 and 5 in the forward direction and the thrusters 2 and 4 in the reverse direction.
 * - TURN RIGHT : Turn the AUV right, turning on the thrusters 2 and 5 in the forward direction and the thrusters 1 and 4 in the reverse direction.
 * - TURN LEFT  : Turn the AUV left, turning on the thrusters 1 and 4 in the forward direction and the thrusters 2 and 5 in the reverse direction.
 * - NONE       : No action.
 */
enum class Action{
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT,
    UP,
    DOWN,
    TURNRIGHT,
    TURNLEFT,
    NONE
};

/**
 * @brief Converts the action enum to a string.
 * 
 * @param action The action to convert to string.
 * 
 * @return The action in string format.
 */
string actionToString(Action action);

#endif // UTILS_H
