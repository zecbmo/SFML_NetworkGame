#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "PlayerCharacter.h"
#include "NetworkedCharacter.h"
#include "DebugUI.h"
#include <list>

#define SCREEN_MESSAGE_Y_OFFSET 40
#define SCREEN_MESSAGE_X_OFFSET 10
#define STARTING_OFFSET 100

#define SERVER_PORT_NUMBER 7777
#define CLIENT_PORT_NUMBER 7778

//Used for testing
#define SERVER_IP "127.0.0.1"
#define CLIENT_IP "127.0.0.1"

//Update Packet and overloads
struct PlayerUpdatePacket
{
	float XPos;
	float YPos;
	sf::Uint8 Dir;
	sf::Uint8 ID;

};

sf::Packet& operator <<(sf::Packet& packet, const PlayerUpdatePacket& m);
sf::Packet& operator >>(sf::Packet& packet, PlayerUpdatePacket& m);


class Application
{
public:
	Application();
	~Application();

	void Init(sf::RenderWindow* Window, int ScreenWidth, int ScreenHeight);

	void Update(float dt);
	void Render();
	

private:
	sf::RenderWindow* m_Window;
	int m_ScreenWidth;
	int m_ScreenHeight;

	DebugUI DebugScreen;
	PlayerCharacter PlayerCharacter;

	//Networking
	//Server and Client elements
	unsigned short int m_ServerPortNumber;
	unsigned short int m_ClientPortNumber;
	bool m_HasAuthority;
	sf::UdpSocket m_UDPSocket;	
	sf::Uint32 m_PlayerIDTracker; //Host will always be player 1 //standard sized ints used when being sent over network packets
	std::list<NetworkedCharacter*> m_NetworkCharacterList;

	//Sever Spefific
	sf::TcpListener m_Listener;
	sf::SocketSelector m_Selector;	
	std::list<sf::TcpSocket*> m_ClientTCPSockets;
	std::string m_WelcomeMessage;

	//Client Specific
	sf::TcpSocket m_ClientSideTCPSocket;

	//NetworkFuntions
	void ServerSetUp();
	void ClientSetUp();

	void ServerUpdate();
	void ClientUpdate();

	//Player Start Position
	sf::Vector2f m_StartPosOne;
	sf::Vector2f m_StartPosTwo;
	sf::Vector2f m_StartPosThree;
	sf::Vector2f m_StartPosFour;

	//RenderHelpers
	void RenderNetworkedCharacters();

};

