#include "Logger.hpp"

#include <iostream>
#include <fstream>
#include <ctime>

void Logger::write_log(const std::string& _msg)
{
	if (clean)
	{
		Logger::logFile.open(LOG_FILE);
		Logger::clean = false;
	}
	else
		Logger::logFile.open(LOG_FILE, std::fstream::app);

	time_t epoch = time(0);

	tm* lt = localtime(&epoch);

	Logger::logFile << "(LOG " << lt->tm_hour << ":" << lt->tm_sec << ") " << _msg << std::endl;
	Logger::logFile.close();
	
}

std::ofstream Logger::logFile;
bool Logger::clean = true;
