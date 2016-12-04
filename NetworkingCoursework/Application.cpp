#include "Application.h"
#include <iostream>
#include <stdint.h>

//player packet overloads
sf::Packet& operator <<(sf::Packet& packet, const PlayerUpdatePacket& m)
{
	return packet << m.XPos << m.YPos << m.Dir << m.ID << m.PlayersColour << m.ServerTimeStamp;
}
sf::Packet& operator >>(sf::Packet& packet, PlayerUpdatePacket& m)
{
	return packet >> m.XPos >> m.YPos >> m.Dir >> m.ID >> m.PlayersColour >> m.ServerTimeStamp;
}
//Welcome Message Overloads
sf::Packet& operator <<(sf::Packet& packet, const WelcomePacket& m)
{
	return packet << m.WelcomeMessage << m.IDTracker << m.ServerGameTime;
}
sf::Packet& operator >>(sf::Packet& packet, WelcomePacket& m)
{
	return packet >> m.WelcomeMessage >> m.IDTracker >> m.ServerGameTime;
}
//Welcome message reply overloads
sf::Packet& operator <<(sf::Packet& packet, const WelcomePacketReply& m)
{
	return packet << m.PlayersChosenColour  << m.UDPPort;
}
sf::Packet& operator >>(sf::Packet& packet, WelcomePacketReply& m)
{
	return packet >> m.PlayersChosenColour >> m.UDPPort;
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


	//Ask user wheter they will act as host/client and set up
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
		ServerTimeDisplay = DebugScreen.AddToDebugScreen("Server Time:", ScreenWidth - ScreenWidth/2, 0);

	}
	else //CLIENT
	{
		ClientSetUp();
		DebugScreen.AddToDebugScreen("Client", 0, 0);
		ServerTimeDisplay = DebugScreen.AddToDebugScreen("Server Time:", ScreenWidth - ScreenWidth/2, 0);
	}

	m_TimeOut = sf::seconds(1.0);
	m_UDPSocket.setBlocking(false);
	m_NetworkedUpdatesSpeed = 0.2f;
}

void Application::Update(float dt)
{
	//Update local variables
	if (m_HasAuthority) //SERVER
	{
		ServerUpdate(); 
		char temp[256];
		sf::Time  CurrentTime = LocalGameClock.getElapsedTime();
		sprintf_s(temp, "ServerTime : %f", CurrentTime.asSeconds());
		ServerTimeDisplay->setString(temp);
		
	}
	else //CLIENT
	{
		ClientUpdate();
		char temp[256];
		sf::Time  CurrentTime = LocalGameClock.getElapsedTime();
		float SyncedTime = CurrentTime.asSeconds() + TimeDifferenceSync;
		sprintf_s(temp, "ServerTime : %f", SyncedTime);// +TimeDifferenceSync);
		ServerTimeDisplay->setString(temp);
	}

	//Update Game with local variables
	PlayerCharacter.Update(dt);
	for (auto iter : m_NetworkCharacterList)
	{
		iter->Update(dt, LocalGameClock.getElapsedTime().asSeconds());
	}

	DebugScreen.Update();
}

void Application::Render()
{
	//Render everything here
	PlayerCharacter.Render(m_Window);
	RenderNetworkedCharacters();

	//last to be on top 
	DebugScreen.Render(m_Window);
}

float Application::LatencyTCPRequest(sf::TcpSocket& Socket)
{
	float Latency;
	//Send the current Game Time to server
	sf::Packet LatencyPacket;
	LatencyPacket << LocalGameClock.getElapsedTime().asSeconds();
	Socket.send(LatencyPacket);
	//recieve message back from server with the same time stamp oriigionally sent
	Socket.receive(LatencyPacket);
	//Compare difference and divide by 2
	if (LatencyPacket >> Latency)
	{
		Latency = (LocalGameClock.getElapsedTime().asSeconds() - Latency) /2;
	}
	else
	{
		Latency = -1;
	}
	return Latency;
}

void Application::RecieveLatenyRequest(sf::TcpSocket& Socket)
{
	//Recieve a timestamp and send it back to sender
	sf::Packet LatencyPacket;
	Socket.receive(LatencyPacket);
	float Latency;
	if (LatencyPacket >> Latency)
	{
		Socket.send(LatencyPacket);
	}
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

	//Add listener and UDP socket to the selector
	m_Selector.add(m_Listener);
	m_Selector.add(m_UDPSocket);

	
	//Controlling Character Setup
	PlayerCharacter.Init(m_PlayerIDTracker, kRed, m_StartPosOne, "Assets/Textures/Chars.png", DebugScreen);
	m_PlayerIDTracker++;
}

