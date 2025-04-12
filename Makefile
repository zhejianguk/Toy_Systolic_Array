CXX = g++
CXXFLAGS = -O2

SOURCES = main.cpp Matrix.cpp SystolicArray.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET  = main.elf

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
