#include "cursor.hpp"
#include "functions.hpp"
#include "mine.hpp"
#include <curses.h>
#include <iostream>
#include <ncurses.h>

using std::cin;
using std::cout;

int main() {
  int xSize{10}, ySize{10}, minesNumber{5};
  cout << "Введите размер поля по высоте (в клетках): ";
  cin >> xSize;
  cout << "Введите размер поля по ширине (в клетках): ";
  cin >> ySize;
  cout << "Введите количество мин: ";
  cin >> minesNumber;

  int openCellsToWin{xSize * ySize - minesNumber};

  initscr();
  start_color();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_CYAN);
  init_pair(3, COLOR_RED, COLOR_BLACK);

  auto board = initBoard(xSize, ySize, minesNumber);

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
    case 259: // Вверх
      cur.prevY = cur.y;
      cur.prevX = cur.x;
      cur.y--;
      if (cur.y < 0)
        cur.y = ySize - 1;
      break;
    case 258: // Вниз
      cur.prevY = cur.y;
      cur.prevX = cur.x;
      cur.y++;
      if (cur.y > (ySize - 1))
        cur.y = 0;
      break;
    case 260: // Влево
      cur.prevX = cur.x;
      cur.prevY = cur.y;
      cur.x--;
      if (cur.x < 0)
        cur.x = xSize - 1;
      break;
    case 261: // Вправо
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
      mvaddch(cur.prevY, cur.prevX * 2, (char)(previousCell.minesNear + 48));
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
      mvaddch(cur.y, cur.x * 2, (char)(selected_cell.minesNear + 48));
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
      mvaddch(cur.y, cur.x * 2, (char)(selected_cell.minesNear + 48));
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
        mvaddch(y, x, (char)(current_cell.minesNear + 48));
    }
  }

  if (won)
    printw("\nYou won!");
  if (losed)
    printw("\nYou losed!");
  printw("\nPress any key...");
  getch();
  endwin();

  for (int i = 0; i < ySize; i++) {
    delete[] board[i];
  }
  delete[] board;
}
