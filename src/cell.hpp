#ifndef MINE_HPP
#define MINE_HPP

struct cell {
  unsigned x;
  unsigned y;
  unsigned mines_near;
  bool is_mined;
  bool is_flagged;
  bool is_opened;
};

#endif