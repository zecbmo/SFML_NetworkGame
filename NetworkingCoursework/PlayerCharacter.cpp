#include "PlayerCharacter.h"
#include "ErrorSystem.h"


PlayerCharacter::PlayerCharacter()
{
}


PlayerCharacter::~PlayerCharacter()
{
}

void PlayerCharacter::Init(int PlayerID, PlayerColour Colour, sf::Vector2f StartingPos, std::string Filename, DebugUI& DebugScreen)
{
	//Set the player ID which keep track on individual players
	m_ID = PlayerID;

	m_Texture_Width = 14;
	m_Texture_Height = 18;

	//Load the texture and apply it to the player Sprite		
	if (!m_Texture.loadFromFile(Filename))
	{
		error::StringError(Filename, "Unable to load image in player character");
	}

	//The texture is tiled horizontally for all chars. 
	//the y position represents there pixel start point for different characters (Look at png for more info)
	switch (Colour)
	{
	case kRed:
		m_Sprite_Y_Pos = 0;
		break;
	case kPurple:
		m_Sprite_Y_Pos = 18;

		break;
	case kBlack:
		m_Sprite_Y_Pos = 36;
		break;
	case kGreen:
		m_Sprite_Y_Pos = 54;
		break;
	}

	//Apply texture and scale it
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setTextureRect(sf::IntRect(0, m_Sprite_Y_Pos, m_Texture_Width, m_Texture_Height));
	m_Sprite.setScale(sf::Vector2f(5.f, 5.f));
	m_Sprite.setPosition(StartingPos);

	//Set the initial Vars for the player
	m_Speed = 100.0f;
	m_Colour = Colour;

	//Set Ref to Debug UI
	m_Debug = &DebugScreen;
	//PlayerPositionText = m_Debug->AddToDebugScreen("player one position", 0, 0, kYellowColour);
}

void PlayerCharacter::Update(float dt)
{

	//Move player
	Movement(dt);

	//update Debug info

	
	sf::Vector2f pos = m_Sprite.getPosition();
	char temp[256];
	sprintf_s(temp, "Player Position : X %.2f Y %.2f", pos.x, pos.y);
	//PlayerPositionText->setString(temp);

}

void PlayerCharacter::Movement(float dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_Sprite.move(-m_Speed * dt, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{	
		m_Sprite.move(m_Speed* dt, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_Sprite.move(0, m_Speed * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_Sprite.move(0, -m_Speed* dt);
	}




}

void PlayerCharacter::Render(sf::RenderWindow* Window)
{
	Window->draw(m_Sprite);
}
