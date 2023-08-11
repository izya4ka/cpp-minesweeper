#include "cursor.hpp"
#include "difficulty.hpp"
#include "functions.hpp"
#include "mine.hpp"
#include <array>
#include <curses.h>
#include <iostream>
#include <ncurses.h>

using std::cin;
using std::cout;
using std::endl;

const std::array<difficulty, 3> difficultyArray = {
    {{10, 10, 15}, {15, 15, 30}, {20, 20, 60}},
};

int main() {

  cout << "Выберите сложность: " << endl;
  cout << "0 - лёгко" << endl;
  cout << "1 - нормально" << endl;
  cout << "2 - сложно" << endl;
  int difficultyChoice{-1};

  while (!(difficultyChoice >= 0 && difficultyChoice < 3)) {
    cin >> difficultyChoice;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(999, '\n');
      difficultyChoice = -1;
      cout << "Введите число от 0 до 2: ";
    }
  }

  const auto &xSize{difficultyArray[difficultyChoice].columns},
      &ySize{difficultyArray[difficultyChoice].rows},
      &minesNumber{difficultyArray[difficultyChoice].minesCount};

  const int openCellsToWin{xSize * ySize - minesNumber};

  const auto board = initBoard(xSize, ySize, minesNumber);
  
  initscr();
  start_color();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_CYAN);
  init_pair(3, COLOR_RED, COLOR_BLACK);


  for (int y = 0; y < ySize; y++) {
    for (int x = 0; x < xSize * 2 - 1; x += 2) {
      mvaddch(y, x, '#');
    }
  }

  cursor cur{0, 0, 0, 0};
  bool stop{false}, losed{false}, won{false};
  int openedCells{0};
  while (true) {
    bool toOpen{false}, toFlag{false};

    if (openedCells == openCellsToWin)
      won = true;

    if (stop || losed || won)
      break;

    switch (getch()) {
    case KEY_UP:
      cur.prevY = cur.y;
      cur.prevX = cur.x;
      cur.y--;
      if (cur.y < 0)
        cur.y = ySize - 1;
      break;
    case KEY_DOWN:
      cur.prevY = cur.y;
      cur.prevX = cur.x;
      cur.y++;
      if (cur.y > (ySize - 1))
        cur.y = 0;
      break;
    case KEY_LEFT:
      cur.prevX = cur.x;
      cur.prevY = cur.y;
      cur.x--;
      if (cur.x < 0)
        cur.x = xSize - 1;
      break;
    case KEY_RIGHT:
      cur.prevX = cur.x;
      cur.prevY = cur.y;
      cur.x++;
      if (cur.x > (xSize - 1))
        cur.x = 0;
      break;
    case 'f':
    case 'F':
      toFlag = true;
      break;
    case 27: // ESC
      stop = true;
      break;
    case 10: // Enter
      toOpen = true;
      break;
    }

    auto previousCell = board[cur.prevY][cur.prevX];

    if (previousCell.isOpened)
      mvaddch(cur.prevY, cur.prevX * 2, (char)(previousCell.minesNear + '0'));
    else if (previousCell.isFlaged) {
      attrset(COLOR_PAIR(3));
      mvaddch(cur.prevY, cur.prevX * 2, 'F');
      attrset(COLOR_PAIR(1));
    } else
      mvaddch(cur.prevY, cur.prevX * 2, '#');

    auto &selected_cell = board[cur.y][cur.x];

    attrset(COLOR_PAIR(2));
    if (selected_cell.isFlaged)
      mvaddch(cur.y, cur.x * 2, 'F');
    else if (selected_cell.isOpened)
      mvaddch(cur.y, cur.x * 2, (char)(selected_cell.minesNear + '0'));
    else
      mvaddch(cur.y, cur.x * 2, '#');

    if (toFlag && !selected_cell.isOpened) {
      selected_cell.isFlaged = !selected_cell.isFlaged;
      if (selected_cell.isFlaged) {
        attrset(COLOR_PAIR(2));
        mvaddch(cur.y, cur.x * 2, 'F');
        attrset(COLOR_PAIR(1));
      }
    }

    if (toOpen && !selected_cell.isFlaged) {
      if (!selected_cell.isOpened)
        openedCells++;
      selected_cell.isOpened = true;
      if (selected_cell.isMined) {
        losed = true;
        continue;
      }
      mvaddch(cur.y, cur.x * 2, (char)(selected_cell.minesNear + '0'));
    }
    attrset(COLOR_PAIR(1));
  }

  attrset(COLOR_PAIR(1));

  for (int y = 0; y < ySize; y++) {
    for (int x = 0; x < xSize * 2 - 1; x += 2) {
      cell current_cell = board[y][x / 2];
      if (current_cell.isMined) {
        attrset(COLOR_PAIR(3));
        mvaddch(y, x, 'M');
        attrset(COLOR_PAIR(1));
      } else
        mvaddch(y, x, (char)(current_cell.minesNear + '0'));
    }
  }

  if (won)
    printw("\nYou won!");
  if (losed)
    printw("\nYou losed!");
  getch();

  endwin();

  for (int i = 0; i < ySize; i++) {
    delete[] board[i];
  }
  delete[] board;
}
