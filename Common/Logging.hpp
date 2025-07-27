#pragma once

#include <iostream>

void logError(std::string tag, std::string log);
void logDebug(std::string tag, std::string log);
void logInfo(std::string tag, std::string log);

/* The idea is to migrate these to a better interface,
   but for now this is a terminal based game. */
void logEssential(std::string log);
