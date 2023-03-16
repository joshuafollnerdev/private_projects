// Copyright 2018.
// Auhtor: Joshua Follner <joshua.follner@gmx.de>

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <list>
#include <tuple>
#include "./Board.h"
#include "./FileHandler.h"

class Console {
 private:
  // Board Pointer for Board access.
  Board* _board;

  // GridSize.
  std::tuple<int, int> _gridSize;

  // The max undo steps in series.
  int _maxUndo;

  // Last coordinates of mouse click.
  int _lastClickedX;
  int _lastClickedY;

  // Center coordinates.
  int _cx;
  int _cy;

  // Initialize the screen.
  void initScreen();

  // Processes the user input.
  int processUserInput(int);

  // Draws the islands.
  void drawIslands();

  // Draws a bridge or delete it.
  // Also checks the weight of the islands.
  void drawBridge(int, int, int, int, int);

  // Marks a island green when there are enough bridges or marks it red
  // when there are too much bridges.
  // (4rd argument -> 0 = nothing, 1 = green, 2 = red)
  void drawIslandState(int, int, int, int);

  // Gets the bridges from Board and print them. (for undo and solution)
  void drawAllBridges();

 public:
  // Standard Constructor.
  Console();

  // Constructor with argument.
  explicit Console(Board*);

  // Checks if user clicked on a island.
  bool checkIfClickOnIsland(int, int);

  // Function that processes the game.
  void playHashiwokakero();

  // Computes the board coordinate of a number.
  int computeBoardCoordinate(int);
};

#endif  // CONSOLE_H_
