#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "PlayerCharacter.h"
#include "NetworkedCharacter.h"
#include "DebugUI.h"
#include <list>
#include <time.h>  
#include <chrono>
#include "Bomb.h"

#define SCREEN_MESSAGE_Y_OFFSET 40
#define SCREEN_MESSAGE_X_OFFSET 10
#define STARTING_OFFSET 100

#define SERVER_PORT_NUMBER 7777
#define CLIENT_PORT_NUMBER 7778

//Used for testing
#define SERVER_IP "127.0.0.1"
#define CLIENT_IP "127.0.0.1"



/**
*	Player Update Packet
*
*	This is the update packet that will be sent x times/second
*	it will be used to update positions for the players
*	It also contains information regarding player direction for animation
*	It has a timestamp for ordering positions in an array for prediction
*
*/
struct PlayerUpdatePacket
{
	float XPos;
	float YPos;
	sf::Uint8 Dir;
	sf::Uint8 ID;
	sf::Uint32 PlayersColour;
	float ServerTimeStamp;
};

/**
*	Welcome Packet
*
*	Used to send a welcome message to the new player
*	Has the gametime stamp with the player ID 
*	to let the player know which ID will be theirs
*/
struct WelcomePacket
{
	std::string WelcomeMessage;
	sf::Uint32 IDTracker;
	float ServerGameTime;
};

/**
*	Welcome Packet Reply
*
*	Lets the sever know which colour the player 
*	has chosen and the UDP port of the client
*/
struct WelcomePacketReply
{
	sf::Uint32 PlayersChosenColour;
	unsigned short UDPPort;
};

/**
*	Bomb Packet
*
*	Has the position of the bomb and the time it will explode
*
*/
struct BombPacket
{
	float Xpos;
	float Ypos;
	float TimeToExplode;
};

/**
*	Packet Overloads 
*
*	Getting our structs to work with SFML data types
*/
sf::Packet& operator <<(sf::Packet& packet, const PlayerUpdatePacket& m);
sf::Packet& operator >>(sf::Packet& packet, PlayerUpdatePacket& m);

sf::Packet& operator <<(sf::Packet& packet, const WelcomePacket& m);
sf::Packet& operator >>(sf::Packet& packet, WelcomePacket& m);

sf::Packet& operator <<(sf::Packet& packet, const WelcomePacketReply& m);
sf::Packet& operator >>(sf::Packet& packet, WelcomePacketReply& m);

sf::Packet& operator <<(sf::Packet& packet, const BombPacket& m);
sf::Packet& operator >>(sf::Packet& packet, BombPacket& m);

/**	
*	The Application Class
*
*	This is the main Game Loop controller 
*	Thats and an initialisation an Update and a Render Function which will be run 
*	from the main.ccp
*
*/


class Application
{
public:
	Application();
	~Application();

	/**
	*	The Intialiser function
	*	Will initialse all variables and set up the player/server/client
	*
	*	@param RenderWindow pointer to the created SFML window
	*	@param ScreenWidth the set screen width
	*	@param ScreenHeight the set screen height
	*
	*/
	void Init(sf::RenderWindow* Window, int ScreenWidth, int ScreenHeight);

	/**
	*	Update the main game loop
	*	
	*	@param delta time 
	*/
	void Update(float dt);

	/**
	*	Render all objects to the SFML window
	*
	*/
	void Render();
	

private:
	sf::RenderWindow* m_Window;
	int m_ScreenWidth;
	int m_ScreenHeight;

	/**
	*	All UI text controlled in the Debug screen
	*/
	DebugUI DebugScreen;

	/**
	*	Will represent the local player character
	*/
	PlayerCharacter PlayerCharacter;

	//Networking
	//Server and Client elements
	/**
	*	Ports for initialising the Server/Client
	*/
	unsigned short int m_ServerPortNumber;
	unsigned short int m_ClientPortNumber;

	/**
	*	If has Authority then it is there server - only one connected machine can have Authority
	*/
	bool m_HasAuthority;
	/**
	*	One UDP socket that will allow messages from multiple addresses/ports
	*/
	sf::UdpSocket m_UDPSocket;	

	/**
	*	Tracks which player you are. The host will always been No.1. When players connect the server sends them this number
	*/
	sf::Uint32 m_PlayerIDTracker; 
	/**
	*	Network Characters are the Visual Representation of other players. Stores them in a list when a client joins
	*/
	std::list<NetworkedCharacter*> m_NetworkCharacterList;
	/**
	*	The time out controlls how long to wait befor a TCP send/recieve will fail
	*/
	sf::Time m_TimeOut;
	/**
	*	how many seconds must pass before the client sends the next update - Tested with 5/10/20 updates per second
	*/
	float m_NetworkedUpdatesSpeed; 

