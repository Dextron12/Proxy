#version 410 core

out vec4 FragColour;
uniform vec4 ourColour;

void main(){
    FragColour = ourColour;
}