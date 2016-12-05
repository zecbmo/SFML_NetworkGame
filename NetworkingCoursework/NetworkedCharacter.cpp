#include "NetworkedCharacter.h"



NetworkedCharacter::~NetworkedCharacter()
{
}

void NetworkedCharacter::Update(float dt, float GameTime)
{

	UpdateSpriteState(dt);


	//Psuedo code from http://www.it.uu.se/edu/course/homepage/igs/ht06/labs/bigs_deadreckoning.pdf



	sf::Vector2f Pos1 = m_PredictionList[0].Position;
	sf::Vector2f Pos2 = m_PredictionList[1].Position;
	sf::Vector2f Pos3 = m_PredictionList[2].Position;
	float t1 = m_PredictionList[0].ServerTimeStampInSeconds;
	float t2 = m_PredictionList[1].ServerTimeStampInSeconds;
	float t3 = m_PredictionList[2].ServerTimeStampInSeconds;

	/********************************  Convergence  ********************************/

	////PrevPredictedPathVector = Last_KnownPos[1] – Last_KnownPos[2]
	//sf::Vector2f PrevPredictedPathVec = Pos2 - Pos3;

	////PrevDeltaTime = Last_KnownTime[1] – Last_KnownTime[2]
	//float  PrevDeltaTime = t2 - t3;

	////StartDeltaTime = Last_KnownTime[0] – Last_KnownTime[2]
	//float StartDeltaTime  = t1 - t3;

	////PrevVelocity = PrevPredictedPathVector / PrevDeltaTime
	//sf::Vector2f PrevVelocity = PrevPredictedPathVec / PrevDeltaTime;

	////StartPos = Last_KnownPos[2] + StartDeltaTime * 	PrevVelocity
	//sf::Vector2f StartPos = Pos3 + StartDeltaTime * PrevVelocity;

	////CurrentDeltaTime = Now - Last_KnownTime[0]
	//float CurrentDeltaTime = GameTime - t1;

	////PredictedPathVector = Last_KnownPos[0] – 	Last_KnownPos[1]
	//sf::Vector2f PredictedPathVec = Pos1 - Pos2;

	////DeltaTime = Last_KnownTime[0] – Last_KnownTime[1]
	//float DeltaTime = t1 - t2;

	////EndPos = Last_KnownPos[0] + CurrentDeltaTime * 	PredictedPathVector / DeltaTime
	//sf::Vector2f EndPos = Pos1 + CurrentDeltaTime * (PredictedPathVec / DeltaTime);

	////LinearConvergenceVector = EndPos - StartPos
	//sf::Vector2f LinearConvergence = EndPos - StartPos;

	////Velocity = LinearConvergenceVector / DeltaTime
	//sf::Vector2f Velocity = LinearConvergence / DeltaTime;

	////NewPos = StartPos + CurrentDeltaTime * Velocity
	//sf::Vector2f NewPos = StartPos + CurrentDeltaTime * Velocity;


	/******************************** Dead reckoning ************************/
	sf::Vector2f ppv = Pos1 - Pos2;
	float dt2 = t1 - t2;
	sf::Vector2f dv = ppv / dt2;
	float ndt = GameTime - t2;
	sf::Vector2f NewPos = Pos1 + ndt * dv;
	/*************************************************************************/

	m_Sprite.setPosition(NewPos);
}

void NetworkedCharacter::AddToPredictionList(float x, float y, float TimeStamp, sf::Clock* Clock, float CurrentX, float CurrentY)
{
	m_Position = sf::Vector2f(CurrentX, CurrentY);
	//Create new Timstruct
	TimePositionStruct NewValue;
	NewValue.Position = sf::Vector2f(x, y);
	NewValue.ServerTimeStampInSeconds = TimeStamp;

	//Find where it should go in the list
	for (int i = 0; i < 3; i++)
	{
		float TimeCheck = m_PredictionList[i].ServerTimeStampInSeconds;

		if (TimeStamp > TimeCheck)
		{
			if (i == 0)
			{
				m_PredictionList[2] = m_PredictionList[1];
				m_PredictionList[1] = m_PredictionList[0];
				m_PredictionList[0] = NewValue;
				break;
			}
			else if(i == 1)
			{
				m_PredictionList[2] = m_PredictionList[1];
				m_PredictionList[1] = NewValue;
				break;
			}
			else
			{
				m_PredictionList[2] = NewValue;
			}
		}

	
	}

	//UpdateLerpValues(Clock);
}

void NetworkedCharacter::UpdateLerpValues(sf::Clock* Clock)
{
	//The Current position will be the Starting point of the Lerp
	//if (m_DoOnce)
	//{
	//	m_LerpStartPos = sf::Vector2f(0, 0);
	//	m_DoOnce = false;
	//}
	//else
	//{
	//	m_LerpStartPos = m_Sprite.getPosition();
	//}
	////Get the difference betweeb the two positions

	//
	//m_LerpEndPos = Pos1 + (Pos1 - Pos2) * (ct - st);

	////Get the time difference between the messages
	//float TimeDifferenceBetweenMessages = m_PredictionList[0].ServerTimeStampInSeconds - m_PredictionList[1].ServerTimeStampInSeconds;

	//m_ExpectedTimeToReachEnd = Clock->getElapsedTime().asSeconds() + TimeDifferenceBetweenMessages;

}
//void NetworkedCharacter::UpdateLerpValues(sf::Clock* Clock)
//{
//	//The Current position will be the Starting point of the Lerp
//	if (m_DoOnce)
//	{
//		m_LerpStartPos = sf::Vector2f(0, 0);
//		m_DoOnce = false;
//	}
//	else
//	{
//		m_LerpStartPos = m_Sprite.getPosition();
//	}
//	//Get the difference betweeb the two positions
//	sf::Vector2f Pos1 = m_PredictionList[0].Position;
//	sf::Vector2f Pos2 = m_PredictionList[1].Position;
//	float t1 = m_PredictionList[0].ServerTimeStampInSeconds;
//	float t2 = m_PredictionList[1].ServerTimeStampInSeconds;
//
//
//	sf::Vector2f Offset = Pos1 - Pos2;
//	float TimeToMoveOffset = t1 - t2;
//	float TimeDifFromlastPacket = Clock->getElapsedTime().asSeconds() - t1;
//	
//
//	m_Sp
//	//Client is at pos1 at t1
//	//Client is at pos2 at t2
//	//the time now is t3
//	//from this we need to get where client will be at t4
//	m_LerpEndPos
//	
//
//	m_ExpectedTimeToReachEnd = Clock->getElapsedTime().asSeconds() + TimeDifFromlastPacket;
//
//}
sf::Vector2f NetworkedCharacter::LerpFunction(sf::Vector2f Start, sf::Vector2f End, float Alpha)
{
	//v0 + t*(v1-v0)
	//return Start + Alpha*(End - Start);
	//(1 - t)*v0 + t*v1;
	return (1-Alpha)*Start + Alpha*End;
}
