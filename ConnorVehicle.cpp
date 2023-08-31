#include "ConnorVehicle.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>

#ifndef M_PI
#define M_PI 3.14159265
#endif

ConnorVehicle::ConnorVehicle(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint modelMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialDiffuseColorUniformLocation, GLint materialSpecularColorUniformLocation,
                             GLint materialAmbientColorUniformLocation, GLint materialGlossUniformLocation) {
    _propAngle = M_PI/2.0f;
    _propAngleRotationSpeed = M_PI / 16.0f;

    _shaderProgramHandle                            = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx           = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.normalMtx        = normalMtxUniformLocation;
    _shaderProgramUniformLocations.modelMtx = modelMtxUniformLocation;
    _shaderProgramUniformLocations.materialColorDiffuse    = materialDiffuseColorUniformLocation;
    _shaderProgramUniformLocations.materialColorSpecular    = materialSpecularColorUniformLocation;
    _shaderProgramUniformLocations.materialColorAmbient    = materialAmbientColorUniformLocation;
    _shaderProgramUniformLocations.materialGloss    = materialGlossUniformLocation;

    _rotatePlaneAngle = M_PI / 2.0f;

    _colorDiffuseBody = glm::vec4( 0.0f, 0.78f, 1.0f, 1.0f );
    _colorSpecularBody = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
    _colorAmbientBody = glm::vec4( 0.0f, 0.078f, 0.0f, 1.0f );
    _glossBody = glm::vec3( 0.0f, 0.0f, 1.0f );
    _scaleBody = glm::vec3( 2.0f, 0.5f, 1.0f );

    _colorDiffuseWing = glm::vec4( 0.0f, 0.78f, 1.0f, 1.0f );
    _colorSpecularWing = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
    _colorAmbientWing = glm::vec4( 0.0f, 0.078f, 0.0f, 1.0f );
    _glossWing = glm::vec3( 1.0f, 0.0f, 0.0f );
    _scaleWing = glm::vec3( 1.5f, 0.5f, 1.0f );
    _rotateWingAngle = M_PI / 2.0f;

    _colorDiffuseNose = glm::vec4( 0.0f, 0.78f, 1.0f, 1.0f );
    _colorSpecularNose = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
    _colorAmbientNose = glm::vec4( 0.0f, 0.078f, 0.0f, 1.0f );
    _glossNose = glm::vec3( 0.0f, 1.0f, 0.0f );
    _rotateNoseAngle = M_PI / 2.0f;

    _colorDiffuseProp = glm::vec4( 0.0f, .78f, 0.0f, 1.0f );
    _colorSpecularProp = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
    _colorAmbientProp = glm::vec4( 0.0f, 0.078f, 0.0f, 1.0f );
    _glossProp = glm::vec3( 1.0f, 1.0f, 1.0f );
    _scaleProp = glm::vec3( 1.1f, 1.0f, 0.025f );
    _transProp = glm::vec3( 0.1f, 0.0f, 0.0f );

    float frontOffset = .1f;

    _RF = glm::vec3 (frontOffset, 0.0f, 0.06f);
    _LF = glm::vec3 (frontOffset, 0.0f, -0.06f);
    _LB = glm::vec3 (-.1f, 0.0f, -0.06f);
    _RB = glm::vec3 (-.1f, 0.0f, 0.06f);

    _colorDiffuseTail = glm::vec4( 0.0f, 0.78f, 1.0f , 1.0f);
    _colorSpecularTail = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f);
    _colorAmbientTail = glm::vec4( 0.0f, 0.078f, 0.0f, 1.0f );
    _glossTail = glm::vec3( 1.0f, 1.0f, 0.0f );
}

