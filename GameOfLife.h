// Copyright 2018, Joshua Follner
// Author: Joshua Follner <joshua.follner@gmx.de>
//
// Used code from Musterloesung.

#ifndef GAMEOFLIFE_H_
#define GAMEOFLIFE_H_

#include <gtest/gtest.h>

// A class to play the game of life.
class GameOfLife {
 public:
  // Constructor.
  GameOfLife();
  // Destructor
  ~GameOfLife();
  // The game of life itself.
  void game();

 private:
  static const int _MAX_GRID_SIZE = 1000;
  bool _grid1[_MAX_GRID_SIZE][_MAX_GRID_SIZE];
  bool _grid2[_MAX_GRID_SIZE][_MAX_GRID_SIZE];
  bool (*_oldGrid)[_MAX_GRID_SIZE];
  bool (*_newGrid)[_MAX_GRID_SIZE];
  int _lastClickedX;
  int _lastClickedY;
  int _livingCells;
  bool _running;
  int _steps;
  int _xSize;
  int _ySize;
  int _drawMinX;
  int _drawMaxX;
  int _drawMinY;
  int _drawMaxY;
  int _glider[5][2] = { {1, 0}, {2, 1}, {0, 2}, {1, 2}, {2, 2} };
  int _factory[36][2] = {
  {24, 0},
  {22, 1}, {24, 1},
  {12, 2}, {13, 2}, {20, 2}, {21, 2}, {34, 2}, {35, 2},
  {11, 3}, {15, 3}, {20, 3}, {21, 3}, {34, 3}, {35, 3},
  {0, 4}, {1, 4}, {10, 4}, {16, 4}, {20, 4}, {21, 4},
  {0, 5}, {1, 5}, {10, 5}, {14, 5}, {16, 5}, {17, 5}, {22, 5}, {24, 5},
  {10, 6}, {16, 6}, {24, 6},
  {11, 7}, {15, 7},
  {12, 8}, {13, 8}
  };


  // Initialize the terminal
  void initializeGame();

  // Handle key and mouse presses. Change the new grid.
  int processUserInput(int key);
  FRIEND_TEST(GameOfLife, processUserInputEsc);
  FRIEND_TEST(GameOfLife, processUserInputSpace);
  FRIEND_TEST(GameOfLife, processUserInputSmallS);
  FRIEND_TEST(GameOfLife, processUserInputSmallG);
  FRIEND_TEST(GameOfLife, processUserInputBigG);
  FRIEND_TEST(GameOfLife, processUserInputSmallR);

  // Calculate the next state and write it into new grid.
  void updateState();
  FRIEND_TEST(GameOfLife, updateStateEmptyGrid);
  FRIEND_TEST(GameOfLife, updateStateStaticObject);
  FRIEND_TEST(GameOfLife, updateStateP2Blinker);

  // Count the number of alive cells around given coordinates in the old grid.
  int aliveNeighbours(int x, int y);
  FRIEND_TEST(GameOfLife, aliveNeighboursSingle);
  FRIEND_TEST(GameOfLife, aliveNeighboursMultiple);

  // Draw the new grid using ncurses.
  void showState();
};

#endif  // GAMEOFLIFE_H_
