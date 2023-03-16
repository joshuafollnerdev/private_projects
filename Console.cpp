// Copyright 2018.
// Author: Joshua Follner <joshua.follner@gmx.de>
// Used code from: https://daphne.informatik.uni-freiburg.de/
// ss2018/ProgrammierenCplusplus/svn/loesungen/blatt-04/GameOfLife.cpp

#include <math.h>
#include <ncurses.h>
#include <tuple>
#include <vector>
#include "./Board.h"
#include "./Console.h"

// ____________________________________________________________________________
Console::Console() {}

// ____________________________________________________________________________
Console::Console(Board* board) {
  _board = board;
  _lastClickedX = -1;
  _lastClickedY = -1;
  _gridSize = _board->getGridSize();
  _maxUndo = _board->getMaxUndo();
}

// ____________________________________________________________________________
void Console::initScreen() {
  initscr();
  start_color();
  cbreak();
  noecho();
  curs_set(false);
  nodelay(stdscr, true);
  keypad(stdscr, true);
  mousemask(ALL_MOUSE_EVENTS, NULL);
}

// ____________________________________________________________________________
int Console::processUserInput(int key) {
  MEVENT event;
  switch (key) {
    case 27:
      // Exit
      return -1;
      break;
    case 'u':
      // Undo
      return 2;
      break;
    case KEY_MOUSE:
      if (getmouse(&event) == OK) {
        if (event.bstate & BUTTON1_CLICKED) {
          // Store position of click.
          _lastClickedX = event.x;
          _lastClickedY = event.y;
          // mvprintw(5, 0, std::to_string(event.x).c_str());
          // mvprintw(5, 5, std::to_string(event.y).c_str());
          return 1;
        }
      }
      break;
  }
  return 0;
}

// ____________________________________________________________________________
void Console::drawIslands() {
  int x, y, count;
  std::vector<std::tuple<int, int, int>>* islands = _board->getIslands();
  for (auto& island : *islands) {
    x = std::get<0>(island);
    y = std::get<1>(island);
    count = std::get<2>(island);
    mvprintw((y * 3) + 2, (x * 3) + 2, std::to_string(count).c_str());
  }
  refresh();
}

// ____________________________________________________________________________
void Console::drawBridge(int x1, int y1, int x2, int y2, int count) {
  std::tuple<int, int, int, int> bridge = _board->getOrder(x1, y1, x2, y2);
  int xx1 = (std::get<0>(bridge) * 3) + 2;
  int yy1 = (std::get<1>(bridge) * 3) + 2;
  int xx2 = (std::get<2>(bridge) * 3) + 2;
  int yy2 = (std::get<3>(bridge) * 3) + 2;
  if (xx1 == xx2) {
    // Vertival.
    for (int i = yy1 + 1; i < yy2; i++) {
      if (count == 0) {
        mvprintw(i, xx1, " ");
      }
      if (count == 1) {
        mvprintw(i, xx1, "I");
      }
      if (count == 2) {
        mvprintw(i, xx1, "H");
      }
    }
  }
  if (yy1 == yy2) {
     // Horizontal.
    for (int i = xx1 + 1; i < xx2; i++) {
      if (count == 0) {
        mvprintw(yy1, i, " ");
      }
      if (count == 1) {
        mvprintw(yy1, i, "-");
      }
      if (count == 2) {
        mvprintw(yy1, i, "=");
      }
    }
  }
  // Check islands weight.
  int state1 = _board->checkWeightIsland(x1, y1);
  int state2 = _board->checkWeightIsland(x2, y2);
  std::tuple<int, int, int> isle1 = *(_board->getIsland(x1, y1));
  std::tuple<int, int, int> isle2 = *(_board->getIsland(x2, y2));
  int weight1 = std::get<2>(isle1);
  int weight2 = std::get<2>(isle2);
  if (state1 == 0) { drawIslandState(xx1, yy1, weight1, 0); }
  if (state2 == 0) { drawIslandState(xx2, yy2, weight2, 0); }
  if (state1 == 1) { drawIslandState(xx1, yy1, weight1, 1); }
  if (state2 == 1) { drawIslandState(xx2, yy2, weight2, 1); }
  if (state1 == 2) { drawIslandState(xx1, yy1, weight1, 2); }
  if (state2 == 2) { drawIslandState(xx2, yy2, weight2, 2); }
  refresh();
}

