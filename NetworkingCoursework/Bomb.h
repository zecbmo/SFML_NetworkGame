#pragma once
#include "SFML/Graphics.hpp"

/*
*	The Bomb class will Display bombs and the screen and blow up after a given time
*/

class Bomb
{
public:
	
	/*
	*	The Bomb Constructor
	*
	*	@param Positon of the bomb to be placed
	*	@param The Server time in which it should explode
	*/
	Bomb(sf::Vector2f Position, float TimeToExplode);
	~Bomb();

	/*
	*	Update the bombs - Checks if the should explode against the current time
	*
	*	@param Delta time
	*	@param Current Game Time
	*/
	void Update(float dt, float CurrentTime);
	/*
	*	Render in the Selected Window
	*
	*	@param The SFML Window pointer
	*/
	void Render(sf::RenderWindow* Window);
	/*
	*	Returns if the bomb has exploded or not
	*	@returns true if bomb exploded
	*/
	bool IsExploded() { return m_IsExploded; };

private:
	/*
	*	The Game time in which the bomb will explode
	*/
	float m_TimeToExplode;
	/*
	*	Bool to monitor if exploded
	*/
	bool m_IsExploded;
	/*
	*	Function telling the bomb what to do when exploded
	*	Needs to be expanded on
	*/
	void Explode();

	//rendering
	/*
	*	Members for rendering
	*/
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
	int m_TextureWidth;

	//animating
	/*
	*	Members for animating
	*/
	int m_CurrentSpritePos;
	float m_AnimCounter;
	float m_FramesPerSecond;
};

