#pragma once
#include <SFML/Graphics.hpp>
#include "Tools/ResourceManager.h"
#include "World/Board.h"
#include "World/Statistics.h"

class Game
{
public:
	enum class Sound
	{
		GameOver,
		NextLevel,
		RowIsCleared
	};

	Game();

	void Run();
	static void PlaySound(Sound sound);

private:
	enum class ScreenResolution
	{
		_FullHD, 
		_2K,    
		_4K,     
		Other    
	};

	enum class Font
	{
		GUI 
	};

	enum class Music
	{
		BackgroundTheme
	};

	static ResourceManager<sf::Font, Font> fonts;
	static ResourceManager<sf::SoundBuffer, Sound> sounds;
	static ResourceManager<sf::Music, Music> music;

	const sf::String WINDOW_TITLE = "Tetris";
	sf::VideoMode windowSize;
	sf::RenderWindow window;

	std::unique_ptr<Tetramino> currentTetramino;
	Board board;
	Statistics statistics;
	float timeToMoveTetraminoDown;

	int GetScreenScaleFactor(sf::VideoMode size) const;

	void Initialize();
	void InitializeFonts();
	void InitializeSounds();
	void InitializeMusic();

	void ProcessEvents();
	void Update(float deltaTime);
	void Render();

	void CreateNewTetramino();
};
