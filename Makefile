SRC  = src/main.cpp src/glad.c
INC = -I"C:\Programming\C++\Libraries\OpenGL\glad\include" -I"C:\Programming\C++\Libraries\OpenGL\glfw\include" -L"C:\Programming\C++\Libraries\OpenGL\glfw\lib" 
LIB =  -lgdi32 -lopengl32 -lglfw3dll
OUT = main

build: $(SRC)
	@g++ $(SRC) $(INC) $(LIB) -o$(OUT)
	@$(OUT).exe