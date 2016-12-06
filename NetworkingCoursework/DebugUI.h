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
*	Messages will be strings that appear on screen and delete after a small moment of time - can be added in main game loop
*
*/


/*
*	Timed Message Struct Keeps track of messages to appear in the "Message Box"
*	After an elpased time will remove them and no longer be displayed
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

/*
*	The Text Colour
*/
enum DebugColour {kWhiteColour, kRedColour, kYellowColour, kBlueColour};



class DebugUI
{
public:
	DebugUI();
	~DebugUI();

	
	/*
	*	Initialsier
	*
	*	@param Font File name for the font to be used
	*   @param the positon of the message box x/y
	*/
	void Init(std::string FontFileName, int MessageBoxPosX, int MessageBoxPosY);

	/*
	*	Render funciton
	*
	*	@param Pointer to the SFML window
	*/
	void Render(sf::RenderWindow* Window);

	void Update();
	/*
	*	Add To Debug Screen is an initialiser funciton that will display text on the screen.
	*	It will return a pointer to a Text object that can be update to change what is said on the display in run time.
	*
	*	@param The Starting String
	*	@param the x positon of the text display
	*	@param the y positon of the text display
	*	@param the Colour of the Text
	*/
	sf::Text* AddToDebugScreen(std::string InitialString, int XPos, int YPos, DebugColour Colour = kWhiteColour);

	/*
	*	Add message Will add a once off message to be displayed in the message box
	*
	*	@param The message to be displayd
	*	@param The Colour
	*	@param the time the message will stay on the screen - Set to default if not overriden
	*/
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

