#include "Application.h"
#include <iostream>
#include <stdint.h>

sf::Packet& operator <<(sf::Packet& packet, const PlayerUpdatePacket& m)
{
	return packet << m.XPos << m.YPos << m.Dir << m.ID;
}
sf::Packet& operator >>(sf::Packet& packet, PlayerUpdatePacket& m)
{
	return packet >> m.XPos >> m.YPos >> m.Dir >> m.ID;
}


Application::Application()
{
}


Application::~Application()
{
}

void Application::Init(sf::RenderWindow * Window, int ScreenWidth, int ScreenHeight)
{

	//Store window and pointers

	m_Window = Window;
	m_ScreenWidth = ScreenWidth;
	m_ScreenHeight = ScreenHeight;

	//Set Up the debug screen
	DebugScreen.Init("Assets/Font/font.ttf", SCREEN_MESSAGE_X_OFFSET, ScreenHeight-SCREEN_MESSAGE_Y_OFFSET );

	//Event Settings
	m_Window->setKeyRepeatEnabled(false);

	//Welcome Message
	m_WelcomeMessage = "Welcome To The BomberDude Game!";

	//Starting positions
	m_StartPosOne = sf::Vector2f(STARTING_OFFSET/2, STARTING_OFFSET);
	m_StartPosTwo = sf::Vector2f(ScreenWidth - STARTING_OFFSET, STARTING_OFFSET);
	m_StartPosThree = sf::Vector2f(STARTING_OFFSET/2, ScreenHeight -STARTING_OFFSET*2);
	m_StartPosFour = sf::Vector2f(ScreenWidth - STARTING_OFFSET, ScreenHeight - STARTING_OFFSET*2);


	//Server and Client
	std::cout << "Please Select (H)ost or (C)lient..." << std::endl;
	char HostOrClient;
	std::cin >> HostOrClient;

	HostOrClient = toupper(HostOrClient);
	if (HostOrClient == 'H')
	{
		m_HasAuthority = true;
	}
	else if(HostOrClient == 'C')
	{
		m_HasAuthority = false;

		std::cout << "Please Select Port Number..." << std::endl;
		unsigned short int Port;
		std::cin >> Port;

		m_ClientPortNumber = Port;
	}
	else
	{
		error::ErrorMessage("Invalid Choice when selecting Client/Host.");
	}

	
	m_ServerPortNumber = SERVER_PORT_NUMBER;
	
	//Set the initial player count (ie just the host)
	m_PlayerIDTracker = 1;

	if (m_HasAuthority) //SERVER
	{
		ServerSetUp();
		DebugScreen.AddToDebugScreen("Server", 0,0);
	}
	else //CLIENT
	{
		ClientSetUp();
		DebugScreen.AddToDebugScreen("Client", 0, 0);

	}
	
	m_UDPSocket.setBlocking(false);
}

void Application::Update(float dt)
{
	if (m_HasAuthority) //SERVER
	{
		ServerUpdate();
	}
	else //CLIENT
	{
		ClientUpdate();
	}

	//Update Game
	PlayerCharacter.Update(dt);
	for (auto iter : m_NetworkCharacterList)
	{
		iter->Update(dt);
	}

	DebugScreen.Update();
}

void Application::Render()
{
	//Render everything here
	PlayerCharacter.Render(m_Window);
	RenderNetworkedCharacters();
	DebugScreen.Render(m_Window);
}

void Application::ServerSetUp()
{
	//Set up UDP Socket
	m_UDPSocket.bind(m_ServerPortNumber);

	//Set up listener for TCP Connetions
	if (m_Listener.listen(m_ServerPortNumber) != sf::Socket::Done)
	{
		error::ErrorMessage("Host unable to bind the listener to port");
	}
	//Add listener to the selector
	m_Selector.add(m_Listener);
	m_Selector.add(m_UDPSocket);

	

	//Test Character Setup
	PlayerCharacter.Init(m_PlayerIDTracker, kRed, m_StartPosOne, "Assets/Textures/Chars.png", DebugScreen);
	m_PlayerIDTracker++;
}

