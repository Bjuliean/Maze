CC=g++ -std=c++17
CFLAGS=-Wall -Wextra -Werror
TFLAGS=-lgtest -std=c++17
SRC=model/*.cc model/maze/*.cc controller/*.cc
SRC_FROM_REPORT=../model/*.cc ../model/maze/*.cc ../controller/*.cc ../tests/*.cc
ALL_SRC=*.cc model/*.cc model/maze/*.cc controller/*.cc tests/*.cc view/mainwindow/*.cc
TEST=tests/test.cc
TEST_OUTPUT=unit_test
GCOV_PATH=report
APPNAME=Maze
ARCHIVE=Archive_Maze
DOC_PATH=documentation

all: gcov_report

test: clean
	$(CC) $(TEST) $(SRC) $(CFLAGS) $(TFLAGS) -o $(TEST_OUTPUT)
	./$(TEST_OUTPUT)

gcov_report: clean
	mkdir $(GCOV_PATH) && cd $(GCOV_PATH) && $(CC) --coverage $(SRC_FROM_REPORT) $(CFLAGS) $(TFLAGS) -o gcov_report
	./$(GCOV_PATH)/gcov_report
	lcov -t "gcov_report" -o $(GCOV_PATH)/gcov_report.info --no-external -c -d ./
	genhtml -o report $(GCOV_PATH)/gcov_report.info
	open $(GCOV_PATH)/index.html

clean:
	rm -rf *.gcda *.gcno *.info *.txt $(TEST_OUTPUT) $(GCOV_PATH)

cleanall: clean uninstall
	rm -rf *.tgz build
	cd $(DOC_PATH) && rm -rf html latex

valgrind: test
	valgrind --leak-check=full ./$(TEST_OUTPUT)

install:
	mkdir build
	cd view && qmake && make && make clean && rm Makefile && mv view ../build
	cd build && mv view $(APPNAME)

uninstall:
	rm -rf build

dvi:
	cd $(DOC_PATH) && doxygen Doxyfile && cd ..
	open $(DOC_PATH)/html/index.html

dist: install
	rm -rf $(ARCHIVE)/
	mkdir $(ARCHIVE)/
	mkdir $(ARCHIVE)/src
	cp -r ./build/$(APPNAME) $(ARCHIVE)/src/
	tar cvzf $(ARCHIVE).tgz $(ARCHIVE)/
	rm -rf $(ARCHIVE)/

clang:
	clang-format -style=google -n $(ALL_SRC)
	clang-format -style=google -i $(ALL_SRC)
