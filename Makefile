SRC = src/main.cpp C:\Users\ethan\Programming\C++\Libraries\OpenGL\glad\src\glad.c
INC = -I"C:\Users\ethan\Programming\C++\Libraries\OpenGL\glfw_x64\include" -L"C:\Users\ethan\Programming\C++\Libraries\OpenGL\glfw_x64\lib" -I"C:\Users\ethan\Programming\C++\Libraries\OpenGL\glad\include" -I"C:\Users\ethan\Programming\C++\Libraries\OpenGL\glm" -I"C:\Users\ethan\Programming\C++\Proxy\include"
LIB = -lmingw32 -lgdi32 -lopengl32 -lglfw3dll
OUT = main

build: $(SRC)
	@g++ $(SRC) $(INC) $(LIB) -o $(OUT)
	@$(OUT).exe