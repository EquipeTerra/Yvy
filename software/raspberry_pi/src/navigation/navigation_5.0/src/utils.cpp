#include "utils.h"

string actionToString(Action action){
    if(Action::FORWARD == action) return "FORWARD";
    else if(Action::BACKWARD == action) return "BACKWARD";
    else if(Action::RIGHT == action) return "RIGHT";
    else if(Action::LEFT == action) return "LEFT";
    else if(Action::UP == action) return "UP";
    else if(Action::DOWN == action) return "DOWN";
    else if(Action::TURNRIGHT == action) return "TURNRIGHT";
    else if(Action::TURNLEFT == action) return "TURNLEFT";
    else if(Action::NONE == action) return "NONE";
    else return "UNKNOWN";
}
