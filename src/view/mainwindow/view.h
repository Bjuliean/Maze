#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QColorDialog>

#include "../controller/controller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class view; }
QT_END_NAMESPACE

namespace s21 {

/*!
    \brief The base class of view.
*/
class view : public QMainWindow
{
    Q_OBJECT

public:
    view(QWidget *parent = nullptr);
    ~view();

    /*!
        Pre-setting widget window settings.
    */
    void SceneInit();

    /*!
        Maze drawing.
    */
    void Draw();

    /*!
        Setting up maze parameters.
    */
    void ConfigMazeDraw();

    /*!
        Some button connection.
    */
    void SlotConnect();

    /*!
        Pre-setting buttons.
    */
    void ButtonsInit();

    /*!
        Generate a route using the coordinates specified on the spinboxes.
    */
    void BuildRoute();

    /*!
        Drawing the scene widget background.
    */
    void DrawSceneBackground();

private slots:
    void on_pushButton_generate_clicked();

    void on_pushButton_load_clicked();

    void MazeParamsUpdate();

    void on_pushButton_buildroute_clicked();

    void on_pushButton_clean_clicked();

    void on_pushButton_mazecolor_clicked();

    void on_pushButton_pathcolor_clicked();

    void on_pushButton_backgroundcolor_clicked();

    void on_pushButton_savefile_clicked();

private:
    Ui::view *ui;
    QGraphicsScene *scene_;
    size_t unit_height_;
    size_t unit_width_;
    size_t maze_height_;
    size_t maze_width_;
    s21::Controller controller_;
    QPen maze_color_;
    QPen path_color_;
    QColor background_color_;
};

}; // namespace s21
#endif // VIEW_H
