class Camera {
    private:
        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;
        glm::vec3 cameraBack;
        glm::vec3 direction;
        const float MOVEMENTSPEED = 3.5f;
        float yaw = -90.0f;
        float pitch = 0.0f;
    
    public:
        Camera() {
            cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
            cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
            cameraBack = glm::vec3(-1.0f, 0.0f, 0.0f);

            calculateDirection();

            cameraFront = glm::normalize(direction);
        }

        void calculateDirection(){
            direction.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
            direction.y = sin(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw) * cos(glm::radians(pitch)));
        }
    
        void handleForward(float deltaTime) {
            glm::vec3 camF = cameraFront;
            camF.y = 0.0f;
            cameraPos += MOVEMENTSPEED * deltaTime * camF;
        }   
    
        void handleBackward(float deltaTime) {
            glm::vec3 camF = cameraFront;
            camF.y = 0.0f;
            cameraPos -= MOVEMENTSPEED * deltaTime * camF;
        }
    
        void handleLeft(float deltaTime) {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * MOVEMENTSPEED * deltaTime;
        }
    
        void handleRight(float deltaTime) {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * MOVEMENTSPEED * deltaTime;
        }

        void handleUp(float deltaTime) {
            cameraPos += MOVEMENTSPEED * deltaTime * cameraUp;
        }

        void handleDown(float deltaTime) {
            cameraPos -= MOVEMENTSPEED * deltaTime * cameraUp;
        }

        void setYaw(float yaw1){
            yaw = yaw1;
        }

        void setPitch(float pitch1){
            pitch = pitch1;
        }

        float getYaw(){
            return yaw;
        }

        float getPitch(){
            return pitch;
        }
    
        glm::mat4 getViewMatrix() {
            calculateDirection();
            cameraFront = glm::normalize(direction);
            return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        }
    };