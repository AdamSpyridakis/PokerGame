CXX = g++
CXXFLAGS = -std=c++11

SRCS = main.cpp Dealer/dealer.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = PokerBot

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	del main.o
	del dealer.o
	del PokerBot.exe