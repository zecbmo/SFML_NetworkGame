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

void NetworkedCharacter::Update(float dt)
{
	//Get Expected Location 
	//Lerp to Expected location
	//change Direction passed on direction of travel/Rather than key press

	UpdateSpriteState(dt);
}

void NetworkedCharacter::PredictNextMovement(sf::Clock &Clock)
{

	//Get CurrentTime ---- This allows us to get the alpha (for how far we should predict ahead)

	//Math here from two latest positions

	//Movement returned is the exptect movement time
}
