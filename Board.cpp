// Copyright 2018.
// Author: Joshua Follner <joshua.follner@gmx.de>

#include <tuple>
#include <list>
#include "./Board.h"

// ____________________________________________________________________________
Board::Board() {
  _maxUndo = 5;
}

// ____________________________________________________________________________
int Board::checkBridge(int x1, int y1, int x2, int y2) {
  _bridgeTuple* bridge = getBridge(x1, y1, x2, y2);
  _islandTuple* island1 = getIsland(x1, y1);
  _islandTuple* island2 = getIsland(x2, y2);
  // If one or both given coordinates are no islands. Incorrect move.
  if (island1 == nullptr || island2 == nullptr) { return -1; }
  int wIsland1 = getWeightIsland(*island1);
  int wIsland2 = getWeightIsland(*island2);
  // If the bridge exists.
  if (bridge != nullptr) {
    int wBridge = std::get<4>(*bridge);
    // If bridge weight is 1 and a second bridge can be add.
    if (wBridge == 1 && wIsland1 > 0 && wIsland2 > 0) {
      return 2;  // Add second bridge.
    } else { return 0; }  // Else delete bridge
  }
  // If one or both islands has not enough weight. Incorrect move.
  if (wIsland1 == 0 || wIsland2 == 0) { return -1; }
  // If vertical.
  if (getOrientation(x1, y1, x2, y2) == 0) {
    int x = x1;
    int startY = y1 + 1;
    int endY = y2 - 1;
    // If the lower island was klicked first.
    if (y2 < y1) {
      startY = y2 + 1;
      endY = y1 - 1;
    }
    // If there crosses a bridge. Incorrect move.
    for (int y = startY; y <= endY; y++) {
      if (getIsland(x, y) != nullptr) { return -1; }
      if (getBridge(x, y) != nullptr) { return -1; }
    }
    return 1;  // If vertical and correct move. Add this single bridge.
  } else if (getOrientation(x1, y1, x2, y2) == 1) {
    // If horizontal.
    int y = y1;
    int startX = x1 + 1;
    int endX = x2 - 1;
    // If the right island was klicked first.
    if (x2 < x1) {
      startX = x2 + 1;
      endX = x1 - 1;
    }
    // If there crosses a bridge. Incorrect move.
    for (int x = startX; x <= endX; x++) {
      if (getIsland(x, y) != nullptr) { return -1; }
      if (getBridge(x, y) != nullptr) { return -1; }
    }
    return 1;  // If horizontal and correct move. Add this single bridge.
  } else { return -1; }
}

// ____________________________________________________________________________
int Board::getOrientation(int x1, int y1, int x2, int y2) const {
  if (x1 == x2 && y1 != y2) { return 0; }
  if (y1 == y2 && x1 != x2) { return 1; }
  return -1;
}

// ____________________________________________________________________________
bool Board::setBridge(int x1, int y1, int x2, int y2) {
  auto bridge = getBridge(x1, y1, x2, y2);
  int check = checkBridge(x1, y1, x2, y2);
  if (check != -1) { setMove(); }
  if (check == 2) {
    std::get<4>(*bridge) = 2;
  } else if (check == 1) {
    if (bridge == nullptr) {
      auto order = getOrder(x1, y1, x2, y2);
      _bridgeTuple b = std::make_tuple(
        std::get<0>(order),
        std::get<1>(order),
        std::get<2>(order),
        std::get<3>(order),
        1);
      _bridges.push_back(b);
    }
  } else if (check == 0) {
    for (int i = 0; i < static_cast<int>(_bridges.size()); i++) {
      if (_bridges.at(i) == *bridge) {
        _bridges.erase(_bridges.begin() + i);
      }
    }
  } else { return false; }
  return true;
}

// ____________________________________________________________________________
std::tuple<int, int, int, int> Board::getOrder(int x1, int y1,
    int x2, int y2) const {
  if (getOrientation(x1, y1, x2, y2) == 0) {
    if (y2 < y1) {
      return std::make_tuple(x2, y2, x1, y1);
    } else {
      return std::make_tuple(x1, y1, x2, y2);
    }
  } else if (getOrientation(x1, y1, x2, y2) == 1) {
    if (x2 < x1) {
      return std::make_tuple(x2, y2, x1, y1);
    } else {
      return std::make_tuple(x1, y1, x2, y2);
    }
  } else { return std::make_tuple(x1, y1, x2, y2); }
}

