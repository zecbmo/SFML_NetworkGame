#pragma once
#include "PlayerCharacter.h"
#include <SFML\Network.hpp>
#include <list>



#define OUT //Tarcking out variables

/**
*	The Time Positon Struct keeps track of a positiona and When it Arrived
*/
struct TimePositionStruct
{
	sf::Vector2f Position;
	float ServerTimeStampInSeconds;
	
};

/** The Networked Character
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
class NetworkedCharacter : public PlayerCharacter
{
public:

	~NetworkedCharacter();

	/**
	*	Updates the Networked Character
	*	Prediciton happens here
	*
	*	@param Delta Time 
	*	@param the current game time
	*/
	void Update(float dt, float GameTime);

	/**
	*	Getter and Setters for the IP of the networked Character
	*/
	sf::IpAddress GetIP() { return m_OriginIP; };
	void SetIP(sf::IpAddress IP) { m_OriginIP = IP; };

	/**
	*	Getters and Setters for the Port of the networked Character
	*/
	unsigned short GetThePort() { return m_OriginPort; };
	void SetThePort(unsigned short Port) { m_OriginPort = Port; };

	/**
	*	Directional Setter
	*/
	inline void SetDir(PlayerDirection Dir) { m_Dir = Dir; };

	/**
	*	I dont use these functions....
	*/
	inline void SetServerLatency(float ServerLat) { m_ServerLatency = m_ServerLatency; };
	inline float GetServerLatency() { return m_ServerLatency;};

	/**
	*	When an update is recieved it is added to the list of positons - keeping track of the latest 3
	*
	*	@param the x positon of the character
	*	@param the y positon of the character
	*	@param the time the message was recieved
	*	@param the Clock - depreciated - really should update this instead of writing this comment
	*	@param Current X and Current Y - current positon of the character
	*/
	void AddToPredictionList(float x, float y, float TimeStamp, sf::Clock* Clock, float CurrentX, float CurrentY);
	

private:
	/**
	*	Stores the Origin IP of the Character
	*/
	sf::IpAddress m_OriginIP;
	/**
	*	Stores the Origin Port of the Character
	*/
	unsigned short m_OriginPort;
	/**
	*	Not used. I should delete this too! I might use it later. It also probably has sentimental value
	*/
	float m_ServerLatency;
	/**
	*	The list of previous positons stored by the character	
	*/
	TimePositionStruct m_PredictionList[3];

	
	//Deprecitated
	//void UpdateLerpValues(sf::Clock* Clock);
	//sf::Vector2f LerpFunction(sf::Vector2f Start, sf::Vector2f End, float Alpha);

	//sf::Vector2f m_LerpStartPos;
	//sf::Vector2f m_LerpEndPos;

	//float m_ExpectedTimeToReachEnd;
	
};

