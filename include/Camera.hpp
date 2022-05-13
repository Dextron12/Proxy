#ifndef CAMERA_HPP
#define CAMERA_HPP

//GLM headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

enum cameraMovement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


class Camera{
    private:
        bool firstMouse;
        double lastX, lastY;
        float pitch, yaw;

        //Camera controlable values:
        float Sensitivity = 0.1f;
        float Speed = 2.5f;
        float f_zoomCap = 45.0f;

        float Zoom;

        //Camera variables
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;


        //private functions
        void updateCameraVectors(){
            glm::vec3 direction;
            direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            direction.y = sin(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            
            Front = glm::normalize(direction);
        }

    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f)){
            Position = position;
            Front = front;
            Up = up;
            updateCameraVectors();
        }

        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float frontX, float frontY, float frontZ){
            Position = glm::vec3(posX, posY, posZ);
            Up = glm::vec3(upX, upY, upZ);
            Front = glm::vec3(frontX, frontY, frontZ);
            updateCameraVectors();
        }


        void updateKeyboard(cameraMovement direction, float deltaTime){

            float velocity = Speed * deltaTime;
            
            if (direction == FORWARD){
                Position += Front * velocity;
            }

            if (direction == BACKWARD){
                Position -= Front * velocity;
            }

            if (direction == LEFT){
                Position -= glm::normalize(glm::cross(Front, Up)) * (Speed * deltaTime);
            }

            if (direction == RIGHT){
                Position += glm::normalize(glm::cross(Front, Up)) * (Speed * deltaTime);
            }

        }

        void updateMouse(double xpos, double ypos){
            if (firstMouse){
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }

            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos;
            lastX = xpos;
            lastY = ypos;

            xoffset *= Sensitivity;
            yoffset *= Sensitivity;

            yaw += xoffset;
            pitch += yoffset;

            if (pitch > 89.0f){
                pitch = 89.0f;
            }
            if (pitch < -89.0f){
                pitch = -89.0f;
            }

            updateCameraVectors();
        }

        void updateZoom(double xoffset, double yoffset){
            Zoom -= (float) yoffset;

            if (Zoom < 1.0f){
                Zoom = 1.0f;
            }

            if (Zoom > f_zoomCap){
                Zoom = f_zoomCap;
            }
        }

    //Getters

    glm::mat4 GetViewMatrix(){
        return glm::lookAt(Position, Position + Front, Up);
    }

    float GetFov(){
        return Zoom;
    }



};


#endif