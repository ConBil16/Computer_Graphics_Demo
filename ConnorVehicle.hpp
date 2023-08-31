#ifndef LAB05_CONNORVEHICLE_HPP
#define LAB05_CONNORVEHICLE_HPP

#include <GL/glew.h>
#include "glm/gtx/rotate_vector.hpp"
#include <glm/glm.hpp>

class ConnorVehicle {
public:
    /// \desc creates a simple plane that gives the appearance of flight
    /// \param shaderProgramHandle shader program handle that the plane should be drawn using
    /// \param mvpMtxUniformLocation uniform location for the full precomputed MVP matrix
    /// \param normalMtxUniformLocation uniform location for the precomputed Normal matrix
    /// \param materialDiffuseColorUniformLocation uniform location for the material diffuse color
    /// \param materialSpecularColorUniformLocation uniform location for the material specular color
    /// \param materialAmbientColorUniformLocation uniform location for the material ambient color
    /// \param materialGlossUniformLocation uniform location for the material glossiness
    ConnorVehicle(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint modelMtxUniformLocation, GLint normalMtxUniformLocation,
                  GLint materialDiffuseColorUniformLocation, GLint materialSpecularColorUniformLocation,
                  GLint materialAmbientColorUniformLocation, GLint materialGlossUniformLocation);

    /// \desc draws the model plane for a given MVP matrix
    /// \param modelMtx existing model matrix to apply to plane
    /// \param viewMtx camera view matrix to apply to plane
    /// \param projMtx camera projection matrix to apply to plane
    /// \note internally uses the provided shader program and sets the necessary uniforms
    /// for the MVP and Normal Matrices as well as the material diffuse color
    void drawPlane( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    /// \desc simulates the plane flying forward by rotating the propeller clockwise
    void flyForward();
    /// \desc simulates the plane flying backward by rotating the propeller counter-clockwise
    void flyBackward();

    //Added in movement factors
    void moveForward(GLfloat movementFactor);
    void moveBackward(GLfloat movementFactor);
    void turnLeft(GLfloat movementFactor);
    void turnRight(GLfloat movementFactor);

    glm::vec3 charPos = glm::vec3(0,.15,0);
    glm::vec3 charDir = glm::vec3(1,0,0);
    float charRot = 0.0f;

private:
    glm::vec3 _RB;
    glm::vec3 _LB;
    glm::vec3 _RF;
    glm::vec3 _LF;



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
        GLint modelMtx;
        /// \desc location of the precomputed Normal matrix
        GLint normalMtx;
        /// \desc location of the material diffuse color
        GLint materialColorDiffuse;
        GLint materialColorSpecular;
        GLint materialColorAmbient;
        GLint materialGloss;
    } _shaderProgramUniformLocations;

    /// \desc angle to rotate our plane at
    GLfloat _rotatePlaneAngle;

    /// \desc color the plane's body
    glm::vec4 _colorDiffuseBody;
    glm::vec4 _colorSpecularBody;
    glm::vec4 _colorAmbientBody;
    glm::vec3 _glossBody;
    /// \desc amount to scale the plane's body by
    glm::vec3 _scaleBody;

    /// \desc color the plane's wing
    glm::vec4 _colorDiffuseWing;
    glm::vec4 _colorSpecularWing;
    glm::vec4 _colorAmbientWing;
    glm::vec3 _glossWing;
    /// \desc amount to scale the plane's wing by
    glm::vec3 _scaleWing;
    /// \desc amount to rotate the plane's wing by
    GLfloat _rotateWingAngle;

    /// \desc color the plane's nose
    glm::vec4 _colorDiffuseNose;
    glm::vec4 _colorSpecularNose;
    glm::vec4 _colorAmbientNose;
    glm::vec3 _glossNose;
    /// \desc amount to rotate the plane's nose by
    GLfloat _rotateNoseAngle;

    /// \desc color the plane's propeller
    glm::vec4 _colorDiffuseProp;
    glm::vec4 _colorSpecularProp;
    glm::vec4 _colorAmbientProp;
    glm::vec3 _glossProp;
    /// \desc amount to scale the plane's propeller by
    glm::vec3 _scaleProp;
    /// \desc amount to translate the plane's propeller by
    glm::vec3 _transProp;

    /// \desc color the plane's tail
    glm::vec4 _colorDiffuseTail;
    glm::vec4 _colorSpecularTail;
    glm::vec4 _colorAmbientTail;
    glm::vec3 _glossTail;

    /// \desc draws just the plane's body
    /// \param modelMtx existing model matrix to apply to plane
    /// \param viewMtx camera view matrix to apply to plane
    /// \param projMtx camera projection matrix to apply to plane
    void _drawPlaneBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawWheels(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawRFwheel(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawLFwheel(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawRBwheel(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawLBwheel(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws a single wing
    /// \param isLeftWing true if left wing, false if right wing (controls wing charRot)
    /// \param modelMtx existing model matrix to apply to plane
    /// \param viewMtx camera view matrix to apply to plane
    /// \param projMtx camera projection matrix to apply to plane
    void _drawPlaneWing(bool isLeftWing, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws the nose of the plane
    /// \param modelMtx existing model matrix to apply to plane
    /// \param viewMtx camera view matrix to apply to plane
    /// \param projMtx camera projection matrix to apply to plane
    void _drawPlaneNose(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
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


#endif //LAB05_CONNORVEHICLE_HPP
