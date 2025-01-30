#version 330 core

// Inputs from the vertex shader
in vec3 fragNormal;

// Uniforms for lighting
uniform vec3 AmbientColor = vec3(0.2);
uniform vec3 LightDirection = normalize(vec3(1, 5, 2));  // First light source
uniform vec3 LightColor = vec3(1);

uniform vec3 LightDirection2 = normalize(vec3(-2, 3, -1)); // Second light source
uniform vec3 LightColor2 = vec3(0.3, 0.1, 0.6);  // Second light color

uniform vec3 DiffuseColor; // Passed in from C++

// Output fragment color
out vec4 fragColor;

void main()
{
    // Compute irradiance from both lights and ambient
    vec3 irradiance = AmbientColor;
    
    // First light source
    irradiance += LightColor * max(0, dot(LightDirection, fragNormal));
    
    // Second light source
    irradiance += LightColor2 * max(0, dot(LightDirection2, fragNormal));

    // Diffuse reflectance
    vec3 reflectance = irradiance * DiffuseColor;

    // Gamma correction
    fragColor = vec4(sqrt(reflectance), 1);
}
