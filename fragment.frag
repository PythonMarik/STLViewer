#version 330 core
out vec4 FragColor;
in vec3 prettyColor;

void main(void)
{
    FragColor = vec4(prettyColor, 1.0f);
}
