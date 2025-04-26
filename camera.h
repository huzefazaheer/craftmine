class Camera {
    private:
        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;
        const float MOVEMENTSPEED = 0.0005f;
    
    public:
        Camera() {
            cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
            cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
            cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        }
    
        void handleForward(float deltaTime) {
            cameraPos += MOVEMENTSPEED * deltaTime * cameraFront;
        }
    
        void handleBackward(float deltaTime) {
            cameraPos -= MOVEMENTSPEED * deltaTime * cameraFront;
        }
    
        void handleLeft(float deltaTime) {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * MOVEMENTSPEED * deltaTime;
        }
    
        void handleRight(float deltaTime) {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * MOVEMENTSPEED * deltaTime;
        }
    
        glm::mat4 getViewMatrix() {
            return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        }
    };