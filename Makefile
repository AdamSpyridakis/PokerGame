CXX = g++
CXXFLAGS = -std=c++23

SRCS = main.cpp 
SRCS += Dealer\Dealer.cpp
SRCS += Common\CommonTypes.cpp
SRCS += Common\Logging.cpp
SRCS += GameLogic\GameLogic.cpp
SRCS += GameLogic\HandCalculator.cpp
SRCS += Player\Player.cpp

OBJS = $(SRCS:.cpp=.o)

TARGET = PokerBot

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
	del $(OBJS)

clean:
	del PokerBot.exe
