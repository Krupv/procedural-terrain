#pragma once
#include "../main.hpp"



namespace sf {
	class Window;
};

// class: Display
class DDisplay {
public:
	DDisplay( int _width, int _height, const std::string& _title  );
	~DDisplay();

	void update();
	inline void clear() 
	{
		this->clear(0.f, 0.f, 0.f, 1.f);
	};

	void clear(float _red, float _green, float _blue, float _alpha );

	inline bool isRunning() 
	{
		return m_running;
	}

	//temp
	void makeActive(bool _a);

	unsigned int getWidth() const;
	unsigned int getHeight() const;

	inline bool isPaused() 
	{
		return m_paused;
	}

	inline void pause(bool _state)
	{
		m_running = ! _state;
	}

	sf::Window* getWindowPtr();

	void pollEvents();

private:
	sf::Window* m_window;
	bool m_running;
	bool m_paused;

}; // end of class


