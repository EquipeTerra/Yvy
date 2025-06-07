#ifndef LOGS_H
#define LOGS_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;
using namespace chrono;
using namespace this_thread;

/**
 * @brief Logs a message to the console or file based on the mode.
 * 
 * @param message The message to log.
 */
void logMessage(std::string message);

/**
 * @brief Logs a message to a file.
 * 
 * @param message The message to log.
 */
void logToFile(std::string message);

/**
 * @brief Gets the current time as a string.
 * 
 * @return The current time in string format.
 */
string getTime();

#endif // LOGS_H