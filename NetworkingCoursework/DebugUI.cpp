#include "DebugUI.h"



DebugUI::DebugUI()
{
}


DebugUI::~DebugUI()
{
	m_TextList.clear();
	m_MessageList.clear();

}

void DebugUI::Init(std::string FontFileName, int MessageBoxPosX, int MessageBoxPosY)
{

	//if the file name is not found throw error
	if (!m_Font.loadFromFile(FontFileName))
	{
		error::StringError(FontFileName, "Unable to load Font in DebugUI");
	}

	m_MessageBoxPosX = MessageBoxPosX;
	m_MessageBoxPosY = MessageBoxPosY;

}

sf::Text* DebugUI::AddToDebugScreen(std::string InitialString, int XPos, int YPos, DebugColour Colour)
{
	sf::Text* NewText = new sf::Text();
	NewText->setFont(m_Font);
	NewText->setString(InitialString);
	NewText->setPosition(sf::Vector2f((float)XPos, (float)YPos));

	switch (Colour)
	{
	case kWhiteColour:
		NewText->setFillColor(sf::Color(255, 255, 255, 255));
		break;
	case kRedColour:
		NewText->setFillColor(sf::Color(255, 0, 0, 255));
		break;
	case kYellowColour:
		NewText->setFillColor(sf::Color(255, 255, 0, 255));
		break;
	case kBlueColour:
		NewText->setFillColor(sf::Color(0, 0, 255, 255));
		break;
	default:
		break;
	}

	m_TextList.push_back(NewText);

	return NewText;
}
void DebugUI::AddMessage(std::string Message, DebugColour Colour, float TimeDelay)
{
	sf::Time CurrentTime = m_DebugClock.getElapsedTime();

	sf::Time DeleteTime = sf::seconds(CurrentTime.asSeconds() + TimeDelay);

	sf::Text Text;
	Text.setFont(m_Font);
	Text.setString(Message);
	//NewText->setPosition(sf::Vector2f(XPos, YPos));

	switch (Colour)
	{
	case kWhiteColour:
		Text.setFillColor(sf::Color(255, 255, 255, 255));
		break;
	case kRedColour:
		Text.setFillColor(sf::Color(255, 0, 0, 255));
		break;
	case kYellowColour:
		Text.setFillColor(sf::Color(255, 255, 0, 255));
		break;
	case kBlueColour:
		Text.setFillColor(sf::Color(0, 0, 255, 255));
		break;
	default:
		break;
	}


	TimedMessage* NewMessage = new TimedMessage(DeleteTime, Text);
	m_MessageList.push_front(NewMessage);
	
}

void DebugUI::Render(sf::RenderWindow* Window)
{
	//DisplayMessages(Window);

	
	if (!m_MessageList.empty())
	{
		int ypos = m_MessageBoxPosY;

		for (auto iter : m_MessageList)
		{
			iter->m_Message.setPosition(m_MessageBoxPosX, ypos);
			Window->draw(iter->m_Message);
			ypos -= PARAGRAH_SPACING;
		}
	
	}

	for(auto iter : m_TextList)
	{
		Window->draw(*iter);
	}

}
void DebugUI::Update()
{
	DeleteOldMessages();
}
void DebugUI::DeleteOldMessages()
{
	for (auto iter = m_MessageList.begin(); iter != m_MessageList.end(); /**nothing*/)
	{
		if ((*iter)->m_DeleteTime < m_DebugClock.getElapsedTime())
		{
			iter = m_MessageList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
void DebugUI::DisplayMessages(sf::RenderWindow * Window)
{
	//go backwars so new messages are first (At the bottom)

}
