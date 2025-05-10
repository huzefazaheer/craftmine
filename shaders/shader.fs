   #version 330 core
   in vec3 vertexColor;
   in vec2 texCoord;
   out vec4 FragColor;
   uniform sampler2D gameTexture;
   void main() {
      FragColor = texture(gameTexture, texCoord);
      if(FragColor.a < 0.1) // Adjust threshold as needed
         discard;
   }