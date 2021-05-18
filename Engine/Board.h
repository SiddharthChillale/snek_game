#pragma once

#include "Graphics.h"
#include "Location.h"
#include <random>


class Board
{
public:
	enum class Occupant {
		Empty,
		Food,
		Poison,
		Obstacle
	};
public:
	Board( Graphics& gfx );
	void DrawCell( const Location& loc,Color c );
	int GetGridWidth() const;
	int GetGridHeight() const;
	bool IsInsideBoard( const Location& loc ) const;
	Occupant CheckForOccupancy(const Location& loc)const;
	
	void FillOccupancy(const Location& loc, const Occupant occupant);
	void SpawnOccupants(std::mt19937& rng, const class Snake& snek, const Occupant occupant);
	
	void DrawBorder();
	void DrawOccupants();

private:
	static constexpr Color borderColor = Colors::Blue;
	static constexpr Color obstacleColor = Colors::Gray;
	static constexpr Color foodColor = Colors::Red;
	static constexpr Color poisonColor = Colors::Magenta;

	
	static constexpr int dimension = 20;
	static constexpr int cellPadding = 1;
	static constexpr int width = 32;
	static constexpr int height = 24;
	static constexpr int borderWidth = 4;
	static constexpr int borderPadding = 2;
	static constexpr int x = 70;
	static constexpr int y = 50;
	// has occupancy 0: Empty, 1: Food, 2: Poison, 3: Obstacle
	Occupant hasOccupancy[width * height] = { Occupant::Empty };
	Graphics& gfx;
};