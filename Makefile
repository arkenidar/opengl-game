all: produce_executable execute_executable

produce_executable:
ifeq (Linux,$(shell uname))
	g++ -o app -g -Wall src/*.cpp $(shell sdl2-config --cflags --libs) -lGL -lGLU
else
	g++ -o app -g -Wall src/*.cpp $(shell sdl2-config --cflags --libs) -lopengl32 -lglu32 -mwindows
endif

execute_executable:
	./app

clean:
	rm app*
