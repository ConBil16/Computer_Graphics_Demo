#ifndef LAB05_ETHE_HPP
#define LAB05_ETHE_HPP

#include <GL/glew.h>
#include "glm/gtx/rotate_vector.hpp"
#include <glm/glm.hpp>
#include <cmath>

class EtheOrb {
public:

    /// \desc ethe's direction
    float etheRotation;

    struct Rune {
        float theta;
        float phi;
        float r;
        float dTheta;
        float dPhi;
    };

    Rune runes[3];
    /// \desc creates a simple plane that gives the appearance of flight
    /// \param shaderProgramHandle shader program handle that the plane should be drawn using
    /// \param mvpMtxUniformLocation uniform location for the full precomputed MVP matrix
    /// \param normalMtxUniformLocation uniform location for the precomputed Normal matrix
    /// \param materialDiffuseColorUniformLocation uniform location for the material diffuse color
    /// \param materialSpecularColorUniformLocation uniform location for the material specular color
    /// \param materialAmbientColorUniformLocation uniform location for the material ambient color
    /// \param materialGlossUniformLocation uniform location for the material glossiness
    EtheOrb(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint modelMtxUniformLocation, GLint normalMtxUniformLocation,
            GLint materialDiffuseColorUniformLocation, GLint materialSpecularColorUniformLocation,
            GLint materialAmbientColorUniformLocation, GLint materialGlossUniformLocation);

    /// \desc draws the model plane for a given MVP matrix
    /// \param modelMtx existing model matrix to apply to plane
    /// \param viewMtx camera view matrix to apply to plane
    /// \param projMtx camera projection matrix to apply to plane
    /// \note internally uses the provided shader program and sets the necessary uniforms
    /// for the MVP and Normal Matrices as well as the material diffuse color
    void drawEtheOrb(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    /// \desc spaz out the runes
    void runeMove();
    /// \desc simulates the plane flying backward by rotating the propeller counter-clockwise
    void flyBackward();

    /// \desc handle ethe and cube positions
    void cubeLag(glm::vec3 newPos);

    /// \desc linear interpolation for lag
    glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t);

    //Added in movement factors
    void moveForward(GLfloat movementFactor);
    void moveBackward(GLfloat movementFactor);
    void turnLeft(GLfloat movementFactor);
    void turnRight(GLfloat movementFactor);

    glm::vec3 charPos = glm::vec3(0,0.3,-1);
    glm::vec3 charDir = glm::vec3(1,0,0);
    float charRot = 0.0f;

private:
    /// \desc current angle of charRot for the propeller
    GLfloat _propAngle;
    /// \desc one charRot step
    GLfloat _propAngleRotationSpeed;

    /// \desc handle of the shader program to use when drawing the plane
    GLuint _shaderProgramHandle;
    /// \desc stores the uniform locations needed for the plan information
    struct ShaderProgramUniformLocations {
        /// \desc location of the precomputed ModelViewProjection matrix
        GLint mvpMtx;
        /// \desc location of the precomputed Normal matrix
        GLint normalMtx;
        GLint modelMtx;
        /// \desc location of the material diffuse color
        GLint materialColorDiffuse;
        GLint materialColorSpecular;
        GLint materialColorAmbient;
        GLint materialGloss;
    } _shaderProgramUniformLocations;

    /// \desc angle to rotate our plane at
    GLfloat _rotatePlaneAngle;

    /// \desc color the plane's body
    glm::vec4 _colorDiffuseOrb;
    glm::vec4 _colorSpecularOrb;
    glm::vec4 _colorAmbientOrb;
    glm::vec4 _glossOrb;
    /// \desc amount to scale the plane's body by
    glm::vec3 _scaleOrb;

    /// \desc color the plane's wing
    glm::vec4 _colorDiffuseEye;
    glm::vec4 _colorSpecularEye;
    glm::vec4 _colorAmbientEye;
    glm::vec4 _glossEye;
    /// \desc amount to scale the plane's wing by
    glm::vec3 _scaleWing;
    /// \desc amount to rotate the plane's wing by
    GLfloat _rotateWingAngle;

    /// \desc color the plane's nose
    glm::vec4 _colorDiffuseRune;
    glm::vec4 _colorSpecularRune;
    glm::vec4 _colorAmbientRune;
    glm::vec4 _glossRune;
    /// \desc amount to rotate the plane's nose by
    GLfloat _rotateNoseAngle;

    /// \desc color the plane's propeller
    glm::vec3 _colorDiffuseProp;
    glm::vec3 _colorSpecularProp;
    glm::vec3 _colorAmbientProp;
    glm::vec3 _glossProp;
    /// \desc amount to scale the plane's propeller by
    glm::vec3 _scaleProp;
    /// \desc amount to translate the plane's propeller by
    glm::vec3 _transProp;

    /// \desc color the plane's tail
    glm::vec4 _colorDiffuseTail;
    glm::vec4 _colorSpecularTail;
    glm::vec4 _colorAmbientTail;
    glm::vec4 _glossTail;

    /// \desc ethe's position
    glm::vec3 _ethePos;
    /// \desc ethe's heading
    glm::vec3 _etheDir;

    /// \desc cube orbit point
    glm::vec3 _cubePos;



    /// \desc draws a transparent sphere
    /// \param modelMtx existing model matrix to apply to plane
    /// \param viewMtx camera view matrix to apply to plane
    /// \param projMtx camera projection matrix to apply to plane
    void _drawOrb(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

    /// \desc draws eyes
    /// \param modelMtx existing model matrix to apply to plane
    /// \param viewMtx camera view matrix to apply to plane
    /// \param projMtx camera projection matrix to apply to plane
    void _drawEyes(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

    /// \desc draws the nose of the plane
    /// \param modelMtx existing model matrix to apply to plane
    /// \param viewMtx camera view matrix to apply to plane
    /// \param projMtx camera projection matrix to apply to plane
    void _drawRunes(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

    /// \desc draws the two propeller pieces
    /// \param modelMtx existing model matrix to apply to plane
    /// \param viewMtx camera view matrix to apply to plane
    /// \param projMtx camera projection matrix to apply to plane
    void _drawPlanePropeller(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws the tail of the plane
    /// \param modelMtx existing model matrix to apply to plane
    /// \param viewMtx camera view matrix to apply to plane
    /// \param projMtx camera projection matrix to apply to plane
    void _drawPlaneTail(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

    /// \desc precomputes the matrix uniforms CPU-side and then sends them
    /// to the GPU to be used in the shader for each vertex.  It is more efficient
    /// to calculate these once and then use the resultant product in the shader.
    /// \param modelMtx model transformation matrix
    /// \param viewMtx camera view matrix
    /// \param projMtx camera projection matrix
    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;


};


#endif //LAB05_ETHE_HPP
