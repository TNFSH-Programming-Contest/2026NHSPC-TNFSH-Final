CXXFLAGS=-std=gnu++14 -Wall -Wextra -Wshadow -O2 -static

.PHONY: all clean compile_outputs_list

all: manager.exe

manager.exe: manager.cpp
	g++ $(CXXFLAGS) manager.cpp -o manager.exe

clean:
	rm -f manager.exe

compile_outputs_list:
	@echo
