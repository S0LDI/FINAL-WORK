#include "Board.h"

#include <cassert>

Board::Board(int rows, int columns, int cellWidth, int cellHeight)
	: ROWS(rows), COLUMNS(columns), CELL_WIDTH(cellWidth), CELL_HEIGHT(cellHeight)
	, WIDTH(rows * CELL_WIDTH), HEIGHT(columns* CELL_HEIGHT)
{
	
	gridContent.resize(rows * columns);
	std::fill(gridContent.begin(), gridContent.end(), EMPTY_CELL_NUMBER);

	
	const sf::Color GRID_LINE_COLOR(55, 55, 55); 
	int totalVerticesCount = (rows + 1 + columns + 1) * 2; 
	grid = sf::VertexArray(sf::Lines, totalVerticesCount);

	

	
	for (int rowIndex = 0; rowIndex <= rows; rowIndex++)
	{
		
		

		grid[rowIndex * 2] = sf::Vertex(sf::Vector2f(0, rowIndex * CELL_HEIGHT));
		grid[rowIndex * 2 + 1] = sf::Vertex(sf::Vector2f(columns * CELL_WIDTH, rowIndex * CELL_HEIGHT));

		grid[rowIndex * 2].color = GRID_LINE_COLOR;
		grid[rowIndex * 2 + 1].color = GRID_LINE_COLOR;
	}
	
	for (int columnIndex = 0; columnIndex <= columns; columnIndex++)
	{
		
		
		

		grid[(rows + 1) * 2 + columnIndex * 2] = sf::Vertex(sf::Vector2f(columnIndex * CELL_WIDTH, 0));
		grid[(rows + 1) * 2 + columnIndex * 2 + 1] = sf::Vertex(sf::Vector2f(columnIndex * CELL_WIDTH, rows * CELL_HEIGHT));

		grid[(rows + 1) * 2 + columnIndex * 2].color = GRID_LINE_COLOR;
		grid[(rows + 1) * 2 + columnIndex * 2 + 1].color = GRID_LINE_COLOR;
	}
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (int rowIndex = 0; rowIndex < ROWS; rowIndex++)
	{
		for (int columnIndex = 0; columnIndex < COLUMNS; columnIndex++)
		{
			if (gridContent[rowIndex * COLUMNS + columnIndex] != EMPTY_CELL_NUMBER) 
			{
				sf::RectangleShape rectangle(sf::Vector2f(CELL_WIDTH, CELL_HEIGHT));
				rectangle.setFillColor(Tetramino::COLORS[gridContent[rowIndex * COLUMNS + columnIndex]]);
				rectangle.setPosition(columnIndex * CELL_WIDTH, rowIndex * CELL_HEIGHT);
				target.draw(rectangle, states);
			}
		}
	}

	target.draw(grid, states);
}

void Board::Spawn(Tetramino& tetramino)
{
	tetramino.SetPosition(COLUMNS / 2, 0);
	ClearFrom(tetramino);

	
	
	for (int columnIndex = 0; columnIndex < COLUMNS; columnIndex++)
	{
		if (gridContent[columnIndex] != EMPTY_CELL_NUMBER)
		{
			isGameOver = true;
			break;
		}
	}

	Add(tetramino);
}

bool Board::Move(Tetramino& tetramino, int offsetX, int offsetY)
{
	offsetX += tetramino.GetPositionX();
	offsetY += tetramino.GetPositionY();

	ClearFrom(tetramino); 

	
	bool visitedCells[Tetramino::MATRIX_SIZE][Tetramino::MATRIX_SIZE] = { false };
	bool canBeMoved = true;
	PerformFloodFill(
		offsetX, offsetY,
		Tetramino::PIVOT_X, Tetramino::PIVOT_Y,
		tetramino.GetType(), tetramino.GetRotation(),
		visitedCells, canBeMoved
	);

	if (canBeMoved)
		tetramino.SetPosition(offsetX, offsetY);

	Add(tetramino); 
	return canBeMoved;
}

bool Board::IsFallen(const Tetramino& tetramino)
{
	ClearFrom(tetramino); 

	
	bool visitedCells[Tetramino::MATRIX_SIZE][Tetramino::MATRIX_SIZE] = { false };
	bool isFallen = true;
	PerformFloodFill(
		tetramino.GetPositionX(), tetramino.GetPositionY() + 1,
		Tetramino::PIVOT_X, Tetramino::PIVOT_Y,
		tetramino.GetType(), tetramino.GetRotation(),
		visitedCells, isFallen
	);

	Add(tetramino);

	return isFallen;
}

void Board::Drop(Tetramino& tetramino)
{
	
	while (Move(tetramino, 0, 1))
		continue;
}

bool Board::RotateLeft(Tetramino& tetramino)
{
	int rotation = tetramino.GetRotation();
	rotation = (rotation > 0) ? rotation - 1 : Tetramino::ROTATIONS_COUNT - 1;

	return Rotate(tetramino, rotation);
}

bool Board::RotateRight(Tetramino& tetramino)
{
	int rotation = tetramino.GetRotation();
	rotation = (rotation < Tetramino::ROTATIONS_COUNT - 1) ? rotation + 1 : 0;

	return Rotate(tetramino, rotation);
}

bool Board::IsGameOver()
{
	return isGameOver;
}

