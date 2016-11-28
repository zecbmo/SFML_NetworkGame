#include "NetworkedCharacter.h"



NetworkedCharacter::NetworkedCharacter()
{
}


NetworkedCharacter::~NetworkedCharacter()
{
}

void NetworkedCharacter::UpdatePosition(float x, float y)
{
	m_Sprite.setPosition(sf::Vector2f(x,y));	
}
