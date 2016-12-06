#ifndef ERROR_SYSTEM_
#define ERROR_SYSTEM_

#include <windows.h>
#include <stdio.h>
#include <string>

/**
*	The Windows Based Error System
*
*	For quickly showing errors incase of out of range values etc
*	allows for custom prompts and ID sortin
*/

namespace error
{
	void ValueError(std::string id, std::string message, float value, bool destroy_window = true);
	void ValueError(std::string id, std::string message, int value, bool destroy_window = true);
	void ErrorMessage(std::string message, bool destroy_window = true);
	void StringError(std::string id, std::string message, bool destroy_window = true);
}

#endif