bool Board::Rotate(Tetramino& tetramino, int rotation)
{
	assert(rotation >= 0 && rotation < Tetramino::ROTATIONS_COUNT);

	ClearFrom(tetramino);

	
	bool visitedCells[Tetramino::MATRIX_SIZE][Tetramino::MATRIX_SIZE] = { false };
	bool canBeRotated = true;
	PerformFloodFill(
		tetramino.GetPositionX(), tetramino.GetPositionY(),
		Tetramino::PIVOT_X, Tetramino::PIVOT_Y,
		tetramino.GetType(), rotation,
		visitedCells, canBeRotated
	);

	if (canBeRotated)
		tetramino.SetRotation(rotation);

	Add(tetramino);

	return canBeRotated;
}

void Board::Add(const Tetramino& tetramino)
{
	PerformFloodFill(tetramino, int(tetramino.GetType()));
}

void Board::ClearFrom(const Tetramino& tetramino)
{
	PerformFloodFill(tetramino, EMPTY_CELL_NUMBER);
}

void Board::PerformFloodFill(const Tetramino& tetramino, int value)
{
	
	
	bool visitedCells[Tetramino::MATRIX_SIZE][Tetramino::MATRIX_SIZE] = { false };

	PerformFloodFill(
		tetramino.GetPositionX(), tetramino.GetPositionY(),
		Tetramino::PIVOT_X, Tetramino::PIVOT_Y,
		tetramino.GetType(), tetramino.GetRotation(),
		visitedCells, value
	);
}

void Board::PerformFloodFill(
	int gridX, int gridY,
	int tetraminoX, int tetraminoY,
	Tetramino::Type type, int rotation,
	bool visitedCells[][Tetramino::MATRIX_SIZE], int value)
{
	if (tetraminoX < 0 || tetraminoX >= Tetramino::MATRIX_SIZE || tetraminoY < 0 || tetraminoY >= Tetramino::MATRIX_SIZE 
		|| gridX < 0 || gridX >= COLUMNS || gridY < 0 || gridY >= ROWS                                                 
		|| visitedCells[tetraminoY][tetraminoX] == true                                                                  
		|| Tetramino::SHAPES[int(type)][rotation][tetraminoY][tetraminoX] == 0)                                          
		return;

	visitedCells[tetraminoY][tetraminoX] = true;

	gridContent[gridY * COLUMNS + gridX] = value;

	
	PerformFloodFill(gridX, gridY - 1, tetraminoX, tetraminoY - 1, type, rotation, visitedCells, value);
	PerformFloodFill(gridX + 1, gridY, tetraminoX + 1, tetraminoY, type, rotation, visitedCells, value);
	PerformFloodFill(gridX, gridY + 1, tetraminoX, tetraminoY + 1, type, rotation, visitedCells, value);
	PerformFloodFill(gridX - 1, gridY, tetraminoX - 1, tetraminoY, type, rotation, visitedCells, value);
}

void Board::PerformFloodFill(
	int gridX, int gridY,
	int tetraminoX, int tetraminoY,
	Tetramino::Type type, int rotation,
	bool visitedCells[][Tetramino::MATRIX_SIZE], bool& hasCollision)
{
	
	if (tetraminoX < 0 || tetraminoX >= Tetramino::MATRIX_SIZE || tetraminoY < 0 || tetraminoY >= Tetramino::MATRIX_SIZE 
		|| visitedCells[tetraminoY][tetraminoX] == true                                                                  
		|| Tetramino::SHAPES[int(type)][rotation][tetraminoY][tetraminoX] == 0)                                         
		return;

	visitedCells[tetraminoY][tetraminoX] = true;

	if (gridX < 0 || gridX >= COLUMNS || gridY < 0 || gridY >= ROWS    
		|| gridContent[gridY * COLUMNS + gridX] != EMPTY_CELL_NUMBER)  
	{
		hasCollision = false;
		return;
	}

	
	PerformFloodFill(gridX, gridY - 1, tetraminoX, tetraminoY - 1, type, rotation, visitedCells, hasCollision);
	PerformFloodFill(gridX + 1, gridY, tetraminoX + 1, tetraminoY, type, rotation, visitedCells, hasCollision);
	PerformFloodFill(gridX, gridY + 1, tetraminoX, tetraminoY + 1, type, rotation, visitedCells, hasCollision);
	PerformFloodFill(gridX - 1, gridY, tetraminoX - 1, tetraminoY, type, rotation, visitedCells, hasCollision);
}

void Board::ShiftRowsDown(int bottomRowIndex)
{
	assert(bottomRowIndex < ROWS);

	for (int row = bottomRowIndex; row > 0; row--) 
		for (int columnIndex = 0; columnIndex < COLUMNS; columnIndex++)
			gridContent[row * COLUMNS + columnIndex] = gridContent[(row - 1) * COLUMNS + columnIndex];
}

int Board::ClearRows(const Tetramino& tetramino)
{
	int deletedRowsCount = 0;

	ClearFrom(tetramino);

	for (int rowIndex = 0; rowIndex < ROWS; rowIndex++)
	{
		
		int emptyCellColumnIndex = 0;
		for (; emptyCellColumnIndex < COLUMNS && gridContent[rowIndex * COLUMNS + emptyCellColumnIndex] != EMPTY_CELL_NUMBER; emptyCellColumnIndex++)
			continue;

		if (emptyCellColumnIndex == COLUMNS) 
		{
			ShiftRowsDown(rowIndex);
			deletedRowsCount++;
		}
	}

	Add(tetramino);

	return deletedRowsCount;
}

int Board::GetWidth() const
{
	return WIDTH;
}

int Board::GetHeight() const
{
	return HEIGHT;
}
