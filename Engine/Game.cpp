/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	brd( gfx ),
	rng( std::random_device()() ),
	snek( {2,2} )
{
	//sndTitle.Play( 1.0f,1.0f );



	for (int i = 0; i < nFoods; i++) {
		
		brd.SpawnOccupants(rng, snek, Board::Occupant::Food); // Food
	}
	for (int i = 0; i < nPoisons; i++) {
		brd.SpawnOccupants(rng, snek, Board::Occupant::Poison); // Poison
	}
	for (int i = 0; i < nObstacles; i++) {
		brd.SpawnOccupants(rng, snek, Board::Occupant::Obstacle); // Obstacle
	}
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = ft.Mark();

	if( gameIsStarted )
	{
		if( !gameIsOver )
		{
			if( wnd.kbd.KeyIsPressed( VK_UP ) )
			{
				delta_loc = { 0,-1 };
			}
			else if( wnd.kbd.KeyIsPressed( VK_DOWN ) )
			{
				delta_loc = { 0,1 };
			}
			else if( wnd.kbd.KeyIsPressed( VK_LEFT ) )
			{
				delta_loc = { -1,0 };
			}
			else if( wnd.kbd.KeyIsPressed( VK_RIGHT ) )
			{
				delta_loc = { 1,0 };
			}

			snekMoveCounter += (speedUpFactor)*dt;
			if( snekMoveCounter >= snekMovePeriod )
			{
				snekMoveCounter -= snekMovePeriod;
				const Location next = snek.GetNextHeadLocation( delta_loc );
				if( !brd.IsInsideBoard( next ) || snek.IsInTileExceptEnd( next ) || brd.CheckForOccupancy(next) == Board::Occupant::Obstacle) // If snek has accident
				{
					gameIsOver = true;
					sndFart.Play();
					sndMusic.StopAll();
				}
				
				switch (brd.CheckForOccupancy(next)) {
				case Board::Occupant::Food: {
					snek.GrowAndMoveBy(delta_loc);

					brd.FillOccupancy(next, Board::Occupant::Empty); // set to empty tile
					brd.SpawnOccupants(rng, snek, Board::Occupant::Obstacle); // spawn obstacle
					sfxEat.Play(rng, 0.8f);
					break;
				}
				case Board::Occupant::Poison: {

					brd.FillOccupancy(next, Board::Occupant::Empty); // set to empty tile
					speedUpFactor += 0.1f;    // speed up the snek speed when poison eaten
					snek.MoveBy(delta_loc);
					sfxEat.Play(rng, 0.8f);
					break;
				}
				default: {
					snek.MoveBy(delta_loc);
				}
				}
				//if( brd.CheckForOccupancy(next) == Board::Occupant::Food) // If Food is eaten
				//{
				//	snek.GrowAndMoveBy( delta_loc );
				//		
				//	brd.FillOccupancy(next, Board::Occupant::Empty); // set to empty tile
				//	brd.SpawnOccupants(rng, snek, Board::Occupant::Obstacle); // spawn obstacle
				//	sfxEat.Play( rng,0.8f );

				//}
				//else if (brd.CheckForOccupancy(next) == Board::Occupant::Poison) // If Poison is eaten
				//{
				//	

				//	brd.FillOccupancy(next, Board::Occupant::Empty); // set to empty tile
				//	speedUpFactor += 0.1f;    // speed up the snek speed when poison eaten
				//	snek.MoveBy(delta_loc);
				//	sfxEat.Play(rng, 0.8f);

				//}
				//else{
				//	snek.MoveBy(delta_loc);
				//}



				sfxSlither.Play( rng,0.08f );
				
			}
			snekMovePeriod = std::max( snekMovePeriod - dt * snekSpeedupFactor,snekMovePeriodMin );
		}
	}
	else
	{
		if( wnd.kbd.KeyIsPressed( VK_RETURN ) )
		{
			sndMusic.Play( 1.0f,0.6f );
			gameIsStarted = true;
		}
	}
}

void Game::ComposeFrame()
{
	if( gameIsStarted )
	{
		snek.Draw( brd );
		
		brd.DrawOccupants();
		if( gameIsOver )
		{
			SpriteCodex::DrawGameOver( 350,265,gfx );
		}
		brd.DrawBorder();
	}
	else
	{
		SpriteCodex::DrawTitle( 290,225,gfx );
	}
}
