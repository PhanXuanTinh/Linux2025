#Define
CXX = gcc
CXXFLAG = -Wall

#Folder
SRC_DIR = src
BUILD_DIR = BUILD

#Source file
MAIN_SRC = $(SRC_DIR)/main.c

all: $(MAIN_SRC)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAG) -o $(BUILD_DIR)/app $(MAIN_SRC)

clean:
	rm -rf $(BUILD_DIR)