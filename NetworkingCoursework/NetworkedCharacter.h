#pragma once
#include "PlayerCharacter.h"
#include <SFML\Network.hpp>
#include <list>

/* The Networked Character
*
*	The NetWorked Character will represent other players on the network playing the game
*	For each client (Host included) the PLAYER chacter will represent the locally controlled character
*	The clients own player character will control movements and updates that are then sent to the host
*	The host will transmite all player changes to the clients
*	When this packet is recieved it will update the NETWORK characters on the clients
*
*	Essentially the Network Characters on clients will only Render other players and predict movement 
*	The Host will use these characters to control the game (whether a player is hit etc) 
*	And transmit this info to the clients
*/

#define OUT //Tarcking out variables

struct TimePositionStruct
{
	sf::Vector2f Position;
	float ServerTimeStampInSeconds;
	
};


class NetworkedCharacter : public PlayerCharacter
{
public:
	NetworkedCharacter();
	~NetworkedCharacter();

	
	void Update(float dt, float GameTime);

	sf::IpAddress GetIP() { return m_OriginIP; };
	void SetIP(sf::IpAddress IP) { m_OriginIP = IP; };
	unsigned short GetThePort() { return m_OriginPort; };
	void SetThePort(unsigned short Port) { m_OriginPort = Port; };

	inline void SetDir(PlayerDirection Dir) { m_Dir = Dir; };

	inline void SetServerLatency(float ServerLat) { m_ServerLatency = m_ServerLatency; };
	inline float GetServerLatency() { return m_ServerLatency;};

	void AddToPredictionList(float x, float y, float TimeStamp, sf::Clock* Clock);
	

private:
	sf::IpAddress m_OriginIP;
	unsigned short m_OriginPort;
	float m_ServerLatency;

	TimePositionStruct m_PredictionList[2];

	void UpdateLerpValues(sf::Clock* Clock);
	sf::Vector2f LerpFunction(sf::Vector2f Start, sf::Vector2f End, float Alpha);

	sf::Vector2f m_LerpStartPos;
	sf::Vector2f m_LerpEndPos;
	float m_ExpectedTimeToReachEnd;


};

