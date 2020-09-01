CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -ldl
EXECUTABLE_NAME = opengltest

main.o: main.cpp
	g++ $(CFLAGS) -o main.o -c main.cpp $(LDFLAGS)

glad.o: glad/src/glad.c
	g++ $(CFLAGS) -o glad.o -c glad/src/glad.c $(LDFLAGS)

$(EXECUTABLE_NAME): main.o glad.o
	g++ $(CFLAGS) -o $(EXECUTABLE_NAME) main.o glad.o $(LDFLAGS)
	

.PHONY: test clean

test: $(EXECUTABLE_NAME)
	./$(EXECUTABLE_NAME)

clean:
	rm -f main.o
	rm -f glad.o
	rm -f $(EXECUTABLE_NAME)
