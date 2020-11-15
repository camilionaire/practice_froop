# this is a makefile, based off of another makefile.
# based off Massey's Makefile and Huidong's
# November 13, 2020
# Camilo Schaser-Hughes

CXX = g++
CXXFLAGS = -g -Wall -Werror
TARGET = frupal
OBJS = main.o hero.o item.o
LIBS = -lncurses

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

main.o: main.cpp

hero.o: hero.cpp

item.o: item.cpp
# the -sign before rm just means to ignore any errors.
# I removed it though
clean:
	rm -f $(TARGET)
	rm -f *.o
