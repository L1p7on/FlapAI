#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <utility>
#include "Obstacle.h"
#include "LTexture.h"
#ifndef DOT_H_
#define DOT_H_
extern LTexture gDotTexture;
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 640;

		//Initializes the variables
		Dot();

    bool end_game = false;

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot and checks collision
		void move( float timeStep, std::vector<Obstacle>& walls );

		//Shows the dot on the screen
		void render();

    bool checkCollision(const SDL_Rect& a, const SDL_Rect& b); 

    void restart();

    int score = 0;

    int length_path = 0;

    int number_of_neurons = 4;

    std::vector<std::vector<double> > weights_before_relu;

    std::vector<double> weights_after_relu;

    std::vector<double> auxiliary_matrix;

    std::pair<int, int> dif (const std::vector<Obstacle>& walls);

    bool decision_making (const int diff_x, const int diff_y);

    private:
		//The X and Y offsets of the dot
		float mPosX, mPosY;

		//The velocity of the dot
		// int mVelX, mVelY;
    float mVelX, mVelY;
		float gravity = 300;
		//Dot's collision box
		SDL_Rect mCollider;
};

int ReLU(double x);

void Enfants(std::vector<Dot>& birds); // Enfants are children in french
#endif // DOT_H_
