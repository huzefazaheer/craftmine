class Camera {
    private:
        const float MOVEMENTSPEED = 4.0f;
        float yaw = -90.0f;
        float pitch = -60.0f;
    
    public:
        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;
        glm::vec3 cameraBack;
        glm::vec3 direction;

        Camera() {
            cameraPos = glm::vec3(0.0f, 20.0f, 5.0f);
            cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
            cameraBack = glm::vec3(-1.0f, 0.0f, 0.0f);

            calculateDirection();

            cameraFront = glm::normalize(direction);
        }

        void calculateDirection(){
            direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));  // X-axis direction
            direction.y = sin(glm::radians(pitch));                           // Y-axis (up/down) direction
            direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));  // Z-axis direction

        }
    
        void handleForward(float deltaTime, int modifier) {
            glm::vec3 camF = cameraFront;
            camF.y = 0.0f;
            cameraPos += modifier * MOVEMENTSPEED * deltaTime * camF;
        }   
    
        void handleBackward(float deltaTime, int modifier) {
            glm::vec3 camF = cameraFront;
            camF.y = 0.0f;
            cameraPos -= modifier * MOVEMENTSPEED * deltaTime * camF;
        }
    
        void handleLeft(float deltaTime) {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * MOVEMENTSPEED * deltaTime;
        }
    
        void handleRight(float deltaTime) {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * MOVEMENTSPEED * deltaTime;
        }

        void handleUp(float deltaTime, int modifier) {
            cameraPos +=  modifier *MOVEMENTSPEED * deltaTime * cameraUp;
        }

        void handleDown(float deltaTime, int modifier) {
            cameraPos -= modifier * MOVEMENTSPEED * deltaTime * cameraUp;
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