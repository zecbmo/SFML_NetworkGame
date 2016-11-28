#include "ErrorSystem.h"

void error::ValueError(std::string id, std::string message, float value, bool destroy_window)
{
	char buffer[255];
	sprintf_s(buffer, "%s %s : %.2f", id.c_str(), message.c_str(), value);
	MessageBoxA(NULL, buffer, "Idiot Programmer", MB_OK);


	if (destroy_window)
	{
		//DestroyWindow(GetActiveWindow());
		exit(1);
	}
}
void error::ValueError(std::string id, std::string message, int value, bool destroy_window)
{
	char buffer[255];
	sprintf_s(buffer, "%s %s : %d", id.c_str(), message.c_str(), value);
	MessageBoxA(NULL, buffer, "Idiot Programmer", MB_OK);

	if (destroy_window)
	{
		//DestroyWindow(GetActiveWindow());
		exit(1);
	}
}
void error::ErrorMessage(std::string message, bool destroy_window)
{
	char buffer[255];
	sprintf_s(buffer, "%s", message.c_str());
	MessageBoxA(NULL, buffer, "Idiot Programmer", MB_OK);


	if (destroy_window)
	{
		//DestroyWindow(GetActiveWindow());
		exit(1);
	}
}
void error::StringError(std::string id, std::string message, bool destroy_window)
{
	char buffer[255];
	sprintf_s(buffer, "%s: %s", id.c_str(), message.c_str());
	MessageBoxA(NULL, buffer, "Idiot Programmer", MB_OK);

	if (destroy_window)
	{
		//DestroyWindow(GetActiveWindow());
		exit(1);
	}
}