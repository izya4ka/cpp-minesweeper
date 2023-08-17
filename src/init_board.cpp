#include "cell.hpp"
#include <random>
#include <stdexcept>
#include <vector>

using std::vector;

vector<vector<cell>> init_board(const int x_size, const int y_size,
                                const int mines_number) {

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> x_dist(0, x_size - 1);
  std::uniform_int_distribution<> y_dist(0, y_size - 1);

  vector<vector<cell>> board{};

  for (int y = 0; y < y_size; y++) {
    vector<cell> row{};
    for (int x = 0; x < x_size; x++) {
      cell c{x, y, 0, false, false, false};
      row.push_back(c);
    }
    board.push_back(row);
  }

  for (int i = 0; i < mines_number; i++) {
    board[y_dist(gen)][x_dist(gen)].is_mined = true;
  }

  for (int y = 0; y < y_size; y++) {
    for (int x = 0; x < x_size; x++) {
      cell &c{board[y][x]};
      if (!c.is_mined) {
        for (int y_to_check = -1; y_to_check < 2; y_to_check++) {
          for (int x_to_check = -1; x_to_check < 2; x_to_check++) {
            try {
              if (board.at(y + y_to_check).at(x + x_to_check).is_mined)
                c.mines_near++;
            } catch (std::out_of_range) {
            };
          }
        }
      }
    }
  }
  return board;
}