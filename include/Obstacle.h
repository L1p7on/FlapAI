#include <SDL2/SDL.h>
#include <random>
#ifndef OBSTACLE_H_
#define OBSTACLE_H_
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern SDL_Renderer* gRenderer;
class Obstacle {
 public:
  SDL_Rect bottom;
  SDL_Rect top;
  Obstacle();
  Obstacle(int bot_h, int top_h, int top_x, int bot_x);
  void render();
  void reset();
  void move();
  int GetVel();
  bool counted;
  int center;

 private:
  int x_vel;
  int y_vel;
  int r, g, b;
};
#endif  // OBSTACLE_H_