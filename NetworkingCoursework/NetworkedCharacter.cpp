#include "NetworkedCharacter.h"



NetworkedCharacter::NetworkedCharacter()
{
}


NetworkedCharacter::~NetworkedCharacter()
{
}

void NetworkedCharacter::UpdatePosition(sf::Clock& Clock)
{
	//TimePositionStruct ExpectedValues = PredictNextMovement(Clock);
	//LerpToExpectedPosition(ExpectedValues, Clock);

	if (!m_PredictionList.empty())
	{
		auto iter = m_PredictionList.begin();

		m_PredictedX = (*iter)->Position.x;
		m_PredictedY = (*iter)->Position.y;
	}

	m_Sprite.setPosition(sf::Vector2f(m_PredictedX, m_PredictedY));
}

void NetworkedCharacter::UpdateTestPosiiton()
{
	m_Sprite.setPosition(500,500);
}

void NetworkedCharacter::Update(float dt)
{
	UpdateSpriteState(dt);
}

void NetworkedCharacter::AddToPredictionList(float x, float y, float TimeStamp)
{
	//Get Rid Of Values If it is too big
	

	//Create new Timstruct
	TimePositionStruct* NewValue = new TimePositionStruct;
	NewValue->Position = sf::Vector2f(x, y);
	NewValue->ServerTimeStampInSeconds = TimeStamp;

	//Find where it should go in the list
	int Tracker = 0;
	for (auto iter : m_PredictionList)
	{
		float TimeCheck = iter->SenderStampInSeconds;

		if (TimeStamp > TimeCheck)
		{
			//this is the positon we want to insert the list
			break;
		}

		Tracker++;
	}

	//The the new values to the list
	auto IterPosition = m_PredictionList.begin();
	std::advance(IterPosition, Tracker);
	m_PredictionList.insert(IterPosition, NewValue);


}

TimePositionStruct NetworkedCharacter::PredictNextMovement(sf::Clock &Clock)
{
	//get The two latest positions/times
	TimePositionStruct First, Second , Expected;
	GetLatestPositions(First, Second);

	//Get The expected position from the two previous
	sf::Vector2f Velocity =  First.Position - Second.Position;
	sf::Vector2f ExpectedPosition = First.Position + Velocity;

	//Get CurrentTime ---- This allows us to get the alpha (for how far we should predict ahead)
	float CurrentTime = Clock.getElapsedTime().asSeconds();
	//Expected Time to be at Expected Position
	float ExpectedTime = (First.ServerTimeStampInSeconds - Second.ServerTimeStampInSeconds) + CurrentTime;

	//Movement returned is the exptect movement time
	Expected.Position = ExpectedPosition;
	Expected.ServerTimeStampInSeconds = ExpectedTime;

	return Expected;
}

void NetworkedCharacter::GetLatestPositions(TimePositionStruct& OUT First, TimePositionStruct&  OUT Second)
{
	//Possible better way of doing this
	for (auto iter : m_PredictionList)
	{
		if (iter == *m_PredictionList.begin())
		{
			First = *iter;
		}

		if (iter == *m_PredictionList.begin() + 1)
		{
			Second = *iter;
		}
	}
}

void NetworkedCharacter::LerpToExpectedPosition(TimePositionStruct Expected, sf::Clock& Clock)
{
	//get The current position of the sprite
	sf::Vector2f CurrentPosition = m_Sprite.getPosition();

	//get Alpha based on the Expected time and current time
	float Alpha = Clock.getElapsedTime().asSeconds() / Expected.SenderStampInSeconds;

	//Lerp from Current potition to Expected position
	sf::Vector2f NewPos = LerpFunction(CurrentPosition, Expected.Position, Alpha);

	//Set the position
	m_PredictedX = NewPos.x;
	m_PredictedY = NewPos.y;

}

sf::Vector2f NetworkedCharacter::LerpFunction(sf::Vector2f Start, sf::Vector2f End, float Alpha)
{
	return Start + (End - Start) * Alpha;
}
