#include <functional>
#include <ncurses.h>

void colored_output(std::function<void(void)> f, const int color_pair) {
  attrset(COLOR_PAIR(color_pair));
  f();
  attroff(COLOR_PAIR(color_pair));
}