// _____________________________________________________________________________
Board::_bridgeTuple* Board::getBridge(int x, int y) {
  for (auto& bridge : _bridges) {
    int x1 = std::get<0>(bridge);
    int y1 = std::get<1>(bridge);
    int x2 = std::get<2>(bridge);
    int y2 = std::get<3>(bridge);
    int orientation = getOrientation(x1, y1, x2, y2);
    if (orientation == 0) {
      for (int i = y1 + 1; i < y2; i++) {
        if (x1 == x && y == i) { return &bridge; }
      }
    } else if (orientation == 1) {
      for (int i = x1 + 1; i < x2; i++) {
        if (y1 == y && x == i) { return &bridge; }
      }
    }
  }
  return nullptr;
}

// _____________________________________________________________________________
Board::_bridgeTuple* Board::getBridge(int x1, int y1, int x2, int y2) {
  for (auto& bridge : _bridges) {
    int bx1 = std::get<0>(bridge);
    int by1 = std::get<1>(bridge);
    int bx2 = std::get<2>(bridge);
    int by2 = std::get<3>(bridge);
    if (bx1 == x1 && by1 == y1 && bx2 == x2 && by2 == y2) { return &bridge; }
    if (bx2 == x1 && by2 == y1 && bx1 == x2 && by1 == y2) { return &bridge; }
  }
  return nullptr;
}

// _____________________________________________________________________________
Board::_islandTuple* Board::getIsland(int x, int y) {
  for (auto& island : _islands) {
    int ix = std::get<0>(island);
    int iy = std::get<1>(island);
    if (x == ix && y == iy) { return &island; }
  }
  return nullptr;
}

// _____________________________________________________________________________
std::tuple<int, int> Board::getGridSize() const {
  return _gridSize;
}

// _____________________________________________________________________________
void Board::setGridSize(std::tuple<int, int> gridSize) {
  _gridSize = gridSize;
}

// _____________________________________________________________________________
Board::_islandType* Board::getIslands() {
  return &_islands;
}

// _____________________________________________________________________________
Board::_bridgeType* Board::getBridges() {
  return &_bridges;
}

// _____________________________________________________________________________
void Board::setIsland(int x, int y, int count) {
  _islandTuple island = std::make_tuple(x, y, count);
  _islands.push_back(island);
}

// _____________________________________________________________________________
int Board::checkWeightIsland(int x, int y) {
  int actWeight = getWeightIsland(*(getIsland(x, y)));
  if (actWeight == 0) {
    return 1;
  }
  if (actWeight > 0) {
    return 0;
  } else {
    return 2;
  }
}

// _____________________________________________________________________________
void Board::setMaxUndo(int maxUndo) {
  _maxUndo = maxUndo;
}

// _____________________________________________________________________________
int Board::getMaxUndo() const {
  return _maxUndo;
}

// _____________________________________________________________________________
int Board::getWeightBridge(int x1, int y1, int x2, int y2) {
  _bridgeTuple bridge = *(getBridge(x1, y1, x2, y2));
  int weight = std::get<4>(bridge);
  return weight;
}

// ____________________________________________________________________________
int Board::getWeightIsland(_islandTuple island) {
  int x = std::get<0>(island);
  int y = std::get<1>(island);
  int count = std::get<2>(island);
  for (auto& bridge : _bridges) {
    int x1 = std::get<0>(bridge);
    int y1 = std::get<1>(bridge);
    int x2 = std::get<2>(bridge);
    int y2 = std::get<3>(bridge);
    int w = std::get<4>(bridge);
    if ((x1 == x && y1 == y) || (x2 == x && y2 == y)) { count -= w; }
  }
  return count;
}

// ____________________________________________________________________________
void Board::setMove() {
  if (static_cast<int>(_moves.size()) < _maxUndo) {
    _moves.push_back(_bridges);
  }
  if (static_cast<int>(_moves.size()) == _maxUndo) {
    _moves.pop_front();
    _moves.push_back(_bridges);
  }
}

// ____________________________________________________________________________
void Board::undoMove() {
  if (_moves.empty() == true) { return; }
  _bridges = _moves.back();
  _moves.pop_back();
}

// ____________________________________________________________________________
int Board::getNumberOfIslands() {
  int size = static_cast<int>(_islands.size());
  return size;
}
