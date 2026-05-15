GTEST_DIR := googletest/googletest
GTEST_INC := -I$(GTEST_DIR)/include -I$(GTEST_DIR)
GTEST_SRC := $(GTEST_DIR)/src/gtest-all.cc
GTEST_MAIN_SRC := $(GTEST_DIR)/src/gtest_main.cc

main:
	g++ --std=c++20 -Ofast ./vector/*.cpp -o main

dev:
	g++ --std=c++20 -Og ./vector/*.cpp -o main

tests:
	g++ --std=c++20 -Og $(GTEST_INC) \
		./vector/studentas.cpp \
		./vector/zmogus.cpp \
		./vector/apdorojimas.cpp \
		./vector/isvestis.cpp \
		./vector/ivestis.cpp \
		./vector/random.cpp \
		./vector/test.cpp \
		./gtest.cpp \
		./$(GTEST_SRC) \
		./$(GTEST_MAIN_SRC) \
		-lpthread \
		-o tests

test: tests
	./tests

testV:
	g++ --std=c++20 -Og $(GTEST_INC) \
		./gtest_vector.cpp \
		./$(GTEST_SRC) \
		./$(GTEST_MAIN_SRC) \
		-lpthread \
		-o testv

testVector: testV
	./testv

clean:
	rm -f main tests *.o