	//Sever Spefific
	/**
	*	The listner that will monitior for new TCP connections
	*/
	sf::TcpListener m_Listener;
	/**
	*	The Selector that will let us know if a connection has a message ready then we can act on it
	*/
	sf::SocketSelector m_Selector;	
	/**
	*	All clients that are added will be stored in a TCP list to monitor their connection
	*/
	std::list<sf::TcpSocket*> m_ClientTCPSockets;
	/**
	*	The Welcome String that is sent to all clients
	*/
	std::string m_WelcomeMessage;

	//Clock
	/**
	*	The Local Game Clock will be the game time but synced to the Server
	*/
	sf::Clock LocalGameClock;
	/**
	*	The newtork update clock is used to keep track of when to send position update packets
	*/
	sf::Clock m_NetworkUpdateClock;
	/**
	*	The Difference in Time Between the server/Client in Seconds
	*/
	float TimeDifferenceSync; 
	/**
	*	Text that is controlled within the DebugUI - Updated to display current time on the screen
	*/
	sf::Text* ServerTimeDisplay;
	/**
	*	Latency TCP Requests
	*
	*	Used in conjunction with the Recieve lateny functon
	*	will Return a time taken in Seconds to send a message to another machine
	*
	*	@param The Socket to send the request to
	*
	*/
	float LatencyTCPRequest(sf::TcpSocket& Socket); //Used to sync clocks
	/**
	*	Recieve Latency
	*	
	*	@param The Socket in which the latecny request comes from
	*/
	void RecieveLatenyRequest(sf::TcpSocket& Socket);
	
	//Client Specific
	/**
	*	The TCP connection to the sever stored on the client
	*/
	sf::TcpSocket m_ClientSideTCPSocket;

	//NetworkFuntions
	/**
	*	Server Setup and Intialisation
	*/
	void ServerSetUp();
	/**
	*	Client Setup and Initialisation - connects to Server here
	*/
	void ClientSetUp();

	/**
	*	Server update - Listens to incomming connections and deals with messages.
	*	Own player character updates happen here
	*/
	void ServerUpdate();
	/**
	*	Client Update - Listens to incoming connecitons and deals with messages.
	*	Own player Character updates happen here
	*	Sending Updates to server also
	*/
	void ClientUpdate();

	//Player Start Position
	/**
	*	The Predifined Start positions of the players for when the join the game
	*/
	sf::Vector2f m_StartPosOne;
	sf::Vector2f m_StartPosTwo;
	sf::Vector2f m_StartPosThree;
	sf::Vector2f m_StartPosFour;

	//RenderHelpers
	/**
	*	A helper function that loops the list of network characters and renders them
	*/
	void RenderNetworkedCharacters();

	//ServerFunctions
	/**
	*	Update functions for when a packet is recieved from the listener
	*/
	void ServerManagePacketsFromListener();
	/**
	*	Update functions for when a packet is recieved from the UDP Socket
	*/
	void ServerManagePacketsfromUDPSocket();
	/**
	*	Update functions for when a packet is recieved from the list of TCP sockets
	*/
	void ServerManagePacketsFromTCPSockets();

	//ClientFuctions
	/**
	*	Update functions for when a packet is recieved from the Udp Socket
	*/
	void ClientManagePacketsfromUDPSocket();
	/**
	*	Update functions for when a packet is recieved from the TCP Socket
	*/
	void ClientManagePacketsfromTCPSocket();

	//bombs
	/**
	*	A list of bombs - when a player sends a bomb message they will be added to the bomb list
	*/
	std::list<Bomb*> m_Bombs;
	/**
	*	Check and Create bombs checks if a player character is requesting to make a bomb and then sends this to the server
	*
	*	@param delta time
	*/
	void CheckForAndCreateBombs(float dt);
	/**
	*	Send Bomb Packet will send the positon of the newly created bomb and when it is expected to go off
	*	
	*	@param Bomb Creation Packet - contains struct with positon and time
	*	@param Socket to send the bomb to - Client will just send this to the server - sevrer will send to all clients
	*/
	void SendBombPacket(BombPacket BombCreationPacket, sf::TcpSocket* Socket);
	/**
	*	Updates the Bombs and explodes them if past the time
	*	Manages deletion from the bomb list
	*
	*	@param delta time
	*/
	void UpdateBombList(float dt);
	/**
	*	Renders the bombs in the list
	*/
	void RenderBombs();
	
};

