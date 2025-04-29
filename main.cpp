#include "config.h"
#include "camera.h"

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

bool wireframeMode = false;
Camera gameCam;
float deltaTime = 0.0f;

float lastFrame = 0.0f;
void getDeltaTime(){
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    static bool zKeyPressed = false;
    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        if(!zKeyPressed) {
            zKeyPressed = true;
            wireframeMode = !wireframeMode;
            
            // Set polygon mode based on wireframe state
            if(wireframeMode) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
    } else {
        zKeyPressed = false;
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        gameCam.handleForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gameCam.handleBackward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gameCam.handleLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gameCam.handleRight(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    gameCam.handleUp(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    gameCam.handleDown(deltaTime);
}

float lastX = 400, lastY = 300;
const float sensitivity = 0.05f;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse)
    {
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
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!";
        return -1;
    }

    // Configure GLFW window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(mode -> width, mode -> height, "Proj", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window!";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);

    // No cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Enable z testing
    glEnable(GL_DEPTH_TEST);

    // Initialize GLEW
    if(glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!";
        return -1;
    }

    Shader shader1("shaders/shader.vs", "shaders/shader.fs");

    // Set up vertex data and buffers
    float vertices[] = {
        // Positions          // Colors (R,G,B)      // Face
        // Front face (Red)
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    // 0
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    // 1
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    // 2
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,    // 3
    
        // Back face (Green)
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,    // 4
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,    // 5
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,    // 6
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,    // 7
    
        // Left face (Blue)
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,    // 8
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,    // 9
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,    // 10
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,    // 11
    
        // Right face (Yellow)
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,    // 12
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,    // 13
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,    // 14
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,    // 15
    
        // Top face (Cyan)
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,    // 16
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,    // 17
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,    // 18
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,    // 19
    
        // Bottom face (Magenta)
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,    // 20
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,    // 21
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,    // 22
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f     // 23
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

    // Bind VAO first
    glBindVertexArray(VAO);

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind and set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configure vertex position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Configure vertex color attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VBO (but keep EBO bound!)
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind VAO (this also unbinds the EBO)
    glBindVertexArray(0);

    // Main loop
    while(!glfwWindowShouldClose(window)) {
        getDeltaTime();
        processInput(window);
        
        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        
        // Draw our two triangles
        shader1.use();

        // Create translations
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        //spin model relative to global position
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        //deals with prespective
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shader1.ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(shader1.ID, "view");
        unsigned int projectionLoc  = glGetUniformLocation(shader1.ID, "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(gameCam.getViewMatrix()));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
        
    glfwTerminate();
    return 0;
}