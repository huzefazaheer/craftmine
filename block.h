#include "config.h"

class Block{
    private:
    glm::vec4 textureVec;

    public:
    Block(){
    }

    // Set coords of texture in atlas where x and y are coords
    void setTexture(float x, float y){
        textureVec = glm::vec4(x, y,  0.25f, 0.125f);
    }

    glm::vec4 getTexture() const{
        return textureVec;
    }
};