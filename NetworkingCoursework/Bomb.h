#pragma once
#include "SFML/Graphics.hpp"

class Bomb
{
public:
	Bomb(sf::Vector2f Position, float TimeToExplode);
	~Bomb();

	void Update(float dt);
	void Render(sf::RenderWindow* Window);
	bool IsExploded() { return m_IsExploded; };

private:
	sf::Clock m_BombClock;
	float m_TimeToExplode;
	bool m_IsExploded;
	void Explode();

	//rendering
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
	int m_TextureWidth;

	//animating
	int m_CurrentSpritePos;
	float m_AnimCounter;
	float m_FramesPerSecond;
};

