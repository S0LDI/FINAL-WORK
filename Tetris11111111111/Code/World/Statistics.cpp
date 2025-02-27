#include "Statistics.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include "../Game.h"

Statistics::Statistics()
{
	font.loadFromFile("../Assets/Fonts/trs-million.ttf");

	textCompletedRowsCount.setFont(font);
	textCompletedRowsCount.setString("Rows: 0");
	textCompletedRowsCount.setCharacterSize(FONT_SIZE);
	textCompletedRowsCount.setPosition(0, 0); 

	textScore.setFont(font);
	textScore.setString("Score: 0");
	textScore.setCharacterSize(FONT_SIZE);
	textScore.setPosition(0, FONT_SIZE + 1); 

	textCurrentLevel.setFont(font);
	textCurrentLevel.setString("Level: 0");
	textCurrentLevel.setCharacterSize(FONT_SIZE);
	textCurrentLevel.setPosition(0, (FONT_SIZE + 1) * 2); 

	textTutorialInfo.setFont(font);
	textTutorialInfo.setString("Use arrow keys - Move\nA, D - Rotate\nSpace - Drop\nEscape - Quit the game");
	textTutorialInfo.setFillColor(sf::Color(50, 255, 50));
	textTutorialInfo.setCharacterSize(FONT_SIZE);
	textTutorialInfo.setPosition(0, (FONT_SIZE + 1) * 4); 

	
	float textGameOverPositionX = -150;
	float textGameOverPositionY = 170;
	textGameOver.setFont(font);
	textGameOver.setString("Game Over");
	textGameOver.setCharacterSize(GAME_OVER_FONT_SIZE);
	textGameOver.setPosition(textGameOverPositionX, textGameOverPositionY);
}

void Statistics::AddRows(int newCompletedRowsCount)
{
	if (newCompletedRowsCount == 0)
		return;

	
	completedRowsCount += newCompletedRowsCount;
	textCompletedRowsCount.setString("Rows: " + std::to_string(completedRowsCount));

	
	switch (newCompletedRowsCount)
	{
	case 1:
		score += 40 * (currentLevel + 1);
		break;
	case 2:
		score += 100 * (currentLevel + 1);
		break;
	case 3:
		score += 300 * (currentLevel + 1);
		break;
	case 4:
		score += 1200 * (currentLevel + 1);
		break;
	}
	textScore.setString("Score: " + std::to_string(score));

	
	int oldLevel = currentLevel;
	currentLevel = completedRowsCount / 10; 
	if (currentLevel > oldLevel) 
		Game::PlaySound(Game::Sound::NextLevel);

	textCurrentLevel.setString("Level: " + std::to_string(currentLevel));
}

int Statistics::GetCurrentLevelNumber() const
{
	return currentLevel;
}

void Statistics::SetGameOver()
{
	isGameOver = true;
}

void Statistics::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	if (!isGameOver)
	{
		target.draw(textCompletedRowsCount, states);
		target.draw(textScore, states);
		target.draw(textCurrentLevel, states);
		target.draw(textTutorialInfo, states);
	}
	else
	{
		target.draw(textGameOver, states);
	}
}
