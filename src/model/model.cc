#include "model.h"

namespace s21 {

void Model::CreateMaze(size_t height, size_t width) {
  maze_.Clear();
  maze_.SetHeight(height);
  maze_.SetWidth(width);
  maze_.GenerateMaze();
}

void Model::CreateMaze(std::string path) {
  maze_.Clear();
  maze_.LoadMaze(path);
  if (maze_.MazeLoaded()) maze_.GenerateMaze();
}

void Model::BuildRoute(size_t start_cell_y, size_t start_cell_x,
                       size_t finish_cell_y, size_t finish_cell_x) {
  maze_.Navigate(start_cell_y, start_cell_x, finish_cell_y, finish_cell_x);
}

void Model::SaveMaze(std::string path) { maze_.SaveMaze(path); }

};  // namespace s21