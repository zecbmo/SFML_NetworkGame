#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Application.h"

#define SCREEN_WIDTH 1200
#define SCREEN_DEPTH 800

int main()
{
	
	

	//Set Up window And clock
	sf::RenderWindow Window(sf::VideoMode(SCREEN_WIDTH, SCREEN_DEPTH), "BomberDude");
	sf::Color WindowColour(50, 50, 50, 255);
	sf::Clock DeltaClock;

	//Create The Application
	Application Game;
	Game.Init(&Window, SCREEN_WIDTH, SCREEN_DEPTH);
	


	while (Window.isOpen())
	{
		//Set up the clock and delta time 
		sf::Time FrameTime = DeltaClock.restart();
		float DeltaTime = FrameTime.asSeconds();

		//If the application is asked to close then shut down
		sf::Event Event;
		while (Window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
				Window.close();
		}

		Window.clear(WindowColour);
		
		//Update and render the application here
		Game.Update(DeltaTime);
		Game.Render();

		Window.display();
	}

	return 0;
}