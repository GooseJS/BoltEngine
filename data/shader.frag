#version 330

in vec3 worldPos;
in vec3 outLightPos;
in vec3 outTexCoord;
out vec4 color;

uniform sampler2DArray texAtlas;

void main()
{

	color = vec4(texture(texAtlas, outTexCoord).xyz, 1.0);
	vec3 worldNormal = cross(dFdx(worldPos.xyz), dFdy(worldPos.xyz));
	float ambient = 0.3;
	worldNormal = normalize(worldNormal);
	vec3 lightDir = normalize(outLightPos + worldPos);
	float diff = max(dot(worldNormal, lightDir), 0.0);
	vec4 result = (ambient + diff) * color;
	color = color;
}