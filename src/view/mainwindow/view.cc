#include "view.h"

#include "ui_view.h"

namespace s21 {

view::view(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::view),
      scene_(),
      unit_height_(0),
      unit_width_(0),
      maze_height_(0),
      maze_width_(0),
      controller_(),
      maze_color_(),
      path_color_(),
      background_color_(0, 115, 153) {
  ui->setupUi(this);
  scene_ = new QGraphicsScene;

  maze_color_.setColor(QColor(0, 0, 0));
  path_color_.setColor(QColor(225, 0, 0));
  SceneInit();
  ButtonsInit();
  SlotConnect();
}

view::~view() {
  delete scene_;
  delete ui;
}

void view::SceneInit() {
  ui->graphicsView->setScene(scene_);
  ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scene_->setSceneRect(0, 0, 500, 500);
  DrawSceneBackground();
}

void view::DrawSceneBackground() {
  QColor nwcolor(background_color_);
  QPolygon pol;
  pol << QPoint(0, 0) << QPoint(500, 0) << QPoint(500, 500) << QPoint(0, 500);
  scene_->addPolygon(pol, QPen(), QBrush(nwcolor, Qt::SolidPattern));
}

void view::ButtonsInit() {
  ui->spinBox_rx->setMinimum(1);
  ui->spinBox_ry->setMinimum(1);
  ui->spinBox_rx->setMaximum(50);
  ui->spinBox_ry->setMaximum(50);

  ui->spinBox_fx->setMinimum(1);
  ui->spinBox_fy->setMinimum(1);
  ui->spinBox_fx->setMaximum(ui->spinBox_rx->value());
  ui->spinBox_fy->setMaximum(ui->spinBox_ry->value());

  ui->spinBox_tx->setMinimum(1);
  ui->spinBox_ty->setMinimum(1);
  ui->spinBox_tx->setMaximum(ui->spinBox_rx->value());
  ui->spinBox_ty->setMaximum(ui->spinBox_ry->value());
}

void view::SlotConnect() {
  connect(ui->spinBox_rx, SIGNAL(valueChanged(int)), this,
          SLOT(MazeParamsUpdate()));
  connect(ui->spinBox_ry, SIGNAL(valueChanged(int)), this,
          SLOT(MazeParamsUpdate()));
}

void view::MazeParamsUpdate() {
  ui->spinBox_fx->setMaximum(ui->spinBox_rx->value());
  ui->spinBox_fy->setMaximum(ui->spinBox_ry->value());
  ui->spinBox_tx->setMaximum(ui->spinBox_rx->value());
  ui->spinBox_ty->setMaximum(ui->spinBox_ry->value());
}

void view::ConfigMazeDraw() {
  maze_height_ = controller_.MazeHeight();
  maze_width_ = controller_.MazeWidth();
  unit_height_ = (scene_->height() / maze_height_);
  unit_width_ = (scene_->width() / maze_width_);
  ui->label_maxx->setText(QString::number(maze_width_));
  ui->label_maxy->setText(QString::number(maze_height_));
}

void view::Draw() {
  scene_->clear();
  ConfigMazeDraw();
  DrawSceneBackground();
  for (size_t i = 0; i < maze_height_; ++i) {
    for (size_t z = 0; z < maze_width_; ++z) {
      if (controller_.MazeMatrix()(i + 1, z + 1).right_wall_)
        scene_->addLine(
            QLineF(QPoint(unit_width_ * (z + 1), unit_height_ * i),
                   QPoint(unit_width_ * (z + 1), unit_height_ * (i + 1))),
            maze_color_);
      if (controller_.MazeMatrix()(i, z + 1).bottom_wall_)
        scene_->addLine(QLineF(QPoint(unit_width_ * z, unit_height_ * i),
                               QPoint(unit_width_ * (z + 1), unit_height_ * i)),
                        maze_color_);
    }
  }
}

void view::BuildRoute() {
  if (controller_.MazeGenerated()) {
    Draw();
    size_t fx = static_cast<size_t>(ui->spinBox_fx->value());
    size_t fy = static_cast<size_t>(ui->spinBox_fy->value());
    size_t tx = static_cast<size_t>(ui->spinBox_tx->value());
    size_t ty = static_cast<size_t>(ui->spinBox_ty->value());

    controller_.BuildRoute(fy, fx, ty, tx);

    for (size_t i = 0; i < controller_.PathCellArray().size() - 1; ++i) {
      size_t x = controller_.PathCellArray()[i].zindx_;
      size_t y = controller_.PathCellArray()[i].iindx_;
      size_t xn = controller_.PathCellArray()[i + 1].zindx_;
      size_t yn = controller_.PathCellArray()[i + 1].iindx_;
      scene_->addLine(QLineF(QPoint((unit_width_ * x) - (unit_width_ / 2),
                                    (unit_height_ * y) - (unit_height_ / 2)),
                             QPoint((unit_width_ * xn) - (unit_width_ / 2),
                                    (unit_height_ * yn) - (unit_height_ / 2))),
                      path_color_);
    }
  }
}

void view::on_pushButton_generate_clicked() {
  controller_.CreateMaze(ui->spinBox_rx->value(), ui->spinBox_ry->value());
  Draw();
}

void view::on_pushButton_load_clicked() {
  QString file = QFileDialog::getOpenFileName();
  if (file.contains(".txt")) {
    controller_.CreateMaze(file.toStdString());
    if (!controller_.MazeMatrix().IsEmpty()) {
      ui->spinBox_rx->setValue(controller_.MazeWidth());
      ui->spinBox_ry->setValue(controller_.MazeHeight());
      Draw();
    }
  }
}

void view::on_pushButton_buildroute_clicked() { BuildRoute(); }

void view::on_pushButton_clean_clicked() {
  controller_.Clear();
  scene_->clear();
  DrawSceneBackground();
}

void view::on_pushButton_mazecolor_clicked() {
  maze_color_.setColor(QColorDialog::getColor(nullptr));
  if (controller_.MazeGenerated()) {
    Draw();
    BuildRoute();
  }
}

void view::on_pushButton_pathcolor_clicked() {
  path_color_.setColor(QColorDialog::getColor(nullptr));
  BuildRoute();
}

void view::on_pushButton_backgroundcolor_clicked() {
  background_color_ = QColorDialog::getColor(nullptr);
  if (controller_.MazeGenerated()) {
    Draw();
    BuildRoute();
  } else
    DrawSceneBackground();
}

void view::on_pushButton_savefile_clicked() {
  QString save = QFileDialog::QFileDialog::getSaveFileName(
      nullptr, "Save maze configuration as: ", QDir::currentPath(), "",
      nullptr);
  if (!save.isEmpty()) {
    if (!save.contains(".txt")) save.append(".txt");
    controller_.SaveMaze(save.toStdString());
  }
}

};  // namespace s21
