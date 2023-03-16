// Copyright 2018.
// Auhtor: Joshua Follner <joshua.follner@gmx.de>

#include <ctype.h>
#include <set>
#include <vector>
#include <string>
#include <tuple>
#include <fstream>
#include <iostream>
#include "./Board.h"
#include "./FileHandler.h"

// _____________________________________________________________________________
FileHandler::FileHandler() {
  _board = new Board;
  _filename = "";
  _filenameSolution = "";
  _maxUndo = 5;
}

// _____________________________________________________________________________
FileHandler::~FileHandler() {
  delete _board;
}

// _____________________________________________________________________________
void FileHandler::handle(int argc, char** argv) {
  // Check command line arguments.
  if (argc < 2) {
    printUsageAndExit();
  }
  if (argc == 2) {
    _filename = argv[1];
  }
  if (argc == 3) {
    _filename = argv[1];
    _maxUndo = atoi(argv[2]);
    if (_maxUndo == 0) {
      _maxUndo = 5;
      _filenameSolution = argv[2];
    }
  }
  if (argc == 4) {
    _filename = argv[1];
    _filenameSolution = argv[2];
    _maxUndo = atoi(argv[3]);
    if (_maxUndo == 0) {
      printUsageAndExit();
    }
  }
  _board->setMaxUndo(_maxUndo);
  // Find out the filetype.
  if (_filenameSolution.find("xy.solution") != std::string::npos) {
    openSolution();
  } else if (_filename.find(".xy") != std::string::npos) {
    openXY();
  } else if (_filename.find(".plain") != std::string::npos) {
    openPlain();
  } else {
    printUsageAndExit();
  }
}

// _____________________________________________________________________________
void FileHandler::openXY() {
  std::ifstream file(_filename.c_str());
  if (!file) {
    printUsageAndExit();
  }
  std::string line;
  int lineCount = 1;
  while (true) {
    std::getline(file, line);
    if (file.eof()) { break; }
    if (lineCount == 1) {
      std::size_t pos1 = line.find(" ");
      std::size_t pos2 = line.find(":");
      std::size_t pos3 = line.find(" ", pos2);
      int xGrid = std::stoi(line.substr(pos1 + 1, pos2 - 1));
      int yGrid = std::stoi(line.substr(pos2 + 1, pos3 - 1));
      std::tuple<int, int> gridSize(xGrid, yGrid);
      _board->setGridSize(gridSize);
    }
    if (line.length() > 0 && line[0] != '#') {
      std::size_t pos1 = line.find(",");
      std::size_t pos2 = line.find(",", pos1 + 1);
      int x = std::stoi(line.substr(0, pos1).c_str());
      int y = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
      int count = std::stoi(line.substr(pos2 + 1).c_str());
      _board->setIsland(x, y, count);
    }
    lineCount += 1;
  }
  file.close();
}

// _____________________________________________________________________________
void FileHandler::openPlain() {
  std::ifstream file(_filename.c_str());
  if (!file) {
    printUsageAndExit();
  }
  std::string line;
  int lineCount = 1;
  while (true) {
    std::getline(file, line);
    if (file.eof()) { break; }
    if (lineCount == 1) {
      std::size_t pos1 = line.find(" ");
      std::size_t pos2 = line.find(":");
      std::size_t pos3 = line.find(" ", pos2);
      int xGrid = std::stoi(line.substr(pos1 + 1, pos2 - 1));
      int yGrid = std::stoi(line.substr(pos2 + 1, pos3 - 1));
      std::tuple<int, int> gridSize(xGrid, yGrid);
      _board->setGridSize(gridSize);
    } else if (line.length() > 0 && line[0] != '#') {
      for (int i = 0; i < static_cast<int>(line.length()); i++) {
        int x;
        int count;
        if (line[i] != ' ') {
          x = i;
          count = std::stoi(line.substr(i, 1).c_str());
          int y = lineCount - 2;
          _board->setIsland(x, y, count);
        }
      }
    } else { continue; }
    lineCount += 1;
  }
  file.close();
}

// _____________________________________________________________________________
void FileHandler::openSolution() {
  openXY();
  std::ifstream file(_filenameSolution.c_str());
  if (!file) {
    printUsageAndExit();
  }
  std::set<std::tuple<int, int>> solveIslands;
  std::string line;
  int lineCount = 1;
  while (true) {
    std::getline(file, line);
    if (file.eof()) { break; }
    if (line.length() > 0 && line[0] != '#') {
      int x1 = std::stoi(line.substr(0, 1).c_str());
      int y1 = std::stoi(line.substr(2, 1).c_str());
      int x2 = std::stoi(line.substr(4, 1).c_str());
      int y2 = std::stoi(line.substr(6, 1).c_str());
      if (_board->setBridge(x1, y1, x2, y2) == false) {
        printIfWrongSolution();
      }

      // Now check if the islands from xy and solution compare.
      std::vector<std::tuple<int, int, int>> islands = *(_board->getIslands());
      for (auto& island : islands) {
        if (std::get<0>(island) == x1 && std::get<1>(island) == y1) {
          solveIslands.insert(std::make_tuple(x1, y1));
        }
        if (std::get<0>(island) == x2 && std::get<1>(island) == y2) {
          solveIslands.insert(std::make_tuple(x2, y2));
        }
      }
    }
    lineCount += 1;
  }

  if (static_cast<int>(solveIslands.size()) != _board->getNumberOfIslands()) {
    printIfWrongSolution();
  }
  file.close();
}

// _____________________________________________________________________________
void FileHandler::printUsageAndExit() {
  std::cerr << "WRONG USAGE!" << std::endl;
  exit(1);
}

// _____________________________________________________________________________
void FileHandler::printIfWrongSolution() {
  std::cerr << "WRONG SOLUTION!" << std::endl;
  exit(1);
}

// _____________________________________________________________________________
Board* FileHandler::getBoard() {
  return _board;
}
