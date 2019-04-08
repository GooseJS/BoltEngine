#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 texCoord;
uniform mat4 projectionViewMatrix;
uniform mat4 translationMatrix;
uniform vec3 lightPos;
out vec3 worldPos;
out vec3 outLightPos;
out vec3 outTexCoord;

void main()
{

	gl_Position = (projectionViewMatrix * translationMatrix) * vec4(vertexPosition.xyz, 1.0);
	worldPos = (translationMatrix * vec4(vertexPosition.xyz, 1.0)).xyz;
	outLightPos = lightPos;
	outTexCoord = texCoord;

}