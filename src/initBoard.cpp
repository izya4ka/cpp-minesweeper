#include "mine.hpp"
#include <cstddef>
#include <cstdlib>
#include <ctime>


cell **initBoard(const int x_size, const int y_size,
                 const int mines_number) {
  srand(time(NULL));

  auto board = new cell *[y_size];

  for (int i = 0; i < y_size; i++) {
    board[i] = new cell[x_size];
  }

  for (int y = 0; y < y_size; y++) {
    for (int x = 0; x < x_size; x++) {
      cell &c = board[y][x];
      c.x = x;
      c.y = y;
      c.minesNear = 0;
      c.isMined = false;
      c.isFlaged = false;
      c.isOpened = false;
    }
  }

  for (int i = 0; i < mines_number; i++) {
    board[rand() % y_size][rand() % x_size].isMined = true;
  }

  bool isXNearMinesFilled{false};
  for (int y = 1; y < y_size - 1; y++) {
    for (int x = 1; x < x_size - 1; x++) {
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
        if (!board[y_size - 1][x].isMined) {
          for (int i = -1; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
              if (board[y_size - 1 - j][x + i].isMined)
                board[y_size - 1][x].minesNear++;
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
    if (!board[y][x_size - 1].isMined) {
      for (int i = -1; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
          if (board[y + i][x_size - 1 - j].isMined)
            board[y][x_size - 1].minesNear++;
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
  if (!board[0][x_size - 1].isMined) {
    for (int i = 0; i < 2; i++) {
      for (int j = -1; j < 1; j++) {
        if (board[i][x_size + j - 1].isMined)
          board[0][x_size - 1].minesNear++;
      }
    }
  }
  if (!board[y_size - 1][0].isMined) {
    for (int i = -1; i < 1; i++) {
      for (int j = 0; j < 2; j++) {
        if (board[y_size + i - 1][j].isMined)
          board[y_size - 1][0].minesNear++;
      }
    }
  }
  if (!board[y_size - 1][x_size - 1].isMined) {
    for (int i = -1; i < 1; i++) {
      for (int j = -1; j < 1; j++) {
        if (board[y_size + i - 1][x_size + j - 1].isMined)
          board[y_size - 1][x_size - 1].minesNear++;
      }
    }
  }
  return board;
}