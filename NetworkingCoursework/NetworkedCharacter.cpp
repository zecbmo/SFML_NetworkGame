#include "NetworkedCharacter.h"



NetworkedCharacter::NetworkedCharacter()
{
}


NetworkedCharacter::~NetworkedCharacter()
{
}

void NetworkedCharacter::Update(float dt, float GameTime)
{
	float Alpha = GameTime / m_ExpectedTimeToReachEnd;
	m_LerpStartPos = LerpFunction(m_LerpStartPos, m_LerpEndPos, Alpha);

	UpdateSpriteState(dt);

	m_Sprite.setPosition(m_LerpStartPos);
	
}

void NetworkedCharacter::AddToPredictionList(float x, float y, float TimeStamp, sf::Clock* Clock)
{
	
	//Create new Timstruct
	TimePositionStruct NewValue;
	NewValue.Position = sf::Vector2f(x, y);
	NewValue.ServerTimeStampInSeconds = TimeStamp;

	//Find where it should go in the list
	for (int i = 0; i < 2; i++)
	{
		float TimeCheck = m_PredictionList[i].ServerTimeStampInSeconds;

		if (TimeStamp > TimeCheck)
		{
			if (i == 0)
			{
				TimePositionStruct Reorder = m_PredictionList[i];
				m_PredictionList[1] = Reorder;
				m_PredictionList[0] = NewValue;
			}
			else
			{
				m_PredictionList[i] = NewValue;
			}
		}

	
	}

	UpdateLerpValues(Clock);
}

void NetworkedCharacter::UpdateLerpValues(sf::Clock* Clock)
{
	//The Current position will be the Starting point of the Lerp
	m_LerpStartPos = m_Sprite.getPosition();

	//Get the difference betweeb the two positions
	sf::Vector2f Pos1 = m_PredictionList[0].Position;
	sf::Vector2f Pos2 = m_PredictionList[1].Position;
	
	m_LerpEndPos = Pos1 + (Pos2 - Pos1);

	//Get the time difference between the messages
	float TimeDifferenceBetweenMessages = m_PredictionList[0].ServerTimeStampInSeconds - m_PredictionList[1].ServerTimeStampInSeconds;

	m_ExpectedTimeToReachEnd = Clock->getElapsedTime().asSeconds() + TimeDifferenceBetweenMessages;

}
sf::Vector2f NetworkedCharacter::LerpFunction(sf::Vector2f Start, sf::Vector2f End, float Alpha)
{
	return Start + (End - Start) * Alpha;
}
