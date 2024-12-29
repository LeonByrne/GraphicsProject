#version 330 core

in vec3 worldPos;
in vec3 worldNormal; 

in vec2 tex;

out vec3 finalColor;

uniform vec3 lightPos;
uniform vec3 lightIntensity;

uniform sampler2D baseTexture;
uniform sampler2D roughnessTexture;
uniform sampler2D normalTexture;

void main()
{
	// Lighting
	vec3 lightDir = lightPos - worldPos;
	float lightDist = dot(lightDir, lightDir);
	lightDir = normalize(lightDir);
	vec3 v = lightIntensity * clamp(dot(lightDir, worldNormal), 0.0, 1.0) / lightDist;

	// Tone mapping
	v = v / (1.0 + v);

	// Gamma correction
	finalColor = pow(v, vec3(1.0 / 2.2));

	vec4 baseColour = texture(baseTexture, tex);
	// vec4 metallicRoughness = texture(roughnessTexture, tex);
	// vec3 normal = texture(normalTexture, tex).rgb * 2.0;

	finalColor = baseColour.rgb;
}
