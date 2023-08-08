#include "mine.hpp"
#include <random>

cell **initBoard(const int xSize, const int ySize, const int minesNumber) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> xDist(0, xSize-1);
  std::uniform_int_distribution<> yDist(0, ySize-1);

  auto board = new cell *[ySize];

  for (int i = 0; i < ySize; i++) {
    board[i] = new cell[xSize];
  }

  for (int y = 0; y < ySize; y++) {
    for (int x = 0; x < xSize; x++) {
      cell &c = board[y][x];
      c.x = x;
      c.y = y;
      c.minesNear = 0;
      c.isMined = false;
      c.isFlaged = false;
      c.isOpened = false;
    }
  }

  for (int i = 0; i < minesNumber; i++) {
    board[yDist(gen)][xDist(gen)].isMined = true;
  }

  bool isXNearMinesFilled{false};
  for (int y = 1; y < ySize - 1; y++) {
    for (int x = 1; x < xSize - 1; x++) {
      if (!board[y][x].isMined) {
        for (int i = -1; i < 2; i++) {
          for (int j = -1; j < 2; j++) {
            if (board[y + i][x + j].isMined)
              board[y][x].minesNear++;
          }
        }
      }
      if (!isXNearMinesFilled) {
        if (!board[0][x].isMined) {
          for (int i = -1; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
              if (board[j][x + i].isMined)
                board[0][x].minesNear++;
            }
          }
        }
        if (!board[ySize - 1][x].isMined) {
          for (int i = -1; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
              if (board[ySize - 1 - j][x + i].isMined)
                board[ySize - 1][x].minesNear++;
            }
          }
        }
      }
    }
    isXNearMinesFilled = true;
    if (!board[y][0].isMined) {
      for (int i = -1; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
          if (board[y + i][j].isMined)
            board[y][0].minesNear++;
        }
      }
    }
    if (!board[y][xSize - 1].isMined) {
      for (int i = -1; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
          if (board[y + i][xSize - 1 - j].isMined)
            board[y][xSize - 1].minesNear++;
        }
      }
    }
  }
  if (!board[0][0].isMined) {
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        if (board[i][j].isMined)
          board[0][0].minesNear++;
      }
    }
  }
  if (!board[0][xSize - 1].isMined) {
    for (int i = 0; i < 2; i++) {
      for (int j = -1; j < 1; j++) {
        if (board[i][xSize + j - 1].isMined)
          board[0][xSize - 1].minesNear++;
      }
    }
  }
  if (!board[ySize - 1][0].isMined) {
    for (int i = -1; i < 1; i++) {
      for (int j = 0; j < 2; j++) {
        if (board[ySize + i - 1][j].isMined)
          board[ySize - 1][0].minesNear++;
      }
    }
  }
  if (!board[ySize - 1][xSize - 1].isMined) {
    for (int i = -1; i < 1; i++) {
      for (int j = -1; j < 1; j++) {
        if (board[ySize + i - 1][xSize + j - 1].isMined)
          board[ySize - 1][xSize - 1].minesNear++;
      }
    }
  }
  return board;
}