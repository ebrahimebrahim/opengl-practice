INCLUDES = -Ilibs/glad/include
CFLAGS = -std=c++17 -O2 -Wall $(INCLUDES)
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -ldl
EXECUTABLE_NAME = opengltest
SRCDIR = src

main.o: $(SRCDIR)/main.cpp
	g++ $(CFLAGS) -o main.o -c $(SRCDIR)/main.cpp $(LDFLAGS)

glad.o: libs/glad/src/glad.c
	g++ $(CFLAGS) -o glad.o -c libs/glad/src/glad.c $(LDFLAGS)

Shader.o: $(SRCDIR)/Shader.cpp
	g++ $(CFLAGS) -o Shader.o -c $(SRCDIR)/Shader.cpp $(LDFLAGS)

$(EXECUTABLE_NAME): main.o glad.o Shader.o
	g++ $(CFLAGS) -o $(EXECUTABLE_NAME) main.o glad.o Shader.o $(LDFLAGS)


.PHONY: test clean

test: $(EXECUTABLE_NAME)
	./$(EXECUTABLE_NAME)

clean:
	rm -f main.o
	rm -f glad.o
	rm -f Shader.o
	rm -f $(EXECUTABLE_NAME)