void ConnorVehicle::drawPlane(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) {
    glUseProgram( _shaderProgramHandle );

    modelMtx = glm::translate(  modelMtx, charPos);
    modelMtx = glm::rotate(modelMtx, charRot + glm::radians(90.0f), CSCI441::Y_AXIS);
    modelMtx = glm::rotate( modelMtx, -_rotatePlaneAngle, CSCI441::Y_AXIS );
    modelMtx = glm::scale( modelMtx, _scaleBody );


    //modelMtx = glm::rotate( modelMtx, _rotatePlaneAngle, CSCI441::Z_AXIS );
    _drawPlaneBody(modelMtx, viewMtx, projMtx);        // the body of our plane
    _drawRBwheel(modelMtx, viewMtx, projMtx);
    _drawLBwheel(modelMtx, viewMtx, projMtx);
    _drawLFwheel(modelMtx, viewMtx, projMtx);
    _drawRFwheel(modelMtx, viewMtx, projMtx);
    //_drawWheels(modelMtx, viewMtx, projMtx);
    //_drawPlaneWing(true, modelMtx, viewMtx, projMtx);  // the left wing
    //_drawPlaneWing(false, modelMtx, viewMtx, projMtx); // the right wing
    //_drawPlaneNose(modelMtx, viewMtx, projMtx);        // the nose
    //_drawPlanePropeller(modelMtx, viewMtx, projMtx);   // the propeller
    //_drawPlaneTail(modelMtx, viewMtx, projMtx);        // the tail
}

void ConnorVehicle::flyForward() {
    _propAngle += _propAngleRotationSpeed;
    if( _propAngle > 2.0f * M_PI ) _propAngle -= 2.0f * M_PI;
}

void ConnorVehicle::flyBackward() {
    _propAngle -= _propAngleRotationSpeed;
    if( _propAngle < 0.0f ) _propAngle += 2.0f * M_PI;
}

