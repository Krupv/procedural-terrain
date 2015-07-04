#pragma once

#include <string>


void debug_print(const std::string& _msg, const std::string& _func,const std::string& _file,int _line);
void stimer(const std::string& _name);
void etimer(const std::string& _name);
void gl_error_print(const std::string& _func,const std::string& _file,int _line);

void error_print(const std::string& _msg, const std::string& _func,const std::string& _file,int _line);
