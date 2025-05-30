CXX = g++
CXXFLAGS = -std=c++11

SRCS = main.cpp 
SRCS += Dealer\dealer.cpp
SRCS += Common\commonTypes.cpp
SRCS += GameLogic\GameLogic.cpp
SRCS += Player\Player.cpp

OBJS = $(SRCS:.cpp=.o)

TARGET = PokerBot

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	del $(OBJS)
	del PokerBot.exe