#include "Bomb.h"
#include "ErrorSystem.h"


Bomb::Bomb(sf::Vector2f Position, float TimeToExplode)
{
	m_IsExploded = false;
	m_TimeToExplode = TimeToExplode;


	//Load the texture and apply it to the player Sprite - SHOULDN't be Done in Run time		
	if (!m_Texture.loadFromFile("Assets/Textures/Bomb.png"))
	{
		error::StringError("Assets/Textures/Bomb.png", "Unable to load image in player character");
	}
	sf::Vector2f Offset(7,16);

	//Apply texture and scale it#
	m_TextureWidth = 240;
	m_Sprite.setPosition(sf::Vector2f(0, 0));
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setTextureRect(sf::IntRect(0, 0, m_TextureWidth, 288));
	m_Sprite.setScale(sf::Vector2f(.25f, .25f));
	m_Sprite.setPosition(Position + Offset);

	//animating 
	m_CurrentSpritePos = 0;
	m_FramesPerSecond = 10;
	m_AnimCounter = 0;
}


Bomb::~Bomb()
{
}

void Bomb::Update(float dt, float CurrentTime)
{
	if (CurrentTime > m_TimeToExplode)
	{
		Explode();
		m_IsExploded = true;
	}
	else if (CurrentTime > (m_TimeToExplode - 0.75f) )//flash if about to explode 
	{
		if (m_AnimCounter > 1)
		{
			if (m_CurrentSpritePos == 0)
			{
				m_CurrentSpritePos = m_TextureWidth;
			}
			else
			{
				m_CurrentSpritePos = 0;
			}		

			m_AnimCounter = 0;
		}

		m_Sprite.setTextureRect(sf::IntRect(m_CurrentSpritePos, 0, m_TextureWidth, 288));

		m_AnimCounter += m_FramesPerSecond*dt;
	}

}

void Bomb::Render(sf::RenderWindow * Window)
{
	Window->draw(m_Sprite);
}

void Bomb::Explode()
{

}
