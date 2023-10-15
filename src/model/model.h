#ifndef MODEL_H
#define MODEL_H

#include "maze/maze.h"

namespace s21 {

/*!
    \brief The base class of model.
*/
class Model {
public:
    Model() = default;

    ~Model() = default;

    /*!
        Generate a random maze based on a given height and width.
    */
    void CreateMaze(size_t height, size_t width);

    /*!
        Generate a maze obtained from a file at a given path.
    */
    void CreateMaze(std::string path);

    /*!
        Build a route according to given coordinates.
    */
    void BuildRoute(size_t start_cell_y, size_t start_cell_x, size_t finish_cell_y, size_t finish_cell_x);

    /*!
        Save the generated maze to a file at the specified path.
    */
    void SaveMaze(std::string path);

    /*!
        Access to maze height.
    */
    inline size_t MazeHeight() { return maze_.GetHeight(); }

    /*!
        Access to maze width.
    */
    inline size_t MazeWidth() { return maze_.GetWidth(); }

    /*!
        Access to maze final matrix.
    */
    inline Matrix<Cell>& MazeMatrix() { return maze_.MatrixResult(); }
    
    /*!
        Access to a vector containing all the cells that belong to the constructed route. 
    */
    inline std::vector<PathSignCell>& PathCellArray() { return maze_.PathCellArray(); }

    /*!
        Was it possible to generate maze.
    */
    inline bool MazeGenerated() const noexcept { return maze_.MazeGenerated(); }

    /*!
        Cleaning matrices.
    */
    inline void Clear() { maze_.Clear(); }

private:
    Maze maze_;

}; // Model

}; // namespace s21

#endif // MODEL_H