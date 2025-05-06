    #include "config.h"
    #include "camera.h"
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"
    #include "PerlinNoise.hpp"
    #include <vector>
    #include <filesystem>
    
    const unsigned int SCR_WIDTH = 1600;
    const unsigned int SCR_HEIGHT = 1200;

    int speedModifier = 1;
    
    bool wireframeMode = false;
    Camera gameCam;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    void getDeltaTime() {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }
    
    void processInput(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    
        static bool zKeyPressed = false;
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            if (!zKeyPressed) {
                zKeyPressed = true;
                wireframeMode = !wireframeMode;
    
                // Set polygon mode based on wireframe state
                if (wireframeMode) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                } else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
        } else {
            zKeyPressed = false;
        }
    
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            gameCam.handleForward(deltaTime, speedModifier);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            gameCam.handleBackward(deltaTime, speedModifier);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            gameCam.handleLeft(deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            gameCam.handleRight(deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            gameCam.handleUp(deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            gameCam.handleDown(deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            speedModifier == 1 ? speedModifier = 6 : speedModifier = 1;
    }   
    
    float lastX = 400, lastY = 300;
    const float sensitivity = 0.05f;
    bool firstMouse = true;
    void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
    
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; 
        lastX = xpos;
        lastY = ypos;
        float yaw = gameCam.getYaw();
        float pitch = gameCam.getPitch();
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        yaw   += xoffset;
        pitch += yoffset;  
        if(pitch > 89.0f)
            pitch =  89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;
        gameCam.setPitch(pitch);
        gameCam.setYaw(yaw);
    }
    
    int main() {
        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            return -1;
        }
    
        // Configure GLFW window
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    
        GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Proj", monitor, NULL);
        if (!window) {
            std::cerr << "Failed to create GLFW window!" << std::endl;
            glfwTerminate();
            return -1;
        }
    
        glfwMakeContextCurrent(window);
        glfwSetCursorPosCallback(window, mouse_callback);
    
        // Set the background color to a light sky blue color (RGBA)
glClearColor(0.529f, 0.808f, 0.922f, 1.0f);  // Light sky blue


        // No cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
        // Enable z testing
        glEnable(GL_DEPTH_TEST);
    
        // Initialize GLEW
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW!" << std::endl;
            return -1;
        }
        

    
        Shader shader1("shaders/shader.vs", "shaders/shader.fs");

        // Load Texture
        unsigned int stoneTexture, grassTexture, dirtTexture;
        glGenTextures(1, &stoneTexture);
        glBindTexture(GL_TEXTURE_2D, stoneTexture);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load the image using stb_image
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);  // Flip the texture vertically
        unsigned char *data = stbi_load("./textures/stone.png", &width, &height, &nrChannels, 0);  // Change this path if necessary
        if (data) {
            if (nrChannels == 3) {
                // If the image has 3 channels (RGB), use GL_RGB
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            } else if (nrChannels == 4) {
                // If the image has 4 channels (RGBA), use GL_RGBA
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            } else {
                std::cerr << "Unsupported number of channels!" << std::endl;
                std::cerr << nrChannels;
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "Failed to load texture!" << std::endl;
        }
        stbi_image_free(data);
        glGenTextures(1, &grassTexture);
        glBindTexture(GL_TEXTURE_2D, grassTexture);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load the image using stb_image
        int width1, height1, nrChannels1;
        stbi_set_flip_vertically_on_load(true);  // Flip the texture vertically
        unsigned char *data1 = stbi_load("./textures/grass.png", &width1, &height1, &nrChannels1, 0);  // Change this path if necessary
        if (data1) {
            if (nrChannels1 == 3) {
                // If the image has 3 channels (RGB), use GL_RGB
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
            } else if (nrChannels1 == 4) {
                // If the image has 4 channels (RGBA), use GL_RGBA
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
            } else {
                std::cerr << "Unsupported number of channels!" << std::endl;
                std::cerr << nrChannels1;
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "Failed to load texture!" << std::endl;
        }
        stbi_image_free(data1);
        glGenTextures(1, &dirtTexture);
        glBindTexture(GL_TEXTURE_2D, dirtTexture);
        int width2, height2, nrChannels2;
        stbi_set_flip_vertically_on_load(true);  // Flip the texture vertically
        unsigned char *data2 = stbi_load("./textures/dirt.png", &width2, &height2, &nrChannels2, 0);  // Change this path if necessary
        if (data2) {
            if (nrChannels2 == 3) {
                // If the image has 3 channels (RGB), use GL_RGB
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
            } else if (nrChannels2 == 4) {
                // If the image has 4 channels (RGBA), use GL_RGBA
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
            } else {
                std::cerr << "Unsupported number of channels!" << std::endl;
                std::cerr << nrChannels2;
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "Failed to load texture!" << std::endl;
        }
        stbi_image_free(data2);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    
        float vertices[] = {
            // Positions              // Colors (R,G,B)         // Texture Coordinates (u,v)
            // Front face (Red)
            -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // 0
            0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // 1
            0.5f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 2
            -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // 3
        
            // Back face (Green)
            0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 4
            -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // 5
            -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // 6
            0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // 7
        
            // Left face (Blue)
            -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 8
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // 9
            -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // 10
            -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // 11
        
            // Right face (Yellow)
            0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // 12
            0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 13
            0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // 14
            0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // 15
        
            // Top face (Cyan)
            -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // 16
            0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // 17
            0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 1.0f,   1.0f, 1.0f,   // 18
            -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 1.0f,   // 19
        
            // Bottom face (Magenta)
            -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // 20
            0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // 21
            0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // 22
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   0.0f, 0.0f    // 23
        };
        
        
    
        unsigned int indices[] = {
            // Front face
            0, 1, 2,
            2, 3, 0,
    
            // Back face
            4, 5, 6,
            6, 7, 4,
    
            // Left face
            8, 9, 10,
            10, 11, 8,
    
            // Right face
            12, 13, 14,
            14, 15, 12,
    
            // Top face
            16, 17, 18,
            18, 19, 16,
    
            // Bottom face
            20, 21, 22,
            22, 23, 20
        };
    
        unsigned int VBO, VAO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

    
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Set up a list of positions where cubes will be placed
        const siv::PerlinNoise::seed_type seed = 123456u;
	    const siv::PerlinNoise perlin{ seed };
        std::vector<glm::vec3> cubePositions = {
            glm::vec3(0.0f, 0.0f, 0.0f),
        };
    
        //DELETE
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {  
                // Adjust frequency and amplitude for smoother transitions across a 100x100 area
                const float e = 
                perlin.octave2D_01((i * 0.02), (j * 0.02), 8) +            // Medium features: still smooth, but more variation
                perlin.octave2D_01((i * 0.1), (j * 0.1), 16) +         // Fine details: subtle changes
                perlin.octave2D_01((i * 0.3), (j * 0.3), 32);          // Very fine details: small but smooth transition

                // Apply smoothing
                float smoothedHeight = round((e + 1.0) * 5.0); // Scale to range [0, 10] with gentle offset

                for (int k = static_cast<int>(smoothedHeight); k > 8    ; k-- ){
                    cubePositions.push_back(glm::vec3(i, k, j));
                }
            }
        }

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW); 

        // Main loop
        while (!glfwWindowShouldClose(window)) {
            getDeltaTime();
            processInput(window);
    
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shader1.use();
    
            glm::mat4 projection = glm::perspective(glm::radians(62.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            unsigned int projectionLoc = glGetUniformLocation(shader1.ID, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
            unsigned int viewLoc = glGetUniformLocation(shader1.ID, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(gameCam.getViewMatrix()));
    
            // Render each cube at its position
            for (const glm::vec3& position : cubePositions) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, position);
    
                unsigned int modelLoc = glGetUniformLocation(shader1.ID, "model");
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
                if(position.y > 11){
                    if(position.y >= 13){
                        glBindTexture(GL_TEXTURE_2D, grassTexture);
                    }else glBindTexture(GL_TEXTURE_2D, dirtTexture);
                }else glBindTexture(GL_TEXTURE_2D, stoneTexture);
                
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
    
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    
        glfwTerminate();

        return 0;
    }
    