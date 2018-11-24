cflags = -c -std=c++1z -Wall
lflags = 
libs = -lconfig++


all: main

run: all
	main



main: main.o menu.o Menu.o rwcfg.o rwdata.o
	g++ $(lflags) -o main main.o menu.o Menu.o rwcfg.o rwdata.o $(libs) 



main.o: main.cpp
	g++ $(cflags) main.cpp

menu.o: menu.cpp
	g++ $(cflags) menu.cpp

Menu.o: Menu.cpp
	g++ $(cflags) Menu.cpp

rwcfg.o: rwcfg.cpp
	g++ $(cflags) rwcfg.cpp

rwdata.o: rwdata.cpp
	g++ $(cflags) rwdata.cpp



# stat
stat: mainstat.cpp
	g++ -std=c++1z -Wall -o stat mainstat.cpp





clean:
	-rm *.o main stat





# end
