#include "config.h"
#include "block.h"
#include "vector"

// Helper function to check ray-AABB intersection
bool rayIntersectsBlock(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, 
    const Block& block, float& intersectionDistance) {
glm::vec3 blockMin = block.getMin();
glm::vec3 blockMax = block.getMax();

float tMin = 0.0f;
float tMax = 100000.0f; // Maximum render distance

for (int i = 0; i < 3; ++i) {
if (fabs(rayDirection[i]) < 0.0001f) {
// Ray is parallel to this axis
if (rayOrigin[i] < blockMin[i] || rayOrigin[i] > blockMax[i]) {
return false;
}
} else {
float ood = 1.0f / rayDirection[i];
float t1 = (blockMin[i] - rayOrigin[i]) * ood;
float t2 = (blockMax[i] - rayOrigin[i]) * ood;

if (t1 > t2) std::swap(t1, t2);
if (t1 > tMin) tMin = t1;
if (t2 < tMax) tMax = t2;

if (tMin > tMax) return false;
}
}

intersectionDistance = tMin;
return true;
}

// Function to perform ray casting and find the selected block
Block* rayCast(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, 
std::vector<Block>& blocks, float maxDistance, 
glm::vec3& hitNormal, glm::vec3& hitPosition) {
Block* selectedBlock = nullptr;
float closestDistance = maxDistance;

for (Block& block : blocks) {
float distance;
if (rayIntersectsBlock(rayOrigin, rayDirection, block, distance)) {
if (distance < closestDistance) {
closestDistance = distance;
selectedBlock = &block;

// Calculate hit position and normal
hitPosition = rayOrigin + rayDirection * distance;

// Determine which face was hit
glm::vec3 blockCenter = block.position;
glm::vec3 delta = hitPosition - blockCenter;
float max = 0.0f;
float absDeltaX = fabs(delta.x);
float absDeltaY = fabs(delta.y);
float absDeltaZ = fabs(delta.z);

if (absDeltaX > absDeltaY && absDeltaX > absDeltaZ) {
hitNormal = glm::vec3(delta.x > 0 ? 1.0f : -1.0f, 0.0f, 0.0f);
} else if (absDeltaY > absDeltaZ) {
hitNormal = glm::vec3(0.0f, delta.y > 0 ? 1.0f : -1.0f, 0.0f);
} else {
hitNormal = glm::vec3(0.0f, 0.0f, delta.z > 0 ? 1.0f : -1.0f);
}
}
}
}

return selectedBlock;
}