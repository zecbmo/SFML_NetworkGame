#pragma once
#include<SFML\Graphics.hpp>
#include "ErrorSystem.h"
#include <list>

#define MESSAGE_DELETE_DELAY 4
#define PARAGRAH_SPACING 25

/*	THE DEBUG CLASS
*
*	The Debug class was created to help display Text on the screen
*	Rather than creating alot of text objects and worrying about Wheter they should render or not
*	The debug class will manage this for us.
*	Add Text to the debug screen should be done in the init functions (WARNING: NEVER IN WHILE LOOP)
*	It will return a pointer to a text object that you can manage
*	This is pushed on to a list of Text objects that the debug class will render
*
*
*	Messages will be strings that appear on scree an delete after a small moment of time
*
*/

struct TimedMessage
{
	TimedMessage(sf::Time DeleteTime, sf::Text Message)
	{
		m_DeleteTime = DeleteTime;
		m_Message = Message;
	}
	sf::Time m_DeleteTime;
	sf::Text m_Message;
};

enum DebugColour {kWhiteColour, kRedColour, kYellowColour, kBlueColour};
class DebugUI
{
public:
	DebugUI();
	~DebugUI();

	
	
	void Init(std::string FontFileName, int MessageBoxPosX, int MessageBoxPosY);

	void Render(sf::RenderWindow* Window);
	void Update();
	sf::Text* AddToDebugScreen(std::string InitialString, int XPos, int YPos, DebugColour Colour = kWhiteColour);
	void AddMessage(std::string Message, DebugColour Colour = kWhiteColour, float TimeDelay = MESSAGE_DELETE_DELAY);

private:
	sf::Clock m_DebugClock;
	sf::Font m_Font;
	std::list<sf::Text*> m_TextList;
	std::list<TimedMessage*> m_MessageList;

	//Message "Box" settings
	int m_MessageBoxPosX;
	int m_MessageBoxPosY;

	void DeleteOldMessages();
	void DisplayMessages(sf::RenderWindow* Window);



};

