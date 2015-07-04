#include "Debug.hpp"

#include <string>
#include <iostream>


#ifdef DEBUG
	#include "Timer.hpp"
	#include <cassert>

	//TMP SOLUTION, THIS SHOULDNT BE HERE, for gl_error_print
	#include <GL/glew.h>
	#include <SFML/OpenGL.hpp>


	void debug_print(const std::string& _msg, const std::string& _func,const std::string& _file,int _line)
	{
		std::cerr << "DEBUG: " << _func << " " << _file << " " << _line  << std::endl;
		std::cerr << " " << _msg << std::endl << std::endl;
	}

	void stimer(const std::string& _name)
	{
		Timer::start(_name);
	}

	void etimer(const std::string& _name)
	{
		Timer::end(_name);
	}

	void gl_error_print(const std::string& _func,const std::string& _file,int _line)
	{
		GLenum glErr;

		glErr = glGetError();
		if (glErr != GL_NO_ERROR)
		{
			std::cerr << "GLError: " << _func << " " << _file << " " << _line << std::endl;
			std::cerr << " " << gluErrorString(glErr) << std::endl << std::endl;
		}
	}
#else
	void debug_print(const std::string& _msg, const std::string& _func,const std::string& _file,int _line) { }
	void stimer(const std::string& _name) { }
	void etimer(const std::string& _name) { }
	void gl_error_print(const std::string& _func,const std::string& _file,int _line) { }
#endif

void error_print(const std::string& _msg, const std::string& _func,const std::string& _file,int _line)
{
	std::cerr << "Error: " << _func << " " << _file << " " << _line  << std::endl;
	std::cerr << " " << _msg << std::endl << std::endl;
}
