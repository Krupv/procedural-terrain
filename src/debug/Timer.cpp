#include "Timer.hpp"

#include <ctime>
#include <iostream>


// STOPWATCH
Stopwatch::Stopwatch(const std::string& _msg) 
{
	m_msg = _msg;
	start();
}

Stopwatch::~Stopwatch() {};

void Stopwatch::start() 
{
	m_start = std::clock();
}

std::string Stopwatch::stop()
{
	m_end = std::clock();
	double time = (double) (m_end - m_start) / CLOCKS_PER_SEC;
	std::string msg = std::string("Timer: ") + m_msg + " took " + std::to_string(time) + " sec";
	return msg;
}




// TIMER
void Timer::start( const std::string& _name)
{
	auto it = m_timers.find(_name);
	if (  it != m_timers.end() )
	{
		std::cerr << "DEBUG> error: Timer named " << _name << " already exists, not creating a new one" << std::endl;
		return;
	}

	Stopwatch watch(_name);
	m_timers.insert(std::pair<std::string, Stopwatch>(_name, watch));
}

void Timer::end( const std::string& _name)
{
	auto it = m_timers.find(_name);
	if (  it == m_timers.end() )
	{
		std::cerr << "DEBUG> error: Timer named " << _name << " not found" << std::endl;
		return;
	}
	
	Logger::write_log(it->second.stop());
	m_timers.erase(it);
}

std::map<std::string, Stopwatch> Timer::m_timers;
