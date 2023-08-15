#include "cell.hpp"
#include <random>

cell **init_board(const int x_size, const int y_size, const int mines_number) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> x_dist(0, x_size-1);
  std::uniform_int_distribution<> y_dist(0, y_size-1);

  auto board = new cell *[y_size];

  for (int i = 0; i < y_size; i++) {
    board[i] = new cell[x_size];
  }

  for (int y = 0; y < y_size; y++) {
    for (int x = 0; x < x_size; x++) {
      cell &c = board[y][x];
      c.x = x;
      c.y = y;
      c.mines_near = 0;
      c.is_mined = false;
      c.is_flagged = false;
      c.is_opened = false;
    }
  }

  for (int i = 0; i < mines_number; i++) {
    board[y_dist(gen)][x_dist(gen)].is_mined = true;
  }

  bool is_x_near_mines_filled{false};
  for (int y = 1; y < y_size - 1; y++) {
    for (int x = 1; x < x_size - 1; x++) {
      if (!board[y][x].is_mined) {
        for (int i = -1; i < 2; i++) {
          for (int j = -1; j < 2; j++) {
            if (board[y + i][x + j].is_mined)
              board[y][x].mines_near++;
          }
        }
      }
      if (!is_x_near_mines_filled) {
        if (!board[0][x].is_mined) {
          for (int i = -1; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
              if (board[j][x + i].is_mined)
                board[0][x].mines_near++;
            }
          }
        }
        if (!board[y_size - 1][x].is_mined) {
          for (int i = -1; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
              if (board[y_size - 1 - j][x + i].is_mined)
                board[y_size - 1][x].mines_near++;
            }
          }
        }
      }
    }
    is_x_near_mines_filled = true;
    if (!board[y][0].is_mined) {
      for (int i = -1; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
          if (board[y + i][j].is_mined)
            board[y][0].mines_near++;
        }
      }
    }
    if (!board[y][x_size - 1].is_mined) {
      for (int i = -1; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
          if (board[y + i][x_size - 1 - j].is_mined)
            board[y][x_size - 1].mines_near++;
        }
      }
    }
  }
  if (!board[0][0].is_mined) {
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        if (board[i][j].is_mined)
          board[0][0].mines_near++;
      }
    }
  }
  if (!board[0][x_size - 1].is_mined) {
    for (int i = 0; i < 2; i++) {
      for (int j = -1; j < 1; j++) {
        if (board[i][x_size + j - 1].is_mined)
          board[0][x_size - 1].mines_near++;
      }
    }
  }
  if (!board[y_size - 1][0].is_mined) {
    for (int i = -1; i < 1; i++) {
      for (int j = 0; j < 2; j++) {
        if (board[y_size + i - 1][j].is_mined)
          board[y_size - 1][0].mines_near++;
      }
    }
  }
  if (!board[y_size - 1][x_size - 1].is_mined) {
    for (int i = -1; i < 1; i++) {
      for (int j = -1; j < 1; j++) {
        if (board[y_size + i - 1][x_size + j - 1].is_mined)
          board[y_size - 1][x_size - 1].mines_near++;
      }
    }
  }
  return board;
}