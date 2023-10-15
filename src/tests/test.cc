#include <gtest/gtest.h>

#include "../controller/controller.h"

TEST(MazeGenerate, Test1) {
  s21::Controller a;
  a.CreateMaze(10, 10);
  for (size_t i = 1; i <= a.MazeHeight(); ++i) {
    for (size_t z = 1; z <= a.MazeWidth(); ++z) {
      if (!(i == 1 && z == 1)) {
        a.BuildRoute(1, 1, i, z);
        EXPECT_TRUE(a.PathCellArray().size() > 0);
      }
    }
  }
}

TEST(MazeGenerate, Test2) {
  s21::Controller a;
  a.CreateMaze(50, 50);
  for (size_t i = 1; i <= a.MazeHeight(); ++i) {
    for (size_t z = 1; z <= a.MazeWidth(); ++z) {
      if (!(i == 1 && z == 1)) {
        a.BuildRoute(1, 1, i, z);
        EXPECT_TRUE(a.PathCellArray().size() > 0);
      }
    }
  }
}

TEST(MazeGenerate, Test3) {
  s21::Controller a;
  a.CreateMaze(5, 20);
  a.BuildRoute(1, 1, 9, 5);
  for (size_t i = 1; i <= a.MazeHeight(); ++i) {
    for (size_t z = 1; z <= a.MazeWidth(); ++z) {
      if (!(i == 1 && z == 1)) {
        a.BuildRoute(1, 1, i, z);
        EXPECT_TRUE(a.PathCellArray().size() > 0);
      }
    }
  }
}

TEST(MazeNavigator, Test1) {
  s21::Controller a;
  a.CreateMaze("tests/testfiles/test1.txt");
  a.BuildRoute(1, 1, 4, 4);
  EXPECT_TRUE(a.MazeMatrix()(1, 1).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(1, 2).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(1, 3).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(1, 4).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(2, 4).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(3, 4).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(3, 3).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(2, 4).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(4, 4).part_of_path_ == true);
}

TEST(MazeNavigator, Test2) {
  s21::Controller a;
  a.CreateMaze(25, 25);
  a.CreateMaze("tests/testfiles/test2.txt");
  a.BuildRoute(1, 1, 5, 5);
  EXPECT_TRUE(a.MazeMatrix()(1, 1).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(1, 2).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(1, 3).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(2, 3).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(3, 3).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(3, 2).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(4, 2).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(4, 3).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(4, 4).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(5, 4).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(5, 5).part_of_path_ == true);
}

TEST(MazeNavigator, Test3) {
  s21::Controller a;
  a.CreateMaze("tests/testfiles/test3.txt");
  a.BuildRoute(1, 1, 5, 5);
  EXPECT_TRUE(a.MazeMatrix()(1, 1).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(2, 1).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(3, 1).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(4, 1).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(5, 1).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(5, 2).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(4, 2).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(3, 2).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(3, 3).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(3, 4).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(2, 4).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(2, 3).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(1, 3).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(1, 4).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(1, 5).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(1, 6).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(1, 7).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(2, 7).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(3, 7).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(4, 7).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(5, 7).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(5, 6).part_of_path_ == true);
  EXPECT_TRUE(a.MazeMatrix()(5, 5).part_of_path_ == true);
}

TEST(MazeSave, Test1) {
  s21::Controller a;
  a.CreateMaze("tests/testfiles/test1.txt");
  a.SaveMaze("save.txt");
  std::ifstream fin1, fin2;
  std::string str1, str2;
  fin1.open("tests/testfiles/test1.txt");
  fin2.open("save.txt");
  if (fin1.is_open() && fin2.is_open()) {
    while (std::getline(fin1, str1)) {
      std::getline(fin2, str2);
      EXPECT_TRUE(str1.compare(str2) == 0);
    }
  }
  fin1.close();
  fin2.close();
  std::remove("save.txt");
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}