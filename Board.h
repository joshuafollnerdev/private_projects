// Copyright 2018.
// Author: Joshua Follner <joshua.follner@gmx.de>

#ifndef BOARD_H_
#define BOARD_H_

#include <gtest/gtest.h>
#include <list>
#include <tuple>
#include <vector>

// Logic class of Hashiwokakero.
class Board {
 private:
  // Typedefs.
  typedef std::tuple<int, int, int> _islandTuple;
  typedef std::tuple<int, int, int, int, int> _bridgeTuple;
  typedef std::vector<_islandTuple> _islandType;
  typedef std::vector<_bridgeTuple> _bridgeType;
  typedef std::list<_bridgeType> _moveType;

  // Grid size.
  std::tuple<int, int> _gridSize;

  // Vector of all islands as tuples.
  _islandType _islands;

  // Vector of all bridges as tuples.
  _bridgeType _bridges;

  // List (stack) of moves for undo function.
  _moveType _moves;

  // The max Undo steps in series.
  int _maxUndo;

 public:
  // Constructor.
  Board();

  // Store next island in vector.
  void setIsland(int, int, int);

  // Store next bridge in vector.
  bool setBridge(int, int, int, int);

  // Get a bridge in the right order ((x1, y1) = left/upper island).
  std::tuple<int, int, int, int> getOrder(int, int, int, int) const;

  // Set the grid size.
  void setGridSize(std::tuple<int, int>);

  // Get the vector of all islands.
  _islandType* getIslands();

  // Get the vector of all bridges.
  _bridgeType* getBridges();

  // Get the grid size as a tuple.
  std::tuple<int, int> getGridSize() const;

  // Get one specific island. (nullptr when doesn´t exist).
  _islandTuple* getIsland(int, int);

  // Check if the given coordinates according to a bridge.
  _bridgeTuple* getBridge(int, int);

  // Get a specific bridge (nullptr when doesn´t exist).
  _bridgeTuple* getBridge(int, int, int, int);

  // Checks if a bridge can be build or has to be deletet.
  // Return: -1 = incorrect move, 0 = delete bridge,
  // 1 = add singel bridge, 2 = add double bridge.
  int checkBridge(int, int, int, int);

  // Check if vertical or horizontal. (0 = vertical, 1 = horizontal)
  int getOrientation(int, int, int, int) const;

  // Checks the weight of the island. ( 0 = free space, 1 = full,
  // 2 = too much bridges)
  int checkWeightIsland(int, int);

  // Sets the max undo steps in series.
  void setMaxUndo(int);

  // Returns the max undo steps in series.
  int getMaxUndo() const;

  // Returns the weight of a bridge.
  int getWeightBridge(int, int, int, int);

  // Returns the weight of a island.
  int getWeightIsland(_islandTuple);

  // Pushs a move to the move stack.
  void setMove();

  // Returns the last status of bridges.
  void undoMove();

  // Returns the number of islands.
  int getNumberOfIslands();

  FRIEND_TEST(Board, setIsland);
  FRIEND_TEST(Board, setGridSize);
  FRIEND_TEST(Board, testCheckWeightIsland);
};

#endif  // BOARD_H_
