#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;
in vec3 lightPos;
in vec3 camPos;


uniform sampler2D diffuse0;
uniform sampler2D normal0;
uniform vec4 lightColor;



vec4 pointLight()
{	
	vec3 lightVec = lightPos - crntPos;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float a = 1.00f;
	float b = 0.70f;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.05f;

	// diffuse lighting
	vec3 normal = normalize(texture(normal0, texCoord).xyz * 2.0f - 1.0f);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specular = 0.0f;
	if (diffuse != 0.0f)
	{
		float specularLight = 0.50f;
		vec3 viewDirection = normalize(camPos - crntPos);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
		specular = specAmount * specularLight;
	};

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient)) * lightColor;
}


void main()
{
	FragColor = pointLight();
}