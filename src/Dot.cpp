#include "../include/Dot.h"
Dot::Dot() :
weights_before_relu(number_of_neurons,std::vector<double>(3,0.0)),
weights_after_relu(number_of_neurons, 0),
auxiliary_matrix(number_of_neurons, 0)
{
    //Initialize the offsets
    mPosX = 50;
    mPosY = 340;

	  //Set collision box dimension
	  mCollider.w = DOT_WIDTH;
	  mCollider.h = DOT_HEIGHT;

    //Initialize the velocity
    mVelX = 0.0;
    mVelY = 0.0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> d;
    for (int i = 0; i < number_of_neurons; ++i) {
      weights_before_relu[i][0] = d(gen);
      weights_before_relu[i][1] = d(gen);
      weights_before_relu[i][2] = d(gen);
      weights_after_relu[i] = d(gen);
    }
}

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            // case SDLK_UP: mVelY -= DOT_VEL; break;
            // case SDLK_DOWN: mVelY += DOT_VEL; break;
            // case SDLK_LEFT: mVelX -= DOT_VEL; break;
            // case SDLK_RIGHT: mVelX += DOT_VEL; break;
            case SDLK_SPACE:  mVelY = -200; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            // case SDLK_UP: mVelY += DOT_VEL; break;
            // case SDLK_DOWN: mVelY -= DOT_VEL; break;
            // case SDLK_LEFT: mVelX += DOT_VEL; break;
            // case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

void Dot::move(float timeStep, std::vector<Obstacle>& walls )
{
    for (int i = 0; i < walls.size(); ++i) {
      if (checkCollision(mCollider, walls[i].bottom)) {
        end_game = true;
        // std::cout << "Score: " << score << "\n";
        return;
      }
      if (checkCollision(mCollider, walls[i].top)) {
        end_game = true;
        // std::cout << "Score: " << score << "\n";
        return;
      }
    }
    //Also check before Move
    //Move the dot left or right
    mPosX += mVelX*timeStep; 
	  mCollider.x = mPosX;

    //If the dot collided or went too far to the left or right
    for (int i = 0; i < walls.size(); ++i) {
      if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) || checkCollision( mCollider, walls[i].top ) ) {
          //Move back
          mPosX -= mVelX*timeStep;
          mCollider.x = mPosX;
          // std::cout << "Score: " << score << "\n";
          end_game = true;
          return;
      }
      if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) || checkCollision( mCollider, walls[i].bottom ) ) {
          //Move back
          mPosX -= mVelX*timeStep;
          mCollider.x = mPosX;
          // std::cout << "Score: " << score << "\n";
          end_game = true;
          return;
      }
    }
    //Move the dot up or down

    mVelY += gravity*timeStep;
    auto diff = dif(walls);
    int diff_x = diff.first;
    int diff_y = diff.second;
    if (decision_making(diff_x, diff_y)) {
      // std::cout << "I WANNA JUMP" << "\n";
      mVelY = -200;
    }

    mPosY += mVelY*timeStep;

	  mCollider.y = mPosY;
    for (int i = 0; i < walls.size(); ++i) {
      //If the dot collided or went too far up or down
      if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) || checkCollision( mCollider, walls[i].top ) ) {
          //Move back
          mPosY -= mVelY*timeStep;
          mCollider.y = mPosY;
          // std::cout << "Score: " << score << "\n";
          end_game = true;
          return;
      }
      if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) || checkCollision( mCollider, walls[i].bottom ) ) {
          //Move back
          mPosY -= mVelY*timeStep;
          mCollider.y = mPosY;
          // std::cout << "Score: " << score << "\n";
          end_game = true;
          return;
      }
    }

    length_path += walls[0].GetVel();

    for (int i = 0; i < walls.size(); ++i) { 
      if (mPosX == walls[i].top.x+walls[i].top.w) {
        ++score;
        return;
      }
    }
}

void Dot::render()
{
    //Show the dot
	gDotTexture.render( int(mPosX), int(mPosY) );
}

void Dot::restart() {
  mPosX = 50;
  mPosY = 340;

  mCollider.x = mPosX;
  mCollider.y = mPosY;

  mVelX = 0.0;
  mVelY = 0.0;
  end_game = false;
  score = 0;
  length_path = 0;
}

int ReLU(double x) {
  return std::max(x, 0.0);
}

std::pair<int, int> Dot::dif (const std::vector<Obstacle>& walls) { 
  auto it = walls.end();
  for (auto first = walls.begin(); first != walls.end(); ++first) {
    if (first->top.x+first->top.w >= mPosX && (it==walls.end() || first->top.x+first->top.w < it->top.x + it->top.w)) {
      it = first;
    }
  }
  return {mPosX - it->top.x, mPosY - it->center};
}

bool Dot::decision_making (const int diff_x, const int diff_y) { 
  double f = 0.0;
  for (int i = 0; i < number_of_neurons; ++i) {
    auxiliary_matrix[i] = ReLU(weights_before_relu[i][0] * mVelY + weights_before_relu[i][1]*diff_x + weights_before_relu[i][2]*diff_y);
  }
  for (int i = 0; i < number_of_neurons; ++i) {
    f += weights_after_relu[i] * auxiliary_matrix[i];
  }
  if (f > 0.0) {
    return true;
  }
  return false;
}

void Enfants(std::vector<Dot>& birds) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<double> d{1, 0.06};
  for (int i = 2; i < (birds.size()-2)/2 ; ++i) { //TODO change i in a normal way
    for (int j = 0; j < birds[i].number_of_neurons; ++j) {
      birds[i].weights_before_relu[j][0] = d(gen) * birds[0].weights_before_relu[j][0];
      birds[i].weights_before_relu[j][1] = d(gen) * birds[0].weights_before_relu[j][1];
      birds[i].weights_before_relu[j][2] = d(gen) * birds[0].weights_before_relu[j][2];
      birds[i].weights_after_relu[j] = d(gen) * birds[0].weights_after_relu[j];
    }
  }
  for (int i = (birds.size()-2)/2; i < birds.size(); ++i) { 
    for (int j = 0; j < birds[i].number_of_neurons; ++j) {
      birds[i].weights_before_relu[j][0] = d(gen) * birds[1].weights_before_relu[j][0];
      birds[i].weights_before_relu[j][1] = d(gen) * birds[1].weights_before_relu[j][1];
      birds[i].weights_before_relu[j][2] = d(gen) * birds[1].weights_before_relu[j][2];
      birds[i].weights_after_relu[j] = d(gen) * birds[1].weights_after_relu[j];
    }
  }
}