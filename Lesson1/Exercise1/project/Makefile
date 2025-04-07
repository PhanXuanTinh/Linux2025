# Compiler and flag
CXX = g++
CXXFLAG = -Wall -fPIC

# Folder
SRC_DIR = src
BUILD_DIR = BUILD

# Source file
LIB_SRC = $(SRC_DIR)/library.cpp
MAIN_SRC = $(SRC_DIR)/main.cpp

# Output
STATIC_LIB = $(BUILD_DIR)/libStaticLibrary.a
SHARED_LIB = $(BUILD_DIR)/libSharedLibrary.so
EXECUTABLE = $(BUILD_DIR)/exeApp

# Build Rule
all: $(STATIC_LIB) $(SHARED_LIB) $(EXECUTABLE)

# Create static lib
$(STATIC_LIB): $(LIB_SRC)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAG) -c $< -o $(BUILD_DIR)/library.o
	ar rcs $@ $(BUILD_DIR)/library.o

# Create shared lib
$(SHARED_LIB): $(LIB_SRC)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAG) -shared -o $@ $<

# Create executable file
$(EXECUTABLE): $(MAIN_SRC) $(SHARED_LIB)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAG) $< -L$(BUILD_DIR) -Wl,-rpath=$(BUILD_DIR) -lSharedLibrary -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)
