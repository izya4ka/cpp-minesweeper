#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "cell.hpp"
#include <functional>
#include <vector>

std::vector<std::vector<cell>> init_board(const int x_size, const int y_size,
                                          const int mines_number);
void colored_output(std::function<void(void)> f, const int color_pair);

#endif