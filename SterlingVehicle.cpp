#include "SterlingVehicle.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>

#ifndef M_PI
#define M_PI 3.14159265
#endif

Vehicle::Vehicle(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint modelMtxUniformLocation, GLint normalMtxUniformLocation,
                 GLint materialDiffuseColorUniformLocation, GLint materialSpecularColorUniformLocation,
                 GLint materialAmbientColorUniformLocation, GLint materialGlossUniformLocation) {

    _shaderProgramHandle                            = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx           = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.modelMtx = modelMtxUniformLocation;
    _shaderProgramUniformLocations.normalMtx        = normalMtxUniformLocation;
    _shaderProgramUniformLocations.materialColorDiffuse    = materialDiffuseColorUniformLocation;
    _shaderProgramUniformLocations.materialColorSpecular    = materialSpecularColorUniformLocation;
    _shaderProgramUniformLocations.materialColorAmbient    = materialAmbientColorUniformLocation;
    _shaderProgramUniformLocations.materialGloss    = materialGlossUniformLocation;

    // x = width, y = height, z = length (forward and back)
    _colorDiffuseBody = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
    _colorSpecularBody = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
    _colorAmbientBody = glm::vec4( 0.1f, 0.1f, 1.0f, 1.0f );
    _glossBody = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
    _scaleBody = glm::vec4( 0.8f, 0.8f, 2.0f, 1.0f );

    _colorDiffuseWing = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
    _colorSpecularWing = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
    _colorAmbientWing = glm::vec4( 0.2f, 0.0f, 0.0f, 1.0f );
    _glossWing = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );

    _colorDiffuseNose = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
    _colorSpecularNose = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
    _colorAmbientNose = glm::vec4( 0.0f, 0.2f, 0.0f, 1.0f );
    _glossNose = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
    _rotateNoseAngle = M_PI / 2.0f;

}

void Vehicle::drawPlane(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) {
    glUseProgram( _shaderProgramHandle );
    //modelMtx = glm::translate(modelMtx,charPos);
    modelMtx = glm::rotate(modelMtx,charRot,CSCI441::Y_AXIS);
    modelMtx = glm::rotate(modelMtx, glm::radians(90.0f), CSCI441::Y_AXIS);

    _drawPlaneBody(modelMtx, viewMtx, projMtx);        // the body of our plane
    _drawWheel(modelMtx, viewMtx, projMtx);             // the wheel of our plane
    _drawPlaneNose(modelMtx, viewMtx, projMtx);        // the nose

}

void Vehicle::_drawPlaneBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    modelMtx = glm::scale( modelMtx, _scaleBody );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform4fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseBody[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularBody[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientBody[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    CSCI441::drawSolidCube( 0.3 );
}

void Vehicle::_drawWheel(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    modelMtx = glm::rotate( modelMtx, 0.0f, CSCI441::X_AXIS );
    modelMtx = glm::rotate(modelMtx, glm::radians(90.0f), CSCI441::Y_AXIS);

    modelMtx = glm::rotate(modelMtx, _rotateWheelAngle, CSCI441::Z_AXIS);

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform4fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseWing[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularWing[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientWing[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    CSCI441::drawSolidTorus( 0.05, 0.2, 8, 8 );
}

void Vehicle::_drawPlaneNose(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    modelMtx = glm::rotate( modelMtx, _rotateNoseAngle, CSCI441::Z_AXIS );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform4fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseNose[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularNose[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientNose[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    CSCI441::drawSolidCone( 0.025, 0.3, 16, 16 );


    modelMtx = glm::scale(modelMtx, glm::vec3(-1,-1,-1));

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    CSCI441::drawSolidCone( 0.025, 0.3, 16, 16 );
}

void Vehicle::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0] );
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.modelMtx, 1, GL_FALSE, &modelMtx[0][0] );
    glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx )));
    glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, &normalMtx[0][0] );
}

void Vehicle::_rotateWheel(float speeeeeed) {
    _rotateWheelAngle += 0.1f*speeeeeed;
}
void Vehicle::moveForward(GLfloat movementFactor) {
    charPos += charDir * movementFactor;
    _rotateWheel(1.0f);
}

void Vehicle::moveBackward(GLfloat movementFactor) {
    charPos -= charDir * movementFactor;
    _rotateWheel(-1.0f);
}

void Vehicle::turnLeft(GLfloat movementFactor) {
    charRot += movementFactor;
    if (charRot > M_PI) {     // no overflow
        charRot -= 2 * M_PI;
    }
    charDir = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), charRot);
}

void Vehicle::turnRight(GLfloat movementFactor) {
    charRot -= movementFactor;
    if (charRot < -M_PI) {    // no overflow
        charRot += 2 * M_PI;
    }
    charDir = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), charRot);
}