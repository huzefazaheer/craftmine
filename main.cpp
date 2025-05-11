    #include "config.h"
    #include "camera.h"
    #include "block.h"
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"
    #include "PerlinNoise.hpp"
    #include <vector>
    #include <filesystem>
    #include "raycaster.h"
    
    const unsigned int SCR_WIDTH = 1600;
    const unsigned int SCR_HEIGHT = 1200;

    int speedModifier = 1.8;
    int upModifier = 1;
    static bool leftMousePressed = false;
    static bool rightMousePressed = false;
    
    bool wireframeMode = false;
    Camera gameCam;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    void getDeltaTime() {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    std::vector<Block> Cubes;
    Material activeMat = DIRT;
    
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
            gameCam.handleUp(deltaTime, speedModifier);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            gameCam.handleDown(deltaTime, speedModifier);
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        speedModifier = 3;
        else
        speedModifier = 1.8;


        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            activeMat = DIRT;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
            activeMat = COBBLESTONE;
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
            activeMat = PLANK;
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
            activeMat = BRICK;
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
            activeMat = WOOD;


        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            if (!leftMousePressed) {
                leftMousePressed = true;
                
                // Perform ray cast
                glm::vec3 rayOrigin = gameCam.cameraPos;
                glm::vec3 rayDirection = gameCam.cameraFront;
                glm::vec3 hitNormal, hitPosition;
                
                Block* selectedBlock = rayCast(rayOrigin, rayDirection, Cubes, 5.0f, hitNormal, hitPosition);
                
                if (selectedBlock != nullptr) {
                    if(selectedBlock->getMaterial() != BEDROCK){
                        // Remove the block
                    auto it = std::find_if(Cubes.begin(), Cubes.end(), 
                    [selectedBlock](const Block& b) {
                        return b.position == selectedBlock->position;
                    });
                
                    if (it != Cubes.end()) {
                        Cubes.erase(it);
                    }
                    }
                }
            }
        } else {
            leftMousePressed = false;
        }


        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            if (!rightMousePressed) {
                rightMousePressed = true;
                
                // Perform ray cast
                glm::vec3 rayOrigin = gameCam.cameraPos;
                glm::vec3 rayDirection = gameCam.cameraFront;
                glm::vec3 hitNormal, hitPosition;
                
                Block* selectedBlock = rayCast(rayOrigin, rayDirection, Cubes, 5.0f, hitNormal, hitPosition);
                
                if (selectedBlock != nullptr) {
                    // Place new block adjacent to the hit face
                    glm::vec3 newBlockPos = selectedBlock->position + hitNormal;
                    
                    // Check if position is already occupied
                    bool positionOccupied = false;
                    for (const Block& b : Cubes) {
                        if (b.position == newBlockPos) {
                            positionOccupied = true;
                            break;
                        }
                    }
                    
                    if (!positionOccupied) {
                        Block placeBlock;
                        placeBlock.position = newBlockPos;
                        placeBlock.setMaterial(activeMat);
                        Cubes.push_back(placeBlock);
                    }
                }
            }
        } else {
            rightMousePressed = false;
        }
    
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
        

    
        Shader gameShader("shaders/shader.vs", "shaders/shader.fs");
        Shader crosshairShader("shaders/crosshair.vs", "shaders/crosshair.fs");


        // Load textures

        GLuint textureAtlas;
        glGenTextures(1, &textureAtlas);
        glBindTexture(GL_TEXTURE_2D, textureAtlas);

        int width, height, nrChannels;
        unsigned char *data = stbi_load("./textureatlas.png", &width, &height, &nrChannels, 0);

        if(data){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(textureAtlas);
        }else {
            std::cerr << "Failed to load texture atlas!" << std::endl;
        }
        stbi_image_free(data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        float aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;


// Crosshair size (base value)
float crosshairSize = 0.015f;
        // Simple crosshair vertices (centered, small size)
        float crosshairVertices[] = {
            // Horizontal line (corrected for aspect ratio)
            -crosshairSize,  0.0f,          0.0f, 0.5f,  // Left point
             crosshairSize,  0.0f,          1.0f, 0.5f,  // Right point
            
            // Vertical line (no aspect ratio correction needed)
             0.0f,                     -crosshairSize*aspectRatio, 0.5f, 0.0f,  // Bottom point
             0.0f,                      crosshairSize*aspectRatio,  0.5f, 1.0f   // Top point
        };
        

        unsigned int crosshairIndices[] = {
            0, 1,  // Horizontal line
            2, 3   // Vertical line
        };

    
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

        // Crosshair VAO setup
        unsigned int crosshairVAO, crosshairVBO, crosshairEBO;
        glGenVertexArrays(1, &crosshairVAO);
        glGenBuffers(1, &crosshairVBO);
        glGenBuffers(1, &crosshairEBO);

        glBindVertexArray(crosshairVAO);

        glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, crosshairEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(crosshairIndices), crosshairIndices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        // Set up a list of positions where cubes will be placed
        const siv::PerlinNoise::seed_type seed = 123456u;
	    const siv::PerlinNoise perlin{ seed };

    
        //DELETE
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {  
                // Adjust frequency and amplitude for smoother transitions across a 100x100 area
                const float e = 
                perlin.octave2D_01((i * 0.01), (j * 0.01), 4) * 0.7 +  // Broad, smooth features (dominant)
                perlin.octave2D_01((i * 0.05), (j * 0.05), 8) * 0.3;   // Subtle medium features

                const float l = 
                perlin.octave2D_01((i * 0.01), (j * 0.03), 20) * 2; 
                perlin.octave2D_01((i * 0.03), (j * 0.03), 3) * 1;   // Subtle medium features  // Subtle medium features

                // Apply smoothing
                float smoothedHeight = round((e + 1.0) * 5.0 + (l * 7)); // Scale to range [0, 10] with gentle offset
                int height = static_cast<int>(smoothedHeight);

                // doo doo code to place trees
                int value = rand() % 2;
                if(i % 13 ==  0 && j % 11 == 0 && value == 0 && height >= 13){
                    int o = i + (rand() % 5) - 2;
                    int p = j + (rand() % 5) - 2;
                    if(p > 100 || p < 0 || o > 100 || o < 0) continue;
                    for (int g = height + 1; g < height + 6; g++){
                        Block block;
                        block.position.x = o;
                        block.position.z = p;
                        block.position.y = g;
                        block.setMaterial(WOOD);
                        Cubes.push_back(block);
                        if (g == height + 4){
                            block.setMaterial(LEAF);
                            block.position.z = p + 1;
                            Cubes.push_back(block);
                            block.position.z = p + 2;
                            Cubes.push_back(block);
                            block.position.z = p - 1;
                            Cubes.push_back(block);
                            block.position.z = p - 2;
                            Cubes.push_back(block);
                            block.position.z = p;
                            block.position.x = o + 1;
                            Cubes.push_back(block);
                            block.position.x = o + 2;
                            Cubes.push_back(block);
                            block.position.x = o - 1;
                            Cubes.push_back(block);
                            block.position.x = o - 2;
                            Cubes.push_back(block);
                            block.position.x = o + 1;
                            block.position.z = p + 1;
                            Cubes.push_back(block);
                            block.position.x = o - 1;
                            block.position.z = p - 1;
                            Cubes.push_back(block);
                            block.position.x = o + 1;
                            block.position.z = p - 1;
                            Cubes.push_back(block);
                            block.position.x = o - 1;
                            block.position.z = p + 1;
                            Cubes.push_back(block);
                            block.setMaterial(WOOD);
                        }else if (g == height + 5){
                            block.setMaterial(LEAF);
                            block.position.x = o + 1;
                            Cubes.push_back(block);
                            block.position.x = o - 1;
                            Cubes.push_back(block);
                            block.position.x = o;
                            block.position.z = p - 1;
                            Cubes.push_back(block);
                            block.position.z = p + 1;
                            Cubes.push_back(block);
                            block.setMaterial(WOOD);
                        }
                    }
                    Block block1;
                        block1.position.x = o;
                        block1.position.z = p;
                        block1.position.y = height + 6;
                        block1.setMaterial(LEAF);
                        Cubes.push_back(block1);
                    
                        
                }

                for (int k = height; k > 7 ; k-- ){
                    Block block;
                    if(k == height && height >= 13){
                        block.setMaterial(GRASS);
                    }
                    else if(k < 11){
                        block.setMaterial(STONE);
                    }else {
                        if ((i + j) % 11 == 0 && (k / 3 == 0 || k % 17 == 0)){
                            block.setMaterial(COARSE_DIRT);
                        }else block.setMaterial(DIRT);
                    }
                    block.position.x = i;
                    block.position.z = j;
                    block.position.y = k;
                    Cubes.push_back(block);
                }
                Block bedrock;
                bedrock.position.x = i;
                bedrock.position.z = j;
                bedrock.position.y = 7;
                bedrock.setMaterial(BEDROCK);
                Cubes.push_back(bedrock);
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
            gameShader.use();
    
            glm::mat4 projection = glm::perspective(glm::radians(65.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            unsigned int projectionLoc = glGetUniformLocation(gameShader.ID, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
            unsigned int viewLoc = glGetUniformLocation(gameShader.ID, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(gameCam.getViewMatrix()));

           
    
            // Render each cube at its position
            for (const Block& cube : Cubes) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cube.position);
    
                unsigned int modelLoc = glGetUniformLocation(gameShader.ID, "model");
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

                unsigned int texCoordsLoc = glGetUniformLocation(gameShader.ID, "uvTransform");

                glm::vec4 cubeT;

                glBindTexture(GL_TEXTURE_2D, textureAtlas);

                glBindVertexArray(VAO);

                cubeT = cube.getFrontTexture();
                glUniform4f(glGetUniformLocation(gameShader.ID, "uvTransform"), cubeT.x, cubeT.y, cubeT.a, cubeT.b);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(GLuint)));
                
                cubeT = cube.getBackTexture();
                glUniform4f(glGetUniformLocation(gameShader.ID, "uvTransform"), cubeT.x, cubeT.y, cubeT.a, cubeT.b);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));
                
                cubeT = cube.getLeftTexture();
                glUniform4f(glGetUniformLocation(gameShader.ID, "uvTransform"), cubeT.x, cubeT.y, cubeT.a, cubeT.b);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(GLuint)));
                
                cubeT = cube.getRightTexture();
                glUniform4f(glGetUniformLocation(gameShader.ID, "uvTransform"), cubeT.x, cubeT.y, cubeT.a, cubeT.b);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(GLuint)));
                
                cubeT = cube.getTopTexture();
                glUniform4f(glGetUniformLocation(gameShader.ID, "uvTransform"), cubeT.x, cubeT.y, cubeT.a, cubeT.b);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(24 * sizeof(GLuint)));
 
                cubeT = cube.getBottomTexture();
                glUniform4f(glGetUniformLocation(gameShader.ID, "uvTransform"), cubeT.x, cubeT.y, cubeT.a, cubeT.b);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(30 * sizeof(GLuint)));
                
                glBindVertexArray(0);
            }
            // Render crosshair (after 3D scene)
            glDisable(GL_DEPTH_TEST); // Disable depth test so it's always on top

            crosshairShader.use();
            glBindVertexArray(crosshairVAO);

glLineWidth(3.0f); // Set line width (default is 1.0)
            glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            glEnable(GL_DEPTH_TEST); // Re-enable depth test for next frame
    
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    
        glfwTerminate();

        return 0;
    }
    