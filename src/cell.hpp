#ifndef MINE_HPP
#define MINE_HPP

struct cell {
  int x;
  int y;
  int mines_near;
  bool is_mined;
  bool is_flagged;
  bool is_opened;
};

#endif