#include "maze.h"

namespace s21 {

void Maze::GenerateMaze() {
  std::vector<Cell> line;
  line.reserve(width_);
  for (size_t i = 0; i < width_; ++i) line.push_back(Cell(i + 1));

  if (!is_loaded_maze_) {
    matrix_result_.Reserve(height_ * width_);
    RandomizeWalls();
  }
  unique_indx_ = width_ + 2;
  for (size_t i = 0; i < height_; ++i) {
    PrepareProcessingLine(line);
    HandleRightWalls(line, i);
    HandleBottomWalls(line, i);
    matrix_result_.PushVector(line);
  }
  HandleLastLine();
  SetBoundaries();
  is_loaded_maze_ = false;
  maze_generated_ = true;
}

void Maze::SetBoundaries() {
  ++height_;
  ++width_;
  matrix_result_.SetColsSize(matrix_result_.Cols() + 1);
  for (size_t i = 0; i < width_ - 1; ++i) {
    matrix_result_.Insert(Cell(0, false, true), 0);
    matrix_result_[0].path_val_ = -2;
  }
  for (size_t i = 0; i < height_; ++i) {
    matrix_result_.Insert(Cell(0, true, false), i * matrix_result_.Cols());
    matrix_result_[i * matrix_result_.Cols()].path_val_ = -2;
  }
  for (size_t i = 1; i < height_; ++i) {
    matrix_result_(i, width_ - 1).SetRightWall();
  }

  matrix_result_(0, 0).RemoveRightWall();
}

void Maze::LoadMaze(std::string path) {
  std::ifstream fin;
  std::vector<size_t> vec;
  size_t counter = 0;
  fin.open(path);
  if (fin.is_open()) {
    std::string str;
    while (std::getline(fin, str)) {
      if (str.length() <= 1) {
        counter++;
      } else {
        switch (counter) {
          case 0:
            ParseMatrixSize(str);
            counter++;
            break;
          case 1:
            ParseRightWallMatrix(str);
            break;
          case 2:
            ParseBottomWallMatrix(str);
            break;
          default:
            break;
        }
      }
    }
    if (SuccessfulParsing())
      is_loaded_maze_ = true;
    else
      Clear();
  }
}

void Maze::ParseBottomWallMatrix(std::string str) {
  char* lex = std::strtok((char*)str.data(), " ");
  while (lex != NULL) {
    matrix_bottom_walls_.Push(std::atoi(lex));
    lex = std::strtok(NULL, " ");
  }
}

void Maze::ParseRightWallMatrix(std::string str) {
  char* lex = std::strtok((char*)str.data(), " ");
  while (lex != NULL) {
    matrix_right_walls_.Push(std::atoi(lex));
    lex = std::strtok(NULL, " ");
  }
}

void Maze::ParseMatrixSize(std::string str) {
  char* lex = std::strtok((char*)str.data(), " ");
  width_ = std::atoi(lex);
  lex = std::strtok(NULL, " ");
  height_ = std::atoi(lex);
  size_t size = width_ * height_;
  matrix_result_.SetColsSize(width_);
  matrix_right_walls_.SetColsSize(width_);
  matrix_bottom_walls_.SetColsSize(width_);
  matrix_result_.Reserve(size);
  matrix_right_walls_.Reserve(size);
  matrix_bottom_walls_.Reserve(size);
}

void Maze::HandleRightWalls(std::vector<Cell>& vec, size_t row) {
  for (size_t i = 0; i < width_ - 1; ++i) {
    if (matrix_right_walls_(row, i) == 1 || vec[i].sets_ == vec[i + 1].sets_) {
      vec[i].SetRightWall();
    } else {
      CombineSets(vec, vec[i].sets_, vec[i + 1].sets_);
    }
  }
}

void Maze::HandleBottomWalls(std::vector<Cell>& vec, size_t row) {
  for (size_t i = 0; i < width_; ++i) {
    if (matrix_bottom_walls_(row, i) == 1 &&
        BottomWallPossible(vec, vec[i].sets_))
      vec[i].SetBottomWall();
  }
}

void Maze::RandomizeWalls() {
  matrix_right_walls_.SetColsSize(matrix_result_.Cols());
  matrix_bottom_walls_.SetColsSize(matrix_result_.Cols());
  matrix_right_walls_.Reserve(matrix_result_.Capacity());
  matrix_bottom_walls_.Reserve(matrix_result_.Capacity());
  matrix_right_walls_.FillWithRandomNumbers(0, 1);
  matrix_bottom_walls_.FillWithRandomNumbers(0, 1);
}

void Maze::CombineSets(std::vector<Cell>& vec, size_t val, size_t whichsets) {
  for (size_t i = 0; i < vec.size(); ++i) {
    if (vec[i].sets_ == whichsets) vec[i].sets_ = val;
  }
}

bool Maze::BottomWallPossible(std::vector<Cell>& vec, size_t sets) {
  size_t total = 0, free = 0;
  for (size_t i = 0; i < vec.size(); ++i) {
    if (vec[i].sets_ == sets) {
      ++total;
      if (!vec[i].bottom_wall_) ++free;
    }
  }
  return total > 1 && free > 1;
}

void Maze::PrepareProcessingLine(std::vector<Cell>& vec) {
  for (size_t i = 0; i < vec.size(); ++i) {
    vec[i].RemoveRightWall();
    if (vec[i].bottom_wall_) vec[i].sets_ = ++unique_indx_;
    vec[i].RemoveBottomWall();
  }
}

void Maze::HandleLastLine() {
  size_t cur = 0, swp = 0;
  for (size_t i = 0; i < width_ - 1; ++i) {
    matrix_result_(height_ - 1, i).SetBottomWall();
    if (matrix_result_(height_ - 1, i).sets_ !=
        matrix_result_(height_ - 1, i + 1).sets_)
      matrix_result_(height_ - 1, i).RemoveRightWall();
    cur = matrix_result_(height_ - 1, i).sets_;
    swp = matrix_result_(height_ - 1, i + 1).sets_;
    for (size_t z = 0; z < width_; ++z) {
      if (matrix_result_(height_ - 1, z).sets_ == swp)
        matrix_result_(height_ - 1, z).sets_ = cur;
    }
  }
  matrix_result_(height_ - 1, width_ - 1).SetBottomWall();
}

void Maze::SaveMaze(std::string path) {
  std::ofstream fout;
  fout.open(path);
  if (fout.is_open()) {
    fout << width_ - 1 << " " << height_ - 1 << "\n";
    for (size_t i = 0; i < matrix_right_walls_.Rows(); ++i) {
      for (size_t z = 0; z < matrix_right_walls_.Cols(); ++z) {
        fout << matrix_right_walls_(i, z);
        if (z != matrix_right_walls_.Cols() - 1) fout << " ";
      }
      fout << "\n";
    }

    fout << "\n";

    for (size_t i = 0; i < matrix_bottom_walls_.Rows(); ++i) {
      for (size_t z = 0; z < matrix_bottom_walls_.Cols(); ++z) {
        fout << matrix_bottom_walls_(i, z);
        if (z != matrix_bottom_walls_.Cols() - 1) fout << " ";
      }
      fout << "\n";
    }

    fout.close();
  }
}

void Maze::Navigate(size_t si, size_t sz, size_t fi, size_t fz) {
  if (CorrectIndx(si, sz) && CorrectIndx(fi, fz) && maze_generated_) {
    PathfinderClear();
    path_arr_.clear();
    SetStartCell(si, sz);
    SetFinishCell(fi, fz);
    CalculatePathVal(start_cell_.iindx_, start_cell_.zindx_);
    ChooseRoute();
  }
}

void Maze::Clear() {
  PathfinderClear();
  matrix_result_.Clear();
  matrix_bottom_walls_.Clear();
  matrix_right_walls_.Clear();
  height_ = width_ = 0;
  maze_generated_ = false;
}

void Maze::PathfinderClear() {
  for (size_t i = 0; i < matrix_result_.Size(); ++i) {
    matrix_result_[i].RemovePathCell();
    if (matrix_result_[i].path_val_ != -2) matrix_result_[i].path_val_ = -1;
  }
  start_cell_ = nullptr;
  finish_cell_ = nullptr;
  path_arr_.clear();
}

bool Maze::CorrectIndx(size_t i, size_t z) {
  if (i < 1 || i > height_ - 1) return false;
  if (z < 1 || z > width_ - 1) return false;
  return true;
}

void Maze::SetStartCell(size_t i, size_t z) {
  start_cell_.iindx_ = i;
  start_cell_.zindx_ = z;
  start_cell_.pcell_ = &matrix_result_(i, z);
  start_cell_.pcell_->Start();
  start_cell_.pcell_->path_val_ = 0;
}

void Maze::SetFinishCell(size_t i, size_t z) {
  finish_cell_.iindx_ = i;
  finish_cell_.zindx_ = z;
  finish_cell_.pcell_ = &matrix_result_(i, z);
  finish_cell_.pcell_->Finish();
}

void Maze::CalculatePathVal(size_t i, size_t z) {
  int nwval = matrix_result_(i, z).path_val_ + 1;
  if (!matrix_result_(i, z).right_wall_ &&
      matrix_result_(i, z + 1).path_val_ == -1) {
    matrix_result_(i, z + 1).path_val_ = nwval;
    CalculatePathVal(i, z + 1);
  }
  if (!matrix_result_(i, z).bottom_wall_ &&
      matrix_result_(i + 1, z).path_val_ == -1) {
    matrix_result_(i + 1, z).path_val_ = nwval;
    CalculatePathVal(i + 1, z);
  }
  if (!matrix_result_(i - 1, z).bottom_wall_ &&
      matrix_result_(i - 1, z).path_val_ == -1) {
    matrix_result_(i - 1, z).path_val_ = nwval;
    CalculatePathVal(i - 1, z);
  }
  if (!matrix_result_(i, z - 1).right_wall_ &&
      matrix_result_(i, z - 1).path_val_ == -1) {
    matrix_result_(i, z - 1).path_val_ = nwval;
    CalculatePathVal(i, z - 1);
  }
}

void Maze::ChooseRoute() {
  size_t i = finish_cell_.iindx_;
  size_t z = finish_cell_.zindx_;
  Cell* pcell = finish_cell_.pcell_;
  while (pcell->path_val_ != 0) {
    if (!matrix_result_(i, z).bottom_wall_ &&
        matrix_result_(i + 1, z).path_val_ == pcell->path_val_ - 1) {
      pcell = &matrix_result_(++i, z);
      pcell->SetPathCell();
    }
    if (!matrix_result_(i, z).right_wall_ &&
        matrix_result_(i, z + 1).path_val_ == pcell->path_val_ - 1) {
      pcell = &matrix_result_(i, ++z);
      pcell->SetPathCell();
    }
    if (!matrix_result_(i - 1, z).bottom_wall_ &&
        matrix_result_(i - 1, z).path_val_ == pcell->path_val_ - 1) {
      pcell = &matrix_result_(--i, z);
      pcell->SetPathCell();
    }
    if (!matrix_result_(i, z - 1).right_wall_ &&
        matrix_result_(i, z - 1).path_val_ == pcell->path_val_ - 1) {
      pcell = &matrix_result_(i, --z);
      pcell->SetPathCell();
    }
  }
  for (size_t i = 0; i < matrix_result_.Rows(); ++i) {
    for (size_t z = 0; z < matrix_result_.Cols(); ++z) {
      if (matrix_result_(i, z).part_of_path_)
        path_arr_.push_back(PathSignCell(&matrix_result_(i, z), i, z));
    }
  }
  std::sort(path_arr_.begin(), path_arr_.end(),
            [](PathSignCell ptr_l, PathSignCell ptr_r) {
              return ptr_l.pcell_->path_val_ < ptr_r.pcell_->path_val_;
            });
}

bool Maze::SuccessfulParsing() {
  return width_ > 1 && height_ > 1 && matrix_right_walls_.Size() > 1 &&
         matrix_bottom_walls_.Size() > 1;
}

};  // namespace s21
