#version 330 core

out vec4 FragColor;

in vec2 texCoord;
// in vec3 Normal;
in vec3 tangentPos;
in vec3 Pos;
in vec3 lightPosition;
in vec3 cameraPos;

uniform sampler2D diffuse0;
uniform sampler2D normal0;

uniform vec4 lightColor;

uniform int lightType;
uniform vec3 lightPos;
uniform int normal_enabled;

vec4 pointLight()
{

	float a = 0.3f;
	float b = 0.7f;
	float dist = length(lightPosition - tangentPos);
	float intens = 1.0f / (a * dist * dist + b * dist + 1.0f);

	vec3 normal = texture(normal0, texCoord).rgb;
	normal = normalize(normal * 2.0f - 1.0f); // [0,1]->[-1,1]
	vec3 color = texture(diffuse0,texCoord).rgb;

	// diffuse
	vec3 lightDir = normalize(lightPosition - tangentPos);
	float diff = max(dot(lightDir, normal),0.0f);
	vec3 diffuse = diff*color;
	vec3 ambient = 0.1 * color;
	
	// specular
	vec3 viewDir = normalize(cameraPos - tangentPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = vec3(0.2) * spec;
	if(normal_enabled == 0)
		return vec4(color, 1.0f);
	else
		return vec4(diffuse + specular + ambient, 1.0f);
}


vec4 directionalLight()
{
	float ambient = 0.40f;
	// diffuse lighting
	vec3 normal = texture(normal0, texCoord).rgb;
	normal = normalize(normal * 2.0f - 1.0f); // [0,1]->[-1,1]
	vec3 lightDirection = normalize(lightPosition);
	float diff = max(dot(normal, lightDirection), 0.0f);

	float specular = 0.0f;
	if (diff != 0.0f)
	{
			float specularLight = 0.50f;
			vec3 viewDirection = normalize(cameraPos - tangentPos);
			vec3 reflectionDirection = normalize(reflect(-lightDirection, normalize(normal)));
			float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
			specular = specAmount * specularLight;
	};
	ambient = 0;
	return texture(diffuse0, texCoord) * lightColor * (diff + specular + ambient);
}

vec4 spotLight()
{
	float ambient = 0.05f;
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	vec3 normal = texture(normal0, texCoord).rgb;
	normal = normalize(normal * 2.0f - 1.0f); // [0,1]->[-1,1]
	vec3 lightDirection = normalize(lightPosition - tangentPos);
	float diff = max(dot(lightDirection, normal), 0.0f);
	float intens=0.0f;
	float specular = 0.0f;
	vec3 color = texture(diffuse0, texCoord).rgb;
	vec3 diffuse = diff * color;
	if (diff != 0.0f)
	{
			float specularLight = 0.50f;
			vec3 viewDirection = normalize(cameraPos - tangentPos);
			vec3 reflectionDirection = normalize(reflect(-lightDirection, normal));
			float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 64);
			specular = specAmount * specularLight;
			float angle = dot(normalize(lightPosition),normalize(lightDirection));// 1->light 0->dark
			intens = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
	};
	return texture(diffuse0, texCoord) * lightColor * (diff*intens + specular*intens + ambient);
}
void main()
{
	vec4 app;
	if(lightType==0)
		app = pointLight();
	if(lightType==1)
		app = spotLight();
	if(lightType==2)
		app=directionalLight();
	FragColor=app;
}