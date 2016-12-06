#pragma once
#include<SFML\Graphics.hpp>
#include "DebugUI.h"

/*
*	Colours that the player can be
*/
enum PlayerColour { kRed, kPurple, kBlack, kGreen };
/*
*	Direcitons the player can move in. limited to N/S/E/W for now
*/
enum PlayerDirection {kNorth, kSouth, kEast, kWest, kNorthEast, kNorthWest, kSouthEast, kSouthWest, kStill};

/*
*	The Player Character
*
*	The Player Character is a player game object that can be controlled by the local user
*	The Directional Pad controls movement and changes the player state.
*	Also controls if the player can place bombs and interaction in the game
*	This object will be accessed to get positon for sending to other clients
*/
class PlayerCharacter
{
public:
	PlayerCharacter();
	~PlayerCharacter();

	/*
	*	The Initialiser for the player character
	*
	*	@param Player Id that was given by the host
	*	@param The selected player colour
	*	@param The starting position
	*	@param the Filename location the textures for the player
	*	@pointer to the debug screen for adding messages
	*/
	void Init(int PlayerID, PlayerColour Colour, sf::Vector2f StartingPos, std::string Filename, DebugUI& DebugScreen);
	
	/*
	*	Update Function
	*
	*	@param Delta Time
	*/
	virtual void Update(float dt);

	/*
	*	Bomb Requests
	*
	*	Controlls whether tha player can set bombs
	*	
	*	@param delta time
	*	@param the current game time
	*	@return returns true if the player plants a bomb
	*/
	bool BombRequests(float dt, float GameTime);

	/*
	*	The Render Function
	*
	*	@param Pointer to the SFML window
	*/
	void Render(sf::RenderWindow* Window);

	//Getters and Setters
	/*
	*	Getter for the player colour
	*/
	inline sf::Uint32 GetColour() { return (sf::Uint32)m_Colour; };
	/*
	*	Getters and Setters for player positions
	*/
	inline float GetXPos() { return m_Sprite.getPosition().x; };
	inline float GetYPos() { return m_Sprite.getPosition().y; };
	/*
	*	Getter For the player ID
	*/
	inline sf::Uint8 GetID() { return m_ID; };
	/*
	*	Getter For the player Direction
	*/
	inline sf::Uint8 GetDir() { return  (sf::Uint8)m_Dir; };

	/*
	*	Gets the delay set for each bomb
	*	Plan would be that different power ups could increase/decrease bombs
	*/
	float GetBombExplosionDelay() { return m_BombExplosionDelay; };
	

protected:
	//Updates	
	/*
	*	Updates the movement of the player character
	*
	*	@param Delta time
	*/
	virtual void Movement(float dt);
	
	//Bomb Clock
	/*
	*	Members controlling the bomb timer/times
	*/
	sf::Clock m_BombClock;
	float m_BombFireRate;
	float m_BombExplosionDelay;
	

	//rendering 
	/*
	*	Members controlling sprite and animation
	*/
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;

	int m_Texture_Width; //Width of the tile
	int m_Texture_Height; //Height of the tile
	int m_Sprite_X_Pos;
	int m_Sprite_Y_Pos;

	//player identifiers
	/*
	*	Player Identifiers
	*/
	sf::Uint8 m_ID;	
	PlayerColour m_Colour;

	//speed which they can move
	/*
	*	Speed in which player can move
	*/
	float m_Speed;	
	
	//Debug
	/*
	*	Members controlling the debug screen/text
	*/
	DebugUI* m_Debug;
	sf::Text* PlayerPositionText;
	sf::Vector2f m_Position;

	//Animation
	/*
	*	Animates the sprirte
	*
	*	@param the position of the sprite to be at on the texture (Starting animation point)
	*	@param delta time
	*/
	void UpdateSprite(int SpritePos,float dt);
	/*
	*	Updates the current state of the sprite (Controlled by the direction the player is moving)
	*
	*	@param delta time
	*/
	void UpdateSpriteState(float dt);
	//animating the sprites
	/*
	*	Members controlling animation
	*/
	float m_FramesPerSecond;
	float m_AnimCounter;
	int m_CurrentSpritePos;
	PlayerDirection m_Dir;
	
	/*
	*	Wierd bug in SFML that returns NAN when calling GetSpritePosition from the Networked Character
	*	Setting the position once from the net char seems to fix it
	*/
	bool m_DoOnce; //fixes sfml bug for the networked characters - (Accessing m_Sprite position returning nan(IND))
};

