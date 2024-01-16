#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>



class Tetramino
{
public:
	enum class Type 
	{
		O,
		I,
		S,
		Z,
		L,
		J,
		T,
		SIZE
	};

	static constexpr unsigned ROTATIONS_COUNT = 4; 
	static constexpr unsigned MATRIX_SIZE = 4;     

	static const unsigned int PIVOT_Y = 1;         
	static const unsigned int PIVOT_X = 2;         

	static const sf::Color COLORS[int(Type::SIZE)]; 
	static const int SHAPES[int(Type::SIZE)][ROTATIONS_COUNT][MATRIX_SIZE][MATRIX_SIZE];  
	                                                                                     
	                                                                                     
	Tetramino(Type type, int rotation);

	Type GetType() const;
	void SetRotation(int rotation);
	int GetRotation() const;
	void SetPosition(int x, int y);
	int GetPositionX() const;
	int GetPositionY() const;
	float GetTimeSinceLastTransform() const;

private:
	const Type type;
	int rotation;
	int positionX;
	int positionY;
	sf::Clock clockSinceLastTransform; 
};
