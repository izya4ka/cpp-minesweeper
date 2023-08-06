#include "cursor.hpp"
#include "functions.hpp"
#include "mine.hpp"
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
  system("clear");

  int openCellsToWin{xSize * ySize - minesNumber};

  initscr();
  start_color();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  auto board = initBoard(xSize, ySize, minesNumber);

  for (int y = 0; y < ySize; y++) {
    for (int x = 0; x < xSize * 2 - 1; x += 2) {
      mvaddch(y, x, '#');
    }
  }

  cursor cur{0, 0};
  bool stop{false}, losed{false}, won{false}, isFirstTurn{true};
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
    else if (previousCell.isFlaged)
      mvaddch(cur.prevY, cur.prevX * 2, 'F');
    else
      mvaddch(cur.prevY, cur.prevX * 2, '#');

    mvaddch(cur.y, cur.x * 2, '*');

    auto &selected_cell = board[cur.y][cur.x];

    if (toFlag) {
      selected_cell.isFlaged = !selected_cell.isFlaged;
      if (selected_cell.isFlaged)
        mvaddch(cur.y, cur.x * 2, 'F');
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
  }

  char char_to_display;

  for (int y = 0; y < ySize; y++) {
    for (int x = 0; x < xSize * 2 - 1; x += 2) {
      cell current_cell = board[y][x / 2];
      if (current_cell.isMined)
        char_to_display = 'M';
      else
        char_to_display = (char)(current_cell.minesNear + 48);
      mvaddch(y, x, char_to_display);
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
