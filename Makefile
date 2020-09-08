INCLUDES = -Ilibs/glad/include -Ilibs/catch -Ilibs/stb_image
CFLAGS = -std=c++17 -O2 $(INCLUDES)
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -ldl
EXECUTABLE_NAME = opengltest
TEST_EXECUTABLE_NAME = $(EXECUTABLE_NAME)_test
SRCDIR = src

# Object files for application other than main.o
OBJ_FILES = glad.o Shader.o stb_image.o

# Object files for testing other than main_test.o
OBJ_FILES_TESTING = shader_test.o


# --- Application targets and executable ---

main.o: $(SRCDIR)/main.cpp
	g++ $(CFLAGS) -Wall -o main.o -c $(SRCDIR)/main.cpp $(LDFLAGS)

glad.o: libs/glad/src/glad.c
	g++ $(CFLAGS) -o glad.o -c libs/glad/src/glad.c $(LDFLAGS)

Shader.o: $(SRCDIR)/Shader.cpp
	g++ $(CFLAGS) -Wall -o Shader.o -c $(SRCDIR)/Shader.cpp $(LDFLAGS)

stb_image.o: libs/stb_image/stb_image.cpp
	g++ $(CFLAGS) -o $@ -c $^

$(EXECUTABLE_NAME): $(OBJ_FILES) main.o
	g++ $(CFLAGS) -o $(EXECUTABLE_NAME) $(OBJ_FILES) main.o $(LDFLAGS)

# --------------------------------------------


# --- Unit test targets and executable ---

main_test.o: tests/main.cpp
	g++ $(CFLAGS) -Isrc -o main_test.o -c tests/main.cpp $(LDFLAGS)

shader_test.o: tests/shader_test.cpp
	g++ $(CFLAGS) -Isrc -o $@ -c $^ $(LDFLAGS)

$(TEST_EXECUTABLE_NAME): $(OBJ_FILES) $(OBJ_FILES_TESTING) main_test.o
	g++ $(CFLAGS) -o $(TEST_EXECUTABLE_NAME) main_test.o $(OBJ_FILES) $(OBJ_FILES_TESTING) $(LDFLAGS)

# --------------------------------------------


.PHONY: run clean test

run: $(EXECUTABLE_NAME)
	./$(EXECUTABLE_NAME)

test: $(TEST_EXECUTABLE_NAME)
	./$(TEST_EXECUTABLE_NAME)

clean:
	rm -f *.o
	rm -f $(TEST_EXECUTABLE_NAME)
	rm -f $(EXECUTABLE_NAME)
