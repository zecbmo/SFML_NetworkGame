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
	virtual void Movement(float dt);

	void Render(sf::RenderWindow* Window);

	//Getters and Setters
	inline sf::Uint32 GetColour() { return (sf::Uint32)m_Colour; };
	inline float GetXPos() { return m_Sprite.getPosition().x; };
	inline float GetYPos() { return m_Sprite.getPosition().y; };
	inline sf::Uint8 GetID() { return m_ID; };
	inline sf::Uint8 GetDir() { return  (sf::Uint8)m_Dir; };

	

protected:

	//rendering 
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;

	int m_Texture_Width; //Width of the tile
	int m_Texture_Height; //Height of the tile
	int m_Sprite_X_Pos;
	int m_Sprite_Y_Pos;

	sf::Uint8 m_ID;

	float m_Speed;

	PlayerDirection m_Dir;
	PlayerColour m_Colour;

	//Debug
	DebugUI* m_Debug;
	sf::Text* PlayerPositionText;


	void UpdateSprite(int SpritePos,float dt);
	void UpdateSpriteState(float dt);
	//animating the sprites
	float m_FramesPerSecond;
	float m_AnimCounter;
	int m_CurrentSpritePos;
	
};

