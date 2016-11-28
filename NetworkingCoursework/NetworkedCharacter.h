#pragma once
#include "PlayerCharacter.h"

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

class NetworkedCharacter : public PlayerCharacter
{
public:
	NetworkedCharacter();
	~NetworkedCharacter();

	void UpdatePosition(float x, float y);
	void Update(float dt) {};
};

