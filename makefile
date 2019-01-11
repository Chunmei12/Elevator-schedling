# this is a makefile of the c++ project uplay elevator
# the standard c++ compiler in the mac is $(command)
# written by chunmei on december 8th, 2019
app = Elevator
elevator = src/Elevator.cpp
elevatorDepth = src/Elevator.h src/MessageBus.h src/Elevators.h

elevators = src/Elevators.cpp
elevatorsDepth = src/MessageBus.h src/Elevators.h

humans = src/Humans.cpp
humansDepth = src/MessageBus.h src/Humans.h

threads = src/Threads.cpp 
threadsDepth = src/Threads.h

utils = src/Utils.cpp
utilsDepth = src/Utils.h

main = src/main.cpp
mainDepth = src/Elevators.h src/MessageBus.h src/Humans.h src/Threads.h

objFile = main.o Threads.o Elevator.o Elevators.o Humans.o  Utils.o

command = clang++ -std=c++11 -stdlib=libc++ -Wall 
# -Wextra -pedantic
edit : $(objFile)
	$(command) -o $(app) $(objFile)
Elevator.o : $(elevator) $(elevatorDepth)
	$(command) -c $(elevator)
Elevators.o : $(elevators) $(elevatorsDepth)
	$(command) -c $(elevators)
Humans.o : $(humans) $(humansDepth)
	$(command) -c $(humans)
Threads.o : $(threads) $(threadsDepth)
	$(command) -c $(threads)
Utils.o : $(utils) $(utilsDepth)
	$(command) -c $(utils)
main.o : $(main) $(mainDepth)
	$(command) -c $(main)
clean :
	rm $(app) $(objFile)