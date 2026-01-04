# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

# Executable name
TARGET = loadbalancer

# Source and object files
SRCS = main.cpp \
       RequestGenerator.cpp \
       RequestQueue.cpp \
       WebServer.cpp \
       LoadBalancer.cpp

OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile step
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Generate Doxygen documentation
docs:
	doxygen Doxyfile

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Clean everything including docs
cleanall: clean
	rm -rf docs
