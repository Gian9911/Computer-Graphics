#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex; 
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 rotation; 
uniform mat4 camMatrix; // projection*view 
uniform mat4 scalingMatrix;
uniform mat4 translationMatrix;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform int lightType;

out vec2 texCoord;
// out vec3 Normal;
out vec3 tangentPos;
out vec3 Pos;
out vec3 lightPosition;
out vec3 cameraPos;


void main()
{ 
	Pos = vec3(translationMatrix * rotation * scalingMatrix * vec4(aPos, 1.0f));
	gl_Position =   camMatrix  * translationMatrix * rotation * scalingMatrix * vec4(aPos, 1.0f); //contains the Position of the current vertex - aveva tangentPos
	texCoord = aTex;
	
	mat3 my_model = transpose(inverse(mat3(translationMatrix * rotation * scalingMatrix)));
	
	vec3 T = normalize(vec3(my_model * aTangent));
	vec3 N = normalize(vec3(my_model * aNormal));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T,B,N));

	lightPosition = TBN * lightPos;
	cameraPos = TBN * camPos;
	tangentPos =   TBN * Pos;
	}
