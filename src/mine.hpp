#ifndef MINE_HPP
#define MINE_HPP

struct cell {
  unsigned x;
  unsigned y;
  unsigned minesNear;
  bool isMined;
  bool isFlaged;
  bool isOpened;
};

#endif