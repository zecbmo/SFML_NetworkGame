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
	m_Sprite.setPosition(sf::Vector2f(0,0));
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setTextureRect(sf::IntRect(0, m_Sprite_Y_Pos, m_Texture_Width, m_Texture_Height));
	m_Sprite.setScale(sf::Vector2f(5.f, 5.f));
	m_Sprite.setPosition(StartingPos);

	//Set the initial Vars for the player
	m_Speed = 100.0f;
	m_Colour = Colour;

	//Set Ref to Debug UI
	m_Debug = &DebugScreen;
	PlayerPositionText = m_Debug->AddToDebugScreen("player position", 0, 40, kYellowColour);

	//Animated Sprite Setup
	m_FramesPerSecond = 8;
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

	m_Position = pos;

}

void PlayerCharacter::Movement(float dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_Sprite.move(-m_Speed * dt, 0);
		m_Dir = kWest;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{	
		m_Sprite.move(m_Speed* dt, 0);
		m_Dir = kEast;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_Sprite.move(0, m_Speed * dt);
		m_Dir = kSouth;
	}
	else  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_Sprite.move(0, -m_Speed* dt);
		m_Dir = kNorth;
	}
	else
	{
		m_Dir = kStill;
	}


	UpdateSpriteState(dt);
}

void PlayerCharacter::Render(sf::RenderWindow* Window)
{
	Window->draw(m_Sprite);	
}

void PlayerCharacter::UpdateSprite(int SpritePos, float dt)
{
	
	if (m_AnimCounter > 1)
	{
		if (SpritePos == 0)
		{
			m_CurrentSpritePos = 0;
		}
		else if (SpritePos == m_CurrentSpritePos)
		{
			m_CurrentSpritePos = SpritePos + m_Texture_Width;
		}
		else
		{
			m_CurrentSpritePos = SpritePos;
		}
	
		m_AnimCounter = 0;

	}

	m_Sprite.setTextureRect(sf::IntRect(m_CurrentSpritePos, m_Sprite_Y_Pos, m_Texture_Width, m_Texture_Height));

	m_AnimCounter += m_FramesPerSecond*dt;
}


void PlayerCharacter::UpdateSpriteState(float dt)
{

	switch (m_Dir)
	{
	case kNorth:
		UpdateSprite(43, dt);
		break;
	case kSouth:
		UpdateSprite(14, dt);
		break;
	case kEast:
		UpdateSprite(98, dt);
		break;
	case kWest:
		UpdateSprite(70, dt);
		break;
	case kNorthEast:
		UpdateSprite(98, dt);
		break;
	case kNorthWest:
		UpdateSprite(70, dt);
		break;
	case kSouthEast:
		UpdateSprite(98, dt);
		break;
	case kSouthWest:
		UpdateSprite(70, dt);
		break;
	case kStill:
		UpdateSprite(0, dt);
		break;
	default:
		break;
	}
}