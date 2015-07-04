#pragma once

#include <iosfwd>


#define LOG_FILE "/tmp/run.log"

class Logger {
public:
	static void write_log(const std::string& _msg);

private:
	static std::ofstream logFile;
	static bool clean;
};
