#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../model/model.h"

namespace s21 {

/*!
    \brief The base class of controller.
*/
class Controller {
public:
    Controller() = default;

    ~Controller() = default;

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
    inline size_t MazeHeight() { return model_.MazeHeight(); }

    /*!
        Access to maze width.
    */
    inline size_t MazeWidth() { return model_.MazeWidth(); }

    /*!
        Access to maze final matrix.
    */
    inline Matrix<Cell>& MazeMatrix() { return model_.MazeMatrix(); }

    /*!
        Access to a vector containing all the cells that belong to the constructed route. 
    */
    inline std::vector<PathSignCell>& PathCellArray() { return model_.PathCellArray(); }

    /*!
        Was it possible to generate maze.
    */
    inline bool MazeGenerated() const noexcept { return model_.MazeGenerated(); }

    /*!
        Cleaning matrices.
    */
    inline void Clear() { model_.Clear(); }

private:
    Model model_;

}; // Controller

}; // namespace s21

#endif // CONTROLLER_H