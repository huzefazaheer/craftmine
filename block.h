#include "config.h"

enum Material{
    STONE,
    GRASS,
    DIRT,
    COARSE_DIRT,
    WOOD,
    LEAF,
    TNT
};

class Block{
    private:
    glm::vec4 frontTextureVec;
    glm::vec4 backTextureVec;
    glm::vec4 leftTextureVec;
    glm::vec4 rightTextureVec;
    glm::vec4 topTextureVec;
    glm::vec4 bottomTextureVec;

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

    public:
    glm::vec3 position;

    Block(){
        setTextures(0.875f,0.25);
        position = glm::vec3(0, 0, 0);
    }

    void setMaterial(Material material){
        if (material == STONE){
            setTextures(0.375f, 0);
        }else if(material == GRASS){
            setSideTextures(0.125f, 0);
            setTopTexture(0, 0);
        }else if(material == WOOD){
            setSideTextures(0.75f, 0);
            setTopTexture(0.875f,0);
        }else if(material == DIRT){
            setTextures(0.25f, 0);
        }else if(material == COARSE_DIRT){
            setTextures(0.375f, 0.25);
        }else if (material == LEAF){
            setTextures(0, 0.25);
        }else setTextures(0.875f,0.25);
        
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