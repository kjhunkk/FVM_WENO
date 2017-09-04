#include "DataType.h"

Alert::Alert()
{

}

Alert::~Alert()
{

}

void Alert::error(const std::string& str, const std::string& file, int_t line)
{
	std::cout << "##########ERROR DETECTED##########\n";
	std::cout << "MESSAGE : " << str << "\n";
	std::cout << "FILE    : " << file << "\n";
	std::cout << "LINE    : " << line << "\n";
	exit(EXIT_FAILURE);
}

void Alert::message(const std::string& str)
{
	std::cout << "## " << str << "\n";
}

Alert Alert::_alert;