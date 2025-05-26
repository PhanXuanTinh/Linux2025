#Define
CXX = gcc
CXXFLAG = -Wall

#Folder
SRC_DIR = src
BUILD_DIR = BUILD

#Source file
MAIN_SRC = $(SRC_DIR)/main.c

# Build main process
all: $(MAIN_SRC)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAG) -o $(BUILD_DIR)/app $(MAIN_SRC)

# Run with environment variable
run: $(MAIN_SRC) #how to run: make run ACTION=1 or 2
	@echo "Run with environment variable = $(ACTION)"
	ACTION=$(ACTION) ./$(BUILD_DIR)/app



clean:
	rm -rf $(BUILD_DIR)