void Application::ClientSetUp()
{
	//try to connect with TCP to the Server

	sf::Socket::Status status = m_ClientSideTCPSocket.connect(SERVER_IP, m_ServerPortNumber);
	if (status != sf::Socket::Done)
	{
		error::ErrorMessage("Client Unable to Connect to TCP Server");
	}
	else
	{
		//Show The Welcome Message
		DebugScreen.AddMessage("Connected To Server", kYellowColour);

		sf::Uint32 RecievedID;
		std::string RecievedMessage;
		sf::Packet packet;
		m_ClientSideTCPSocket.receive(packet);

		if (!(packet >> RecievedMessage >> RecievedID))
		{
			error::ErrorMessage("No Welcome Message Recieved!");
		}
		int ID = RecievedID;
		char WelcomeMessage[256];
		if (std::strcmp(RecievedMessage.c_str(), "To Many Players on Server! Please Try Later") == 0 )
		{
			error::ErrorMessage("Too Many Players on Server");
		}
		else
		{
			sprintf_s(WelcomeMessage, "%s You are Player %d", RecievedMessage.c_str(), RecievedID);
			DebugScreen.AddMessage(WelcomeMessage, kYellowColour);			
		}
		//Create Character from ID

		sf::Vector2f pos;

		switch (ID)
		{
		case 2: pos = m_StartPosTwo;
			break;
		case 3: 
			pos = m_StartPosThree;
			break;
		case 4:
			pos = m_StartPosFour;
			break;
		default:
			
			break;
		}
		PlayerCharacter.Init(ID, kGreen, pos, "Assets/Textures/Chars.png", DebugScreen);

		//Tell the host what colour we have picked
		sf::Uint32 col = PlayerCharacter.GetColour();

		sf::Packet ColourPacket;
		unsigned short UDPPort = m_ClientPortNumber;
		ColourPacket << col << UDPPort;
		m_ClientSideTCPSocket.send(ColourPacket);
		
		//Populate with other Characters

	}

	m_UDPSocket.bind(m_ClientPortNumber);
	
	m_Selector.add(m_UDPSocket);

}

void Application::ServerUpdate()
{
	//Recieving Data
	//make Selector wait for data on any connection
	sf::Time WaitTime = sf::microseconds(1); //we dont need to block.. just keep checking
	if (m_Selector.wait(WaitTime))
	{
		//Check Listener for new connection
		if (m_Selector.isReady(m_Listener))
		{
			//Create a new connection to add to the list
			sf::TcpSocket* NewClient = new sf::TcpSocket;
			//if the connections is successful
			if (m_Listener.accept(*NewClient) == sf::Socket::Done)
			{
				//Check if max players has been met
				if (m_PlayerIDTracker > 4)
				{
					//Send Failure message
					std::string ToMany = "To Many Players on Server! Please Try Later";
					sf::Packet Packet;
					Packet << ToMany << m_PlayerIDTracker;
					NewClient->send(Packet);	
					NewClient->disconnect();
				}
				else
				{
					//Add new connection to TCP list
					m_ClientTCPSockets.push_back(NewClient);
					m_Selector.add(*NewClient);
					//DebugScreen.AddToDebugScreen("New Client Connected", 0, 400);
					DebugScreen.AddMessage("New Client Connected", kYellowColour);

					//Send Welcome Message and player ID
					sf::Packet Packet;
					Packet << m_WelcomeMessage << m_PlayerIDTracker;
					NewClient->send(Packet);
				
					
					//Create a new networked Character
					NetworkedCharacter* NewCharacter = new NetworkedCharacter;
					sf::Vector2f pos;
					switch (m_PlayerIDTracker)
					{
					case 2: pos = m_StartPosTwo;
						break;
					case 3:
						pos = m_StartPosThree;
						break;
					case 4:
						pos = m_StartPosFour;
						break;
					default:

						break;
					}

					//Add the origin IP and port to the newtorked Char
					

					//Create UDP Socket Based on this Address

					sf::Uint32 col;
					sf::Packet RecieveColour;
					unsigned short UDPPort;
					NewClient->receive(RecieveColour);

					
					if(RecieveColour >> col >> UDPPort)
					{

						NewCharacter->Init(m_PlayerIDTracker, (PlayerColour)col, pos, "Assets/Textures/Chars.png", DebugScreen);

						NewCharacter->SetIP(NewClient->getRemoteAddress());
						NewCharacter->SetThePort(UDPPort);

						m_NetworkCharacterList.push_back(NewCharacter);
					}

					//Tell Clients to Add The Hoset networked character
				

					//update tracker number
					m_PlayerIDTracker++;
				}
			}
			else
			{
				error::ErrorMessage("New Connection Failed on Host. Unable to add to TCP Client list!");
			}
		}
		if (m_Selector.isReady(m_UDPSocket))
		{
			//Recieve packet from client

			

			while (true)
			{
				PlayerUpdatePacket PlayerPacket;

				sf::Packet Packet;
				//Packet << PlayerPacket;

				sf::IpAddress Sender;
				unsigned short Port;

				if (m_UDPSocket.receive(Packet, Sender, Port) != sf::Socket::Done)
				{
					break;
				}

				if (Packet >> PlayerPacket)
				{
					//Check the id against the client and update or send the new position to the clients
					for (auto iter : m_NetworkCharacterList)
					{
						if (iter->GetID() == PlayerPacket.ID)
						{
							iter->UpdatePosition(PlayerPacket.XPos, PlayerPacket.YPos);
							iter->SetDir((PlayerDirection)PlayerPacket.Dir);
						}
						else
						{
							m_UDPSocket.send(Packet, iter->GetIP(), iter->GetThePort());
						}

						PlayerUpdatePacket PlayerPacket;

						PlayerPacket.XPos = PlayerCharacter.GetXPos();
						PlayerPacket.YPos = PlayerCharacter.GetYPos();
						PlayerPacket.ID = PlayerCharacter.GetID();
						PlayerPacket.Dir = PlayerCharacter.GetDir();


						sf::Packet InitPacket;
						InitPacket << PlayerPacket;


						m_UDPSocket.send(InitPacket, iter->GetIP(), iter->GetThePort());
					}
				}
			}
		}
	}
}

void Application::ClientUpdate()
{
	PlayerUpdatePacket PlayerPacket;

	PlayerPacket.XPos = PlayerCharacter.GetXPos();
	PlayerPacket.YPos = PlayerCharacter.GetYPos();
	PlayerPacket.ID = PlayerCharacter.GetID();
	PlayerPacket.Dir = PlayerCharacter.GetDir();


	sf::Packet Packet;
	Packet << PlayerPacket;

	sf::IpAddress ip(SERVER_IP);
	m_UDPSocket.send(Packet, ip, m_ServerPortNumber);

	//Listen to the UDP Socket

	sf::Time WaitTime = sf::microseconds(1); //we dont need to block.. just keep checking
	if (m_Selector.wait(WaitTime))
	{
		if (m_Selector.isReady(m_UDPSocket))
		{
			while (true)
			{
				PlayerUpdatePacket PlayerPacket;

				sf::Packet Packet;
				//Packet << PlayerPacket;

				sf::IpAddress Sender;
				unsigned short Port;
				if (m_UDPSocket.receive(Packet, Sender, Port) != sf::Socket::Done)
				{
					break;
				}

				if (Packet >> PlayerPacket)
				{
					//A bool to check against the ID if the player already exists
					bool IsNewPlayer = true;

					//Check the id against the client and update or send the new position to the clients
					for (auto iter : m_NetworkCharacterList)
					{
						if (iter->GetID() == PlayerPacket.ID)
						{
							iter->UpdatePosition(PlayerPacket.XPos, PlayerPacket.YPos);
							iter->SetDir((PlayerDirection)PlayerPacket.Dir);
							//This player exists... we dont need to add them
							IsNewPlayer = false;
						}

					}

					if (IsNewPlayer)
					{
						//Create a new networked Character
						NetworkedCharacter* NewCharacter = new NetworkedCharacter;
						sf::Vector2f pos = sf::Vector2f(PlayerPacket.XPos, PlayerPacket.YPos);


						NewCharacter->Init(PlayerPacket.ID, kRed, pos, "Assets/Textures/Chars.png", DebugScreen);
						m_NetworkCharacterList.push_back(NewCharacter);

					}
				}

			}
		}
	}

}

void Application::RenderNetworkedCharacters()
{
	for (auto iter : m_NetworkCharacterList)
	{
		iter->Render(m_Window);
	}
}
