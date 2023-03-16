// Copyright 2018.
// Auhtor: Joshua Follner <joshua.follner@gmx.de>

#ifndef FILEHANDLER_H_
#define FILEHANDLER_H_

#include <gtest/gtest.h>
#include <string>
#include "./Board.h"

// Class that interpretes the command line arguments and choose the right way to
// open and read a given file.
class FileHandler {
 private:
  // Board Pointer for Board access.
  Board* _board;

  // String for /path/filename.
  std::string _filename;
  std::string _filenameSolution;

  // The max undo steps in series.
  int _maxUndo;

  // Open .xy file read it line by line and set the islands on the Board class.
  void openXY();

  // Open .plain file read it line by line and set the islands
  // on the Board class.
  void openPlain();

  // Open xy file and solution file and set islands and bridges.
  void openSolution();

 public:
  // Constructor.
  FileHandler();

  // Destructor.
  ~FileHandler();

  // Checks the command line arguments and finds out which open fucntion has
  // to be called.
  void handle(int argc, char** argv);

  // Returns the Board object.
  Board* getBoard();

  // Prints if the given solution is no solution of this instance.
  void printIfWrongSolution();

  // Prints the Usage.
  void printUsageAndExit();

  FRIEND_TEST(FileHandler, xyTest);
  FRIEND_TEST(FileHandler, plainTest);
  FRIEND_TEST(FileHandler, handlerTestMaxUndoNoInteger);
  FRIEND_TEST(FileHandler, solutionTestOk);
  FRIEND_TEST(Console, checkIfClickOnIsland);
};

#endif  // FILEHANDLER_H_
