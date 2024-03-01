#include "../include/Obstacle.h"
Obstacle::Obstacle() {
  bottom.w = 40;
  top.w = 40;
  bottom.x = SCREEN_WIDTH - bottom.w;
  top.x = SCREEN_WIDTH - top.w;
  bottom.y = SCREEN_HEIGHT - 100;
  top.y = 0;
  bottom.h = 100;
  top.h = 100;
  x_vel = 1;
  counted = false;
  center = (bottom.y + top.h) / 2;
  r = 0;
  g = 0;
  b = 0;
}

Obstacle::Obstacle(int r_bot_h, int r_top_h, int top_x, int bot_x) {
  bottom.w = 40;
  top.w = 40;
  bottom.x = bot_x;
  top.x = top_x;
  bottom.y = SCREEN_HEIGHT - r_bot_h;
  top.y = 0;
  bottom.h = r_bot_h;
  top.h = r_top_h;
  x_vel = 1;
  counted = false;
  center = (bottom.y + top.h) / 2;
  r = 0;
  g = 0;
  b = 0;
}

void Obstacle::move() {
  top.x -= x_vel;
  bottom.x -= x_vel;
}

void Obstacle::reset() {
  if (top.x + top.w < 0) {
    int height;              // rand a top of the hole in the wall
    std::random_device rd;   // a seed source for the random number engine
    std::mt19937 gen(rd());  // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(100, 500);
    std::uniform_int_distribution<> color(0, 255);
    height = distrib(gen);
    top.x = SCREEN_WIDTH;
    bottom.x = SCREEN_WIDTH;
    top.h = height;
    bottom.h = SCREEN_HEIGHT - top.h - 168;  // 168 - height of the wall
    bottom.y = SCREEN_HEIGHT - bottom.h;
    counted = false;
    center = (bottom.y + top.h) / 2;
    r = color(gen);
    g = color(gen);
    b = color(gen);
  }
}

void Obstacle::render() {
  SDL_SetRenderDrawColor(gRenderer, r, g, b, 0xFF);
  SDL_RenderFillRect(gRenderer, &bottom);
  SDL_RenderFillRect(gRenderer, &top);
}

int Obstacle::GetVel() {
  return x_vel;
}