// ____________________________________________________________________________
void Console::drawIslandState(int x, int y, int count, int state) {
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  if (state == 0) {
    mvprintw(y, x, std::to_string(count).c_str());
  }
  if (state == 1) {
    attron(COLOR_PAIR(1));
    mvprintw(y, x, std::to_string(count).c_str());
    attroff(COLOR_PAIR(1));
  }
  if (state == 2) {
    attron(COLOR_PAIR(2));
    mvprintw(y, x, std::to_string(count).c_str());
    attroff(COLOR_PAIR(2));
  }
  refresh();
}

// ____________________________________________________________________________
bool Console::checkIfClickOnIsland(int x, int y) {
  x = computeBoardCoordinate(x);
  y = computeBoardCoordinate(y);
  if (_board->getIsland(x, y) != nullptr) {
    _cx = (x * 3) + 2;
    _cy = (y * 3) + 2;
    return true;
  }
  return false;
}

// ____________________________________________________________________________
void Console::playHashiwokakero() {
  initScreen();
  drawIslands();
  int key;
  int input;
  int first = 0;
  int second = 0;
  int x1, y1, x2, y2;
  while (true) {
    key = getch();
    input = processUserInput(key);
    refresh();
    if (input == -1) { break; }
    if (first == 0 && input == 1) {
      first = input;
      x1 = _lastClickedX;
      y1 = _lastClickedY;
      continue;
    }

    if (second == 0 && input == 1) {
      second = input;
      x2 = _lastClickedX;
      y2 = _lastClickedY;
    }
    // Undo.
    if (input == 2) {
      clear();
      _board->undoMove();
      drawIslands();
      drawAllBridges();
    }

    // Set the coordinates to the center of the island.
    bool check1 = false;
    bool check2 = false;
    if (checkIfClickOnIsland(x1, y1) == true) {
      x1 = _cx;
      y1 = _cy;
      check1 = true;
    }
    if (checkIfClickOnIsland(x2, y2) == true) {
      x2 = _cx;
      y2 = _cy;
      check2 = true;
    }

    if (first == 1 && second == 1) {
      if (check1 == true && check2 == true) {
        // Set the coordiantes to the real coordiantes of Board class.
        x1 = computeBoardCoordinate(x1);
        y1 = computeBoardCoordinate(y1);
        x2 = computeBoardCoordinate(x2);
        y2 = computeBoardCoordinate(y2);
        if (_board->setBridge(x1, y1, x2, y2) == true) {
          int weight = 0;
          if (_board->getBridge(x1, y1, x2, y2) != nullptr) {
            weight = _board->getWeightBridge(x1, y1, x2, y2);
          }
          drawBridge(x1, y1, x2, y2, weight);
        }
      }
      first = 0;
      second = 0;
    }
  }
  endwin();
}

// ____________________________________________________________________________
int Console::computeBoardCoordinate(int x) {
  float fx = static_cast<float>(x);
  float erg = round((fx - 2) / 3);
  return static_cast<int>(erg);
}

// ____________________________________________________________________________
void Console::drawAllBridges() {
  std::vector<std::tuple<int, int, int, int, int>> bridges;
  bridges = *(_board->getBridges());
  for (auto& bridge : bridges) {
    int x1 = std::get<0>(bridge);
    int y1 = std::get<1>(bridge);
    int x2 = std::get<2>(bridge);
    int y2 = std::get<3>(bridge);
    int count = std::get<4>(bridge);
    drawBridge(x1, y1, x2, y2, count);
  }
}