void ConnorVehicle::_drawPlaneBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glUniform4fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseBody[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularBody[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientBody[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    CSCI441::drawSolidCube( 0.1 );
}

//Following functions draw the wheels

void ConnorVehicle::_drawRBwheel(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    glm::mat4 modelMtx1 = glm::translate( modelMtx, _RB );
    modelMtx1 = glm::rotate( modelMtx1, _propAngle, CSCI441::Z_AXIS );
    modelMtx1 = glm::scale( modelMtx1, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx1, viewMtx, projMtx);

    glUniform4fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseBody[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularBody[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientBody[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    CSCI441::drawSolidCube( 0.1 );

    glm::mat4 modelMtx2 = glm::translate( modelMtx, _RB );
    modelMtx2 = glm::rotate( modelMtx2, static_cast<GLfloat>(M_PI / 2.0f) + _propAngle, CSCI441::Z_AXIS );
    modelMtx2 = glm::scale( modelMtx2, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx2, viewMtx, projMtx);

    CSCI441::drawSolidCube( 0.1 );
}

void ConnorVehicle::_drawLBwheel(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    glm::mat4 modelMtx1 = glm::translate( modelMtx, _LB );
    modelMtx1 = glm::rotate( modelMtx1, _propAngle, CSCI441::Z_AXIS );
    modelMtx1 = glm::scale( modelMtx1, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx1, viewMtx, projMtx);

    glUniform4fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseBody[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularBody[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientBody[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    CSCI441::drawSolidCube( 0.1 );

    glm::mat4 modelMtx2 = glm::translate( modelMtx, _LB );
    modelMtx2 = glm::rotate( modelMtx2, static_cast<GLfloat>(M_PI / 2.0f) + _propAngle, CSCI441::Z_AXIS );
    modelMtx2 = glm::scale( modelMtx2, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx2, viewMtx, projMtx);

    CSCI441::drawSolidCube( 0.1 );
}

void ConnorVehicle::_drawRFwheel(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    glm::mat4 modelMtx1 = glm::translate( modelMtx, _RF );
    modelMtx1 = glm::rotate( modelMtx1, _propAngle, CSCI441::Z_AXIS );
    modelMtx1 = glm::scale( modelMtx1, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx1, viewMtx, projMtx);

    glUniform4fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseBody[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularBody[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientBody[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    CSCI441::drawSolidCube( 0.1 );

    glm::mat4 modelMtx2 = glm::translate( modelMtx, _RF );
    modelMtx2 = glm::rotate( modelMtx2, static_cast<GLfloat>(M_PI / 2.0f) + _propAngle, CSCI441::Z_AXIS );
    modelMtx2 = glm::scale( modelMtx2, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx2, viewMtx, projMtx);

    CSCI441::drawSolidCube( 0.1 );
}

void ConnorVehicle::_drawLFwheel(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    glm::mat4 modelMtx1 = glm::translate( modelMtx, _LF );
    modelMtx1 = glm::rotate( modelMtx1, _propAngle, CSCI441::Z_AXIS );
    modelMtx1 = glm::scale( modelMtx1, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx1, viewMtx, projMtx);

    glUniform4fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseBody[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularBody[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientBody[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    CSCI441::drawSolidCube( 0.1 );

    glm::mat4 modelMtx2 = glm::translate( modelMtx, _LF );
    modelMtx2 = glm::rotate( modelMtx2, static_cast<GLfloat>(M_PI / 2.0f) + _propAngle, CSCI441::Z_AXIS );
    modelMtx2 = glm::scale( modelMtx2, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx2, viewMtx, projMtx);

    CSCI441::drawSolidCube( 0.1 );

}

void ConnorVehicle::_drawPlaneWing(bool isLeftWing, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    modelMtx = glm::scale( modelMtx, _scaleWing );

    if( isLeftWing )
        modelMtx = glm::rotate( modelMtx, -_rotateWingAngle, CSCI441::X_AXIS );
    else
        modelMtx = glm::rotate( modelMtx, _rotateWingAngle, CSCI441::X_AXIS );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform4fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseWing[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularWing[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientWing[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    CSCI441::drawSolidCone( 0.05, 0.2, 16, 16 );
}

void ConnorVehicle::_drawPlaneNose(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    modelMtx = glm::rotate( modelMtx, _rotateNoseAngle, CSCI441::Z_AXIS );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform4fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseNose[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularNose[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientNose[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    CSCI441::drawSolidCone( 0.025, 0.3, 16, 16 );
}





void ConnorVehicle::_drawPlanePropeller(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    glm::mat4 modelMtx1 = glm::translate( modelMtx, _transProp );
    modelMtx1 = glm::rotate( modelMtx1, _propAngle, CSCI441::X_AXIS );
    modelMtx1 = glm::scale( modelMtx1, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx1, viewMtx, projMtx);

    glUniform4fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseProp[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularProp[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientProp[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    CSCI441::drawSolidCube( 0.1 );

    glm::mat4 modelMtx2 = glm::translate( modelMtx, _transProp );
    modelMtx2 = glm::rotate( modelMtx2, static_cast<GLfloat>(M_PI / 2.0f) + _propAngle, CSCI441::X_AXIS );
    modelMtx2 = glm::scale( modelMtx2, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx2, viewMtx, projMtx);

    CSCI441::drawSolidCube( 0.1 );
}

void ConnorVehicle::_drawPlaneTail(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform4fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseTail[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularTail[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientTail[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    CSCI441::drawSolidCone( 0.02, 0.1, 16, 16 );
}

void ConnorVehicle::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0] );
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.modelMtx, 1, GL_FALSE, &modelMtx[0][0] );
    glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx )));
    glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, &normalMtx[0][0] );
}

void ConnorVehicle::moveForward(GLfloat movementFactor) {
    charPos += charDir * movementFactor;
    _propAngle -= _propAngleRotationSpeed;
//    recomputeOrientation();
}

void ConnorVehicle::moveBackward(GLfloat movementFactor) {
    charPos -= charDir * movementFactor;
    _propAngle += _propAngleRotationSpeed;
//    recomputeOrientation();
}

void ConnorVehicle::turnLeft(GLfloat movementFactor) {
    charRot += movementFactor;
    if (charRot > M_PI) {     // no overflow
        charRot -= 2 * M_PI;
    }
    charDir = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), charRot);
}

void ConnorVehicle::turnRight(GLfloat movementFactor) {
    charRot -= movementFactor;
    if (charRot < -M_PI) {    // no overflow
        charRot += 2 * M_PI;
    }
    charDir = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), charRot);
}



