#pragma once
#include <string>
#include <assert.h>
#include "Figure.h"

#define NUM_OF_FIGURE_TYPES 6

class Resources
{
	std::string pathToMapImage;
	std::string pathToFiguresImage;
	FigureType sequenceOfFiguresInImage[NUM_OF_FIGURE_TYPES * 2];
	int figuresInRow;
	int figuresInCol;
public:
	Resources() {}
	Resources(std::string mapPath, std::string figuresPath, FigureType sequence[NUM_OF_FIGURE_TYPES * 2], int rowsInImage, int columnsInImage)
		: pathToMapImage(mapPath), pathToFiguresImage(figuresPath), figuresInRow(rowsInImage), figuresInCol(columnsInImage)
	{
		SetSequenceOfFiguresInImage(sequence);
	}

	std::string GetPathToMapImage() const { return pathToMapImage; }
	std::string GetPathToFiguresImage() const { return pathToFiguresImage; }
	int GetRowsInFiguresImage() const { return figuresInRow; }
	int GetColumnsInFiguresImage() const { return figuresInCol; }
	FigureType GetFigureTypeAt(int index) const
	{
		assert(index >= 0 && index < NUM_OF_FIGURE_TYPES * 2);
		return sequenceOfFiguresInImage[index];
	}

	void SetPathToMapImage(std::string mapPath) { pathToMapImage = mapPath; }
	void SetPathToFiguresImage(std::string figuresPath) { pathToFiguresImage = figuresPath; }
	void SetRowsAndColsInFiguresImage(int rows, int columns)
	{
		figuresInRow = rows;
		figuresInCol = columns;
	}
	void SetSequenceOfFiguresInImage(FigureType sequence[NUM_OF_FIGURE_TYPES * 2])
	{
		for (int i = 0; i < NUM_OF_FIGURE_TYPES * 2; ++i)
			sequenceOfFiguresInImage[i] = sequence[i];
	}
};