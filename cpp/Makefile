CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

SRCS = main.cpp ReservationSystem.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = reservation

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
