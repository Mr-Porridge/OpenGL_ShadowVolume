#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
//新加
in vec3 Normal;
in vec3 Position;

uniform sampler2D texture_diffuse1;
uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{    
    //新加
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    //新加
    //FragColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}