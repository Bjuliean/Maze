#ifndef MAZE_H
#define MAZE_H

#include <algorithm>

#include "../data/matrix.hpp"

namespace s21 {

/*!
    \brief Maze cell structure.
*/
struct Cell {
    Cell(size_t sets = 0, bool rw = false, bool bw = false)
        : sets_(sets)
        , right_wall_(rw)
        , bottom_wall_(bw)
        , part_of_path_(false)
        , path_val_(-1)
        , start_(false)
        , finish_(false)
    {
    }

    Cell(const Cell& other)
        : sets_(other.sets_)
        , right_wall_(other.right_wall_)
        , bottom_wall_(other.bottom_wall_)
        , part_of_path_(other.part_of_path_)
        , path_val_(other.path_val_)
        , start_(other.start_)
        , finish_(other.finish_)
    {
    }

    ~Cell() = default;

    void SetRightWall() { right_wall_ = true; }

    void SetBottomWall() { bottom_wall_ = true; }

    void SetPathCell() { part_of_path_ = true; }

    void RemoveRightWall() { right_wall_ = false; }

    void RemoveBottomWall() { bottom_wall_ = false; }

    void RemovePathCell() { part_of_path_ = false; }

    void Start() { 
        start_ = true;
        part_of_path_ = true;
    }

    void Finish() { 
        finish_ = true;
        part_of_path_ = true;
    }

    void RemoveAllWalls() {
        right_wall_ = false;
        bottom_wall_ = false;
    }

    friend std::ostream& operator<<(std::ostream& os, const Cell cell) {
        return os << cell.sets_;
    }

    size_t sets_;
    bool right_wall_;
    bool bottom_wall_;
    bool part_of_path_;
    int path_val_;
    bool start_;
    bool finish_;

}; // Cell

/*!
    \brief The structure of the cell that is a participant in the constructed route.
*/
struct PathSignCell {
    PathSignCell() = default;

    PathSignCell(Cell* cell, size_t ii = 1, size_t zi = 1)
        : pcell_(cell)
        , iindx_(ii)
        , zindx_(zi)
    {
    }

    ~PathSignCell() = default;

    Cell* pcell_;
    size_t iindx_;
    size_t zindx_;

}; // PathSignCell


/*!
    \brief The class responsible for working with the maze.
*/
class Maze {
public:
    Maze(size_t width = 1, size_t height = 1)
        : matrix_result_(0, 0)
        , matrix_right_walls_(0, 0)
        , matrix_bottom_walls_(0, 0)
        , width_(width)
        , height_(height)
        , is_loaded_maze_(false)
        , unique_indx_(0)
        , start_cell_()
        , finish_cell_()
        , path_arr_()
        , maze_generated_(false)
    {
        matrix_result_.SetColsSize(width_);
    }

    ~Maze() = default;

    /*!
        Sets the given width to the maze, sets the given width to the matrices of the maze.
    */
    inline void SetWidth(size_t val) { 
        width_ = val;
        matrix_result_.SetColsSize(width_);
    }

    /*!
        Sets the given height to the maze.
    */
    inline void SetHeight(size_t val) { height_ = val; }

    /*!
        Access to maze width.
    */
    inline size_t GetWidth() const noexcept { return width_ - 1; }
    
    /*!
        Access to maze heigth.
    */
    inline size_t GetHeight() const noexcept { return height_ - 1; }

    /*!
        Access to a specific cell of the maze.
    */
    inline Cell& GetCell(size_t i, size_t z) { return matrix_result_(i, z); }

    /*!
        Access to a vector containing all the cells that belong to the constructed route.
    */
    inline std::vector<PathSignCell>& PathCellArray() { return path_arr_; }

    /*!
        A method responsible for constructing a route according to given coordinates.
    */
    void Navigate(size_t si, size_t sz, size_t fi, size_t fz);

    /*!
        The method responsible for generating maze.
    */
    void GenerateMaze();

    /*!
        Cleaning matrices.
    */
    void Clear();

    /*!
        Cleaning up path-building data.
    */
    void PathfinderClear();

    /*!
        Save the generated maze to a file at the specified path.
    */
    void SaveMaze(std::string path);

    /*!
        Loading maze along a given path.
    */
    void LoadMaze(std::string path);

    /*!
        Access to the final maze matrix.
    */
    inline Matrix<Cell>& MatrixResult() { return matrix_result_; }

    /*!
        Was it possible to generate maze.
    */
    inline bool MazeGenerated() const noexcept { return maze_generated_; }

    /*!
        Was it possible to load maze from file.
    */
    inline bool MazeLoaded() const noexcept { return is_loaded_maze_; }

private:
    Matrix<Cell> matrix_result_;
    Matrix<size_t> matrix_right_walls_;
    Matrix<size_t> matrix_bottom_walls_;
    size_t width_;
    size_t height_;
    bool is_loaded_maze_;
    size_t unique_indx_;
    PathSignCell start_cell_;
    PathSignCell finish_cell_;
    std::vector<PathSignCell> path_arr_;
    bool maze_generated_;

    void RandomizeWalls();

    void HandleRightWalls(std::vector<Cell>& vec, size_t row);

    void HandleBottomWalls(std::vector<Cell>& vec, size_t row);

    void CombineSets(std::vector<Cell>& vec, size_t val, size_t whichsets);

    bool BottomWallPossible(std::vector<Cell>& vec, size_t sets);

    void PrepareProcessingLine(std::vector<Cell>& vec);

    void HandleLastLine();

    void ParseMatrixSize(std::string str);

    void ParseRightWallMatrix(std::string str);

    void ParseBottomWallMatrix(std::string str);

    void SetBoundaries();

    void SetStartCell(size_t i, size_t z);

    void SetFinishCell(size_t i, size_t z);

    void CalculatePathVal(size_t i, size_t z);

    void ChooseRoute();

    bool CorrectIndx(size_t i, size_t z);

    bool SuccessfulParsing();

}; // Maze

}; // namespace s21

#endif // MAZE_H