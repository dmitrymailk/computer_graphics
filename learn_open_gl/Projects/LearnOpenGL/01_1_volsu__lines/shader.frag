#version 330 core
in vec3 outColor;
out vec4 FragColor;

void main()
{
	// FragColor = vec4(1.0f, 0.3f, 0.1f, 1.0f);
	FragColor = vec4(outColor.x, outColor.y, outColor.z, 1.0f);
}