void Application::ClientSetUp()
{
	//Select colour
	std::cout << "Please Select Colour. (R)ed, (G)reen, (B)lack or (P)urple, " << std::endl;
	char Colour;
	std::cin >> Colour;
	Colour = toupper(Colour);
	PlayerColour ChoosenColour;


	switch (Colour)
	{
	case 'R': ChoosenColour = kRed;
		break;
	case 'G': ChoosenColour = kGreen;
		break;
	case 'B': ChoosenColour = kBlack;
		break;
	case 'P': ChoosenColour = kPurple;
		break;
	default:
		break;
	}
	//

	//try to connect with TCP to the Server
	sf::Socket::Status status = m_ClientSideTCPSocket.connect(SERVER_IP, m_ServerPortNumber, m_TimeOut);
	if (status != sf::Socket::Done)
	{
		error::ErrorMessage("Client Unable to Connect to TCP Server");
	}
	else
	{
		//Show The Welcome Message
		DebugScreen.AddMessage("Connected To Server", kYellowColour);

		WelcomePacket RecievedWelcomePacket;
		
		sf::Packet Packet;
		m_ClientSideTCPSocket.receive(Packet);

		//First message should be a welcome message to allow for setting up existing game variables
		if (!(Packet >> RecievedWelcomePacket))
		{
			error::ErrorMessage("No Welcome Message Recieved!");
		}
		float ServerGameTimeComparison = RecievedWelcomePacket.ServerGameTime - LocalGameClock.getElapsedTime().asSeconds();
		//Sync clocks
		float Latency = LatencyTCPRequest(m_ClientSideTCPSocket);

		TimeDifferenceSync = Latency + ServerGameTimeComparison;

		int ID = RecievedWelcomePacket.IDTracker;
		char WelcomeMessage[256];

		//If there are too Many players dont allow a connection
		if (std::strcmp(RecievedWelcomePacket.WelcomeMessage.c_str(), "To Many Players on Server! Please Try Later") == 0 )
		{
			error::ErrorMessage("Too Many Players on Server");
		}
		else
		{
			sprintf_s(WelcomeMessage, "%s You are Player %d", RecievedWelcomePacket.WelcomeMessage.c_str(), ID);
			DebugScreen.AddMessage(WelcomeMessage, kYellowColour);			
		}

		//Create Character from ID and set starting position

		sf::Vector2f pos;

		switch (ID)
		{
		case 2: 
			pos = m_StartPosTwo;
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
		PlayerCharacter.Init(ID, ChoosenColour, pos, "Assets/Textures/Chars.png", DebugScreen);

		//Reply to Server that we have connected and created a character
		WelcomePacketReply ReplyPacketToSend;

		//Tell the host what colour we have picked and retirn time
		ReplyPacketToSend.PlayersChosenColour = PlayerCharacter.GetColour();		
		ReplyPacketToSend.UDPPort = m_ClientPortNumber;
		//ReplyPacketToSend.ServerGameTimeReturned = RecievedWelcomePacket.ServerGameTime;
		//Send the reply packet
		sf::Packet ReplyPacket;
		ReplyPacket << ReplyPacketToSend;
		m_ClientSideTCPSocket.send(ReplyPacket);
	}

	//Bind and add the socket too the listener
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
			ServerManagePacketsFromListener();
		}
		if (m_Selector.isReady(m_UDPSocket))
		{
			ServerManagePacketsfromUDPSocket();
		}
	}
}

void Application::ClientUpdate()
{
	//if the NetworkClock is greater that the Network update time then reset and send sevrer message
	if (m_NetworkUpdateClock.getElapsedTime().asSeconds() > m_NetworkedUpdatesSpeed)
	{
		PlayerUpdatePacket PlayerPacket;

		PlayerPacket.XPos = PlayerCharacter.GetXPos();
		PlayerPacket.YPos = PlayerCharacter.GetYPos();
		PlayerPacket.ID = PlayerCharacter.GetID();
		PlayerPacket.Dir = PlayerCharacter.GetDir();
		PlayerPacket.PlayersColour = PlayerCharacter.GetColour();
		PlayerPacket.ServerTimeStamp = LocalGameClock.getElapsedTime().asSeconds() + TimeDifferenceSync;

		sf::Packet Packet;
		Packet << PlayerPacket;

		sf::IpAddress ip(SERVER_IP);
		m_UDPSocket.send(Packet, ip, m_ServerPortNumber);

		m_NetworkUpdateClock.restart();
	}
	//Listen to the UDP Socket

	sf::Time WaitTime = sf::microseconds(1); //we dont need to block.. just keep checking
	if (m_Selector.wait(WaitTime))
	{
		if (m_Selector.isReady(m_UDPSocket))
		{
			ClientManagePacketsfromUDPSocket();
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

void Application::ServerManagePacketsFromListener()
{
	//Create a new connection to add to the list
	sf::TcpSocket* NewClient = new sf::TcpSocket;
	//if the connections is successful
	if (m_Listener.accept(*NewClient) == sf::Socket::Done)
	{
		WelcomePacket WelcomePacketToSend;

		//Check if max players has been met
		if (m_PlayerIDTracker > 4)
		{
			//Send Failure message
			WelcomePacketToSend.WelcomeMessage = "To Many Players on Server! Please Try Later";
			sf::Packet Packet;
			Packet << WelcomePacketToSend;
			NewClient->send(Packet);
			NewClient->disconnect();
		}
		else //add a new player
		{
			//Add new connection to TCP list
			m_ClientTCPSockets.push_back(NewClient);
			m_Selector.add(*NewClient);
			//DebugScreen.AddToDebugScreen("New Client Connected", 0, 400);
			DebugScreen.AddMessage("New Client Connected", kYellowColour);

			//Send Welcome Message and player ID
			WelcomePacketToSend.WelcomeMessage = m_WelcomeMessage;
			WelcomePacketToSend.IDTracker = m_PlayerIDTracker;
			sf::Time CurrrentTime = LocalGameClock.getElapsedTime();
			WelcomePacketToSend.ServerGameTime = CurrrentTime.asSeconds();
			

			/***********************************************************************************************************************************************************************/

			//Send Packet
			sf::Packet Packet;
			Packet << WelcomePacketToSend;
			NewClient->send(Packet);

			RecieveLatenyRequest(*NewClient);

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
			WelcomePacketReply RecievedWelcomeReply;


			sf::Packet RecievePacket;
			NewClient->receive(RecievePacket);


			if (RecievePacket >> RecievedWelcomeReply)
			{

				NewCharacter->Init(m_PlayerIDTracker, (PlayerColour)RecievedWelcomeReply.PlayersChosenColour, pos, "Assets/Textures/Chars.png", DebugScreen);

				NewCharacter->SetIP(NewClient->getRemoteAddress());
				NewCharacter->SetThePort(RecievedWelcomeReply.UDPPort);

				m_NetworkCharacterList.push_back(NewCharacter);


			}

			//update tracker number
			m_PlayerIDTracker++;
		}
	}
	else
	{
		error::ErrorMessage("New Connection Failed on Host. Unable to add to TCP Client list!");
	}
}

void Application::ServerManagePacketsfromUDPSocket()
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
					float x = iter->GetXPos();
					float y = iter->GetYPos();

					iter->AddToPredictionList(PlayerPacket.XPos, PlayerPacket.YPos, PlayerPacket.ServerTimeStamp, &LocalGameClock,x ,y);
					iter->SetDir((PlayerDirection)PlayerPacket.Dir);
				}
				else
				{
					m_UDPSocket.send(Packet, iter->GetIP(), iter->GetThePort());
				}

				PlayerUpdatePacket PlayerPacketServer;

				PlayerPacketServer.XPos = PlayerCharacter.GetXPos();
				PlayerPacketServer.YPos = PlayerCharacter.GetYPos();
				PlayerPacketServer.ID = PlayerCharacter.GetID();
				PlayerPacketServer.Dir = PlayerCharacter.GetDir();
				PlayerPacketServer.PlayersColour = PlayerCharacter.GetColour();
				PlayerPacketServer.ServerTimeStamp = LocalGameClock.getElapsedTime().asSeconds();

				
				sf::Packet InitPacket;
				InitPacket << PlayerPacketServer;


				m_UDPSocket.send(InitPacket, iter->GetIP(), iter->GetThePort());
			}
		}
	}
}

void Application::ClientManagePacketsfromUDPSocket()
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
					float x = iter->GetXPos();
					float y = iter->GetYPos();

					iter->AddToPredictionList(PlayerPacket.XPos, PlayerPacket.YPos, PlayerPacket.ServerTimeStamp, &LocalGameClock,x , y);
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


				NewCharacter->Init(PlayerPacket.ID, (PlayerColour)PlayerPacket.PlayersColour, pos, "Assets/Textures/Chars.png", DebugScreen);
				m_NetworkCharacterList.push_back(NewCharacter);

			}
		}

	}
}
