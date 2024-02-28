//TODO delete all Lazy shit
//TODO Improve AI if necessairy 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <random>
#include "../include/Ltimer.h" 
#include "../include/LTexture.h"
#include "../include/Obstacle.h"
#include "../include/Dot.h"
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

bool init();

bool loadMedia();

void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture;

//Rendered texture
LTexture gTextTexture;

TTF_Font* gFont = NULL;


bool init()
{
	bool success = true;
  if (TTF_Init() != 0) {
  std::cout << "TTf_Init" << "\n";
  SDL_Quit();
  return 1;
}
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	bool success = true;

	if( !gDotTexture.loadFromFile( "/tools/dot.bmp" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

  gFont = TTF_OpenFont( "/tools/lazy.ttf", 68 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		SDL_Color textColor = { 255, 100, 100 };
		if( !gTextTexture.loadFromRenderedText( "Game Over", textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	return success;
}

void close()
{
	gDotTexture.free();

	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
  TTF_Quit();
	SDL_Quit();
}

bool Dot::checkCollision( const SDL_Rect& a, const SDL_Rect& b )
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    return true;
}

void Restart(std::vector<Dot>& d, std::vector<Obstacle>& walls) {
  for (auto& el : d) {
    el.restart();
  }
  int n = walls.size();
  for (int i = 0; i < n; ++i) {
    walls.pop_back();
  }
  Obstacle a(300, 300, 300, 300);
  Obstacle b(150, 450, 600, 600);
  Obstacle c(200, 400, 900, 900);
  walls.push_back(a);
  walls.push_back(b);
  walls.push_back(c);
}

int main( int argc, char* args[] )
{
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			bool quit = false;
      bool global_end;
      int generation = 1;
			
      SDL_Event e;

      std::vector<Dot> birds(20);
      LTimer stepTimer;
      std::vector<Obstacle> walls;
      Obstacle a(300, 300, 300, 300);
      Obstacle b(150, 450, 600, 600);
      Obstacle c(200, 400, 900, 900);
      walls.push_back(a);
      walls.push_back(b);
      walls.push_back(c);

			while( !quit )
			{
				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

          if (e.key.keysym.sym == SDLK_r) {
            Restart(birds, walls);
          }
					//Handle input for the dot
          // if (!dot.end_game) {
					//   dot.handleEvent( e );
          // }
				}
        float timeStep = stepTimer.getTicks()/1000.f;
        global_end = birds[0].end_game;
        for (const auto& el : birds) {
          global_end *= el.end_game;
        }
        if (!global_end) {
          for (int i = 0; i < walls.size(); ++i) {
            walls[i].move();
          }
				  for (auto& el : birds) {
            if (el.end_game) {
              continue;
            }
            el.move( timeStep, walls );
          }
          for (int i = 0; i < walls.size(); ++i) {
            walls[i].reset();
          }
        }
        stepTimer.start();
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
				
        for (int i = 0; i < walls.size(); ++i) {
          walls[i].render();
        }

				for (auto& el : birds) {
          if (el.end_game) {
            continue;
          }
          el.render();
        }
        if (global_end) {
          std::sort(birds.begin(), birds.end(), [](const Dot left, const Dot right) {
            return left.length_path > right.length_path;
          });
          generation++;
          std::cout << "generation " << generation - 1 << ", max len: " << birds[0].length_path <<", max score: " << birds[0].score << "\n";
          Enfants(birds);
          Restart(birds, walls);
        }
        if (global_end) {
          gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTextTexture.getHeight() ) / 2 );
        }
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	close();

	return 0;
}
