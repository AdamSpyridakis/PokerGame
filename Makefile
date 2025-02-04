CXX = g++
CXXFLAGS = -std=c++11

CONSOLE_PRINT ?= 0
CXXFLAGS += -DCONSOLE_PRINT=$(CONSOLE_PRINT)

SRCS = main.cpp 
SRCS += Dealer\dealer.cpp
SRCS += commonTypes.cpp
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