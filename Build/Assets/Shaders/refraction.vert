#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec3 otexcoord;

uniform float ior = 2.417;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//transform vertex position/normal to world space
	vec3 position = vec3(model * vec4(vposition, 1));
	vec3 normal = normalize(mat3(model) * vnormal);

	//inverse of view space -> world space
	//last column of mat4 is position
	vec3 viewPosition = inverse(view)[3].xyz;
	vec3 viewDir = normalize(position - viewPosition);

	//refract view direaction about vertex normal using index of refraction
	otexcoord = refract(viewDir, normal, 1/ior);

	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(vposition, 1.0);
}
