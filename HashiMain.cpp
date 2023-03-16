// Copyright 2018.
// Auhtor: Joshua Follner <joshua.follner@gmx.de>

#include "./Console.h"
#include "./FileHandler.h"
#include "./Board.h"


int main(int argc, char** argv) {
  FileHandler handler;

  handler.handle(argc, argv);
  Console console(handler.getBoard());

  console.playHashiwokakero();

  return 0;
}
