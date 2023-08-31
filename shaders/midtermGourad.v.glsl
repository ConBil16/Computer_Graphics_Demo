#version 410 core

// NOTE: Color changed to vec4 for materials to allow transparency/alpha on materials
// Light is still a vec3
// uniform inputs
uniform mat4 mvpMatrix;                 // the precomputed Model-View-Projection Matrix
uniform mat3 normMatrix;

// Need model matrix for specular calculations
uniform mat4 modelMatrix;

// Camera Attributes
uniform vec3 activeCamPos;

// Spot Light
uniform vec3 sLightPos;
uniform vec3 sLightColor;
uniform vec3 sLightDir;
uniform float sLightAngle;

// Point Light
uniform vec3 pLightPos;
uniform vec3 pLightColor;

// Directional Light
uniform vec3 dLightDir;
uniform vec3 dLightColor;   // probably white

uniform vec4 ambientMaterialColor;             // the material color for our vertex (& whole object)
uniform vec4 diffuseMaterialColor;
uniform vec4 specularMaterialColor;
uniform float materialGloss;

// attribute inputs
layout(location = 0) in vec3 vPos;      // the position of this specific vertex in object space
in vec3 vNorm;

// varying outputs
layout(location = 0) out vec4 color;    // color to apply to this vertex

void main() {
    // transform & output the vertex in clip space
    gl_Position = mvpMatrix * vec4(vPos, 1.0);

    // this happens a lot
    vec3 fPos = (modelMatrix * vec4(vPos, 1.0)).xyz;

    // Directional Light...
    vec3 lNorm = normalize(-dLightDir);
    vec3 vNorm2 = normalize(normMatrix * vNorm);

    // Diffuse Calculations
    vec3 diffuseColor = dLightColor * diffuseMaterialColor.xyz * max(dot(lNorm, vNorm2), 0.0);

    // Specular Light
    //vec3 reflectionDir = ((vNorm2 * 2.0 * dot(lNorm, vNorm2)) - lNorm);
    vec3 reflectionDir = reflect(dLightDir, vNorm2);
    vec3 dirToCam = normalize(activeCamPos - fPos);
    vec3 specularColor = (specularMaterialColor.xyz * pow(max(0, dot(dirToCam, reflectionDir)), materialGloss * 1)) * 0.5;

    // Ambient Light
    vec3 ambientColor = dLightColor * ambientMaterialColor.xyz;

    vec3 dirLightColor = specularColor + ambientColor + diffuseColor;


    // Spot Light...

    vec3 spotDir = sLightPos - fPos;
    float lightFalloff = pow(length(spotDir), -1);
    spotDir = normalize(spotDir);

    bool inCone = (acos(dot(spotDir, normalize(sLightDir)))) > (sLightAngle);

    // Diffuse Calculations
    diffuseColor = sLightColor * diffuseMaterialColor.xyz * max(dot(spotDir, vNorm2), 0.0) * lightFalloff;

    // Specular Light
    reflectionDir = reflect(spotDir, vNorm2);
    dirToCam = normalize(activeCamPos - fPos);
    specularColor = sLightColor * (specularMaterialColor.xyz * pow(max(0, dot(dirToCam, reflectionDir)), materialGloss * 1)) * 0.5 * lightFalloff;

    // Ambient Light
    ambientColor = sLightColor * ambientMaterialColor.xyz * lightFalloff;

    if (inCone) {
        diffuseColor = vec3(0.0, 0.0, 0.0);
    }
    vec3 spotLightColor = specularColor + ambientColor + diffuseColor;



    // Point Light...

    vec3 pointDir = pLightPos - fPos;
    lightFalloff = pow(length(pointDir), -2);
    pointDir = normalize(pointDir);

    // Diffuse Calculations
    diffuseColor = pLightColor * diffuseMaterialColor.xyz * max(dot(pointDir, vNorm2), 0.0) * lightFalloff;

    // Specular Light
    reflectionDir = reflect(pointDir, vNorm2);
    dirToCam = normalize(activeCamPos - fPos);
    specularColor = (specularMaterialColor.xyz * pow(max(0, dot(dirToCam, reflectionDir)), materialGloss * 1)) * 0.5 * lightFalloff;

    // Ambient Light
    ambientColor = pLightColor * ambientMaterialColor.xyz * lightFalloff;

    vec3 pointLightColor = specularColor + ambientColor + diffuseColor;


    // All the things!
    color = vec4(spotLightColor + pointLightColor + dirLightColor, ambientMaterialColor.w);
    // color = vec4(1.0, 1.0, 1.0, 1.0);
}