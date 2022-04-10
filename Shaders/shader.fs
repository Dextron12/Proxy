#version 410 core

in vec3 ourPosition;

out vec4 FragColour;
uniform vec4 ourColour;

void main(){
    FragColour = vec4(ourPosition, 1.0);
}