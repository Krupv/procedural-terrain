#pragma once

#include <map>
#include <string>

#include "Logger.hpp"


//TIMER SINGLETON
//TODO: make it thread safe

class Stopwatch {
public:
	Stopwatch(const std::string& _msg);
	~Stopwatch();

	void start();
	std::string stop();

private:
	long m_start,m_end;
	std::string m_msg;
};

class Timer {

public:
	static void start( const std::string& _name);
	static void end( const std::string& _name);

private:
	static std::map<std::string, Stopwatch> m_timers;
};
