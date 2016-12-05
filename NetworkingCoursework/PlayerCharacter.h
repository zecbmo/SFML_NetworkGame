#pragma once
#include<SFML\Graphics.hpp>
#include "DebugUI.h"
enum PlayerColour { kRed, kPurple, kBlack, kGreen };
enum PlayerDirection {kNorth, kSouth, kEast, kWest, kNorthEast, kNorthWest, kSouthEast, kSouthWest, kStill};





class PlayerCharacter
{
public:
	PlayerCharacter();
	~PlayerCharacter();

	void Init(int PlayerID, PlayerColour Colour, sf::Vector2f StartingPos, std::string Filename, DebugUI& DebugScreen);
	
	virtual void Update(float dt);
	bool BombRequests(float dt, float GameTime);

	void Render(sf::RenderWindow* Window);

	//Getters and Setters
	inline sf::Uint32 GetColour() { return (sf::Uint32)m_Colour; };
	inline float GetXPos() { return m_Sprite.getPosition().x; };
	inline float GetYPos() { return m_Sprite.getPosition().y; };
	inline sf::Uint8 GetID() { return m_ID; };
	inline sf::Uint8 GetDir() { return  (sf::Uint8)m_Dir; };

	float GetBombExplosionDelay() { return m_BombExplosionDelay; };
	

protected:
	//Updates	
	virtual void Movement(float dt);
	
	//Bomb Clock
	sf::Clock m_BombClock;
	float m_BombFireRate;
	float m_BombExplosionDelay;
	

	//rendering 
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;

	int m_Texture_Width; //Width of the tile
	int m_Texture_Height; //Height of the tile
	int m_Sprite_X_Pos;
	int m_Sprite_Y_Pos;

	//player identifiers
	sf::Uint8 m_ID;	
	PlayerColour m_Colour;

	//speed which they can move
	float m_Speed;	
	
	//Debug
	DebugUI* m_Debug;
	sf::Text* PlayerPositionText;
	sf::Vector2f m_Position;

	//Animation
	void UpdateSprite(int SpritePos,float dt);
	void UpdateSpriteState(float dt);
	//animating the sprites
	float m_FramesPerSecond;
	float m_AnimCounter;
	int m_CurrentSpritePos;
	PlayerDirection m_Dir;
	
	bool m_DoOnce; //fixes sfml bug for the networked characters - (Accessing m_Sprite position returning nan(IND))
};

