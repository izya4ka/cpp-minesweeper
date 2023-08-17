#include "cell.hpp"
#include "cursor.hpp"
#include "difficulty.hpp"
#include "functions.hpp"
#include <array>
#include <functional>
#include <ncurses.h>

const std::array<difficulty, 3> difficulty_array{
    {{10, 10, 15}, {15, 15, 30}, {20, 20, 60}},
};

const std::array<const char[7], 3> difficulty_names{"EASY", "NORMAL", "HARD"};

int main() {

  initscr();
  start_color();
  keypad(stdscr, true);
  set_escdelay(0);
  noecho();
  curs_set(0);

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_CYAN);
  init_pair(3, COLOR_RED, COLOR_BLACK);

  int difficulty_choice{};

  printw("Choose difficulty:\n");

  colored_output([] { printw("%s\n", difficulty_names[0]); }, 2);
  printw("%s\n", difficulty_names[1]);
  printw("%s\n", difficulty_names[2]);

  bool is_difficulty_chosen{};

  while (true) {
    switch (getch()) {
    case KEY_UP:
      difficulty_choice--;
      break;
    case KEY_DOWN:
      difficulty_choice++;
      break;
    case 10:
      is_difficulty_chosen = true;
      break;
    }
    if (is_difficulty_chosen)
      break;
    if (difficulty_choice > 2)
      difficulty_choice = 0;
    else if (difficulty_choice < 0)
      difficulty_choice = 2;

    move(1, 0);
    for (int i = 0; i < 3; i++) {
      if (i == difficulty_choice) {
        attrset(COLOR_PAIR(2));
      }
      printw("%s\n", difficulty_names[i]);
      attrset(COLOR_PAIR(1));
    }
  }

  clear();
  refresh();

  const auto &x_size{difficulty_array[difficulty_choice].columns},
      &y_size{difficulty_array[difficulty_choice].rows},
      &mines_number{difficulty_array[difficulty_choice].mines_count};

  const int open_cells_to_win{x_size * y_size - mines_number};

  auto board = init_board(x_size, y_size, mines_number);

  for (int y = 0; y < y_size; y++) {
    for (int x = 0; x < x_size * 2 - 1; x += 2) {
      mvaddch(y, x, '#');
    }
  }

  cursor cur{0, 0, 0, 0};
  bool stop{false}, losed{false}, won{false};
  int opened_cells{0};
  while (true) {
    bool to_open{false}, to_flag{false};

    if (opened_cells == open_cells_to_win)
      won = true;

    if (stop || losed || won)
      break;

    switch (getch()) {
    case KEY_UP:
      cur.prev_y = cur.y;
      cur.prev_x = cur.x;
      cur.y--;
      if (cur.y < 0)
        cur.y = y_size - 1;
      break;
    case KEY_DOWN:
      cur.prev_y = cur.y;
      cur.prev_x = cur.x;
      cur.y++;
      if (cur.y > (y_size - 1))
        cur.y = 0;
      break;
    case KEY_LEFT:
      cur.prev_x = cur.x;
      cur.prev_y = cur.y;
      cur.x--;
      if (cur.x < 0)
        cur.x = x_size - 1;
      break;
    case KEY_RIGHT:
      cur.prev_x = cur.x;
      cur.prev_y = cur.y;
      cur.x++;
      if (cur.x > (x_size - 1))
        cur.x = 0;
      break;
    case 'f':
    case 'F':
      to_flag = true;
      break;
    case 27: // ESC
      stop = true;
      break;
    case 10: // Enter
      to_open = true;
      break;
    }

    auto previousCell = board[cur.prev_y][cur.prev_x];

    if (previousCell.is_opened)
      mvaddch(cur.prev_y, cur.prev_x * 2,
              static_cast<char>(previousCell.mines_near + '0'));
    else if (previousCell.is_flagged) {
      colored_output([cur]() { mvaddch(cur.prev_y, cur.prev_x * 2, 'F'); }, 3);
    } else
      mvaddch(cur.prev_y, cur.prev_x * 2, '#');

    auto &selected_cell = board[cur.y][cur.x];

    attrset(COLOR_PAIR(2));
    if (selected_cell.is_flagged)
      mvaddch(cur.y, cur.x * 2, 'F');
    else if (selected_cell.is_opened)
      mvaddch(cur.y, cur.x * 2, static_cast<char>(selected_cell.mines_near + '0'));
    else
      mvaddch(cur.y, cur.x * 2, '#');

    if (to_flag && !selected_cell.is_opened) {
      selected_cell.is_flagged = !selected_cell.is_flagged;
      if (selected_cell.is_flagged) {
        colored_output([cur] { mvaddch(cur.y, cur.x * 2, 'F'); }, 2);
      }
    }

    if (to_open && !selected_cell.is_flagged) {
      if (!selected_cell.is_opened)
        opened_cells++;
      selected_cell.is_opened = true;
      if (selected_cell.is_mined) {
        losed = true;
        continue;
      }
      mvaddch(cur.y, cur.x * 2, static_cast<char>(selected_cell.mines_near + '0'));
    }
    attrset(COLOR_PAIR(1));
  }

  attrset(COLOR_PAIR(1));

  for (int y = 0; y < y_size; y++) {
    for (int x = 0; x < x_size * 2 - 1; x += 2) {
      cell current_cell = board[y][x / 2];
      if (current_cell.is_mined) {
        colored_output([=] { mvaddch(y, x, 'M'); }, 3);
      } else
        mvaddch(y, x, static_cast<char>(current_cell.mines_near + '0'));
    }
  }

  if (won)
    printw("\nYou won!");
  else if (losed)
    printw("\nYou losed!");
  else
    printw("\nYou gave up!");
  getch();

  endwin();
}
