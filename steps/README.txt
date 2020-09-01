These are the steps in the creation of main.cpp, so I can use this as a reference later.
The content of the steps is described below.
To compile any of these, they need to replace main.cpp in the parent directory and then just "make test"

1_window.cpp:
    Create a GLFW window and listen to ESC keypress to quit.
    Make opengl context using GLFW.
    Initialize GLAD to access OpenGL functions.
    glClear the window and keep that render cycle going.
    Respond to window resize by adjusting the viewport accordingly.

2_triangle.cpp:
    Compile and link shaders to make shader program.
    Load triangle vertices into VBO and setup a VAO.
    Draw triangle in screen.
    " Usually when you have multiple objects you want to draw,
      you first generate/configure all the VAOs (and thus the required VBO and attribute pointers)
      and store those for later use.
      The moment we want to draw one of our objects, we take the corresponding VAO, bind it, 
      then draw the object and unbind the VAO again. "

3_ebo.cpp:
    Start using EBO to draw object by referring to vertex indices rather than by referring to vertices.
    Add some cleanup stuff that I forgot about in step 2.
