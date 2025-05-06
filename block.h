#include "config.h"

class Block{
    private:
    glm::vec4 frontTextureVec;
    glm::vec4 backTextureVec;
    glm::vec4 leftTextureVec;
    glm::vec4 rightTextureVec;
    glm::vec4 topTextureVec;
    glm::vec4 bottomTextureVec;

    public:
    Block(){
    }

    // Set coords of texture in atlas where x and y are coords
    void setTextures(float x, float y){
        frontTextureVec = glm::vec4(x, y,  0.25f, 0.125f);
        backTextureVec = frontTextureVec;
        leftTextureVec = frontTextureVec;
        topTextureVec = frontTextureVec;
        rightTextureVec = frontTextureVec;
        bottomTextureVec = frontTextureVec;
    }

    void setSideTextures(float x, float y){
        leftTextureVec = glm::vec4(x, y,  0.25f, 0.125f);
        backTextureVec = leftTextureVec;
        frontTextureVec = leftTextureVec;
        rightTextureVec = leftTextureVec;
    }

    void setTopTexture(float x, float y){
        topTextureVec = glm::vec4(x, y,  0.25f, 0.125f);
    }

    void setBottomTexture(float x, float y){
        bottomTextureVec = glm::vec4(x, y,  0.25f, 0.125f);
    }

    glm::vec4 getFrontTexture() const {
        return frontTextureVec;
    }

    glm::vec4 getBackTexture() const {
        return backTextureVec;
    }

    glm::vec4 getTopTexture() const {
        return topTextureVec;
    }

    glm::vec4 getBottomTexture() const {
        return bottomTextureVec;
    }

    glm::vec4 getLeftTexture() const {
        return leftTextureVec;
    }

    glm::vec4 getRightTexture() const {
        return rightTextureVec;
    }

    
};