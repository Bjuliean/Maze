#include "controller.h"

namespace s21 {

void Controller::CreateMaze(size_t height, size_t width) {
  model_.CreateMaze(height, width);
}

void Controller::CreateMaze(std::string path) { model_.CreateMaze(path); }

void Controller::BuildRoute(size_t start_cell_y, size_t start_cell_x,
                            size_t finish_cell_y, size_t finish_cell_x) {
  model_.BuildRoute(start_cell_y, start_cell_x, finish_cell_y, finish_cell_x);
}

void Controller::SaveMaze(std::string path) { model_.SaveMaze(path); }

};  // namespace s21