#include "Board.h"
#include <assert.h>
#include "Snake.h"

Board::Board( Graphics& gfx )
	:
	gfx( gfx )
{
	 
}

void Board::DrawCell( const Location & loc,Color c )
{
	assert( loc.x >= 0 );
	assert( loc.x < width );
	assert( loc.y >= 0 );
	assert( loc.y < height );

	const int off_x = x + borderWidth + borderPadding;
	const int off_y = y + borderWidth + borderPadding;

	gfx.DrawRectDim( loc.x * dimension + off_x + cellPadding,loc.y * dimension + off_y + cellPadding,dimension - cellPadding * 2,dimension - cellPadding * 2,c );
}

int Board::GetGridWidth() const
{
	return width;
}

int Board::GetGridHeight() const
{
	return height;
}

bool Board::IsInsideBoard( const Location & loc ) const
{
	return loc.x >= 0 && loc.x < width &&
		loc.y >= 0 && loc.y < height;
}

void Board::DrawBorder()
{
	const int top = y;
	const int left = x;
	const int bottom = top + (borderWidth + borderPadding) * 2 + height * dimension;
	const int right = left + (borderWidth + borderPadding) * 2 + width * dimension;

	// top
	gfx.DrawRect( left,top,right,top + borderWidth,borderColor );
	// left
	gfx.DrawRect( left,top + borderWidth,left + borderWidth,bottom - borderWidth,borderColor );
	// right
	gfx.DrawRect( right - borderWidth,top + borderWidth,right,bottom - borderWidth,borderColor );
	// bottom
	gfx.DrawRect( left,bottom - borderWidth,right,bottom,borderColor );
}

void Board::DrawOccupants()
{
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			
			switch (CheckForOccupancy({ x, y })) {
			case Occupant::Food:
				DrawCell({ x, y }, foodColor);
				break;
			case Occupant::Poison:
				DrawCell({ x, y }, poisonColor);
				break;
			case Occupant::Obstacle:
				DrawCell({ x, y }, obstacleColor);
				break;
			}
			/*if (CheckForOccupancy({ x, y }) == Occupant::Food) {
				DrawCell({ x, y }, foodColor);
			}
			if (CheckForOccupancy({ x, y }) == Occupant::Poison) {
				DrawCell({ x, y }, poisonColor);
			}
			if (CheckForOccupancy({ x, y }) == Occupant::Obstacle) {
				DrawCell({ x, y }, obstacleColor);
			}*/

		}
	}
}

Board::Occupant Board::CheckForOccupancy(const Location& loc) const
{
	return hasOccupancy[loc.y*width + loc.x];
}


void Board::FillOccupancy(const Location& loc, const Occupant occupant)
{
	
	hasOccupancy[loc.y * width + loc.x] = occupant;
}

void Board::SpawnOccupants(std::mt19937& rng, const Snake& snek, const Occupant occupant)
{
	std::uniform_int_distribution<int> xDist(0, GetGridWidth() - 1);
	std::uniform_int_distribution<int> yDist(0, GetGridHeight() - 1);

	Location newLoc;
	do
	{
		newLoc.x = xDist(rng);
		newLoc.y = yDist(rng);
	} while (snek.IsInTile(newLoc) || CheckForOccupancy(newLoc) != Occupant::Empty );

	hasOccupancy[newLoc.y * width + newLoc.x] = occupant;
}


