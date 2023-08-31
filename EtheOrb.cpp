#include "EtheOrb.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>

#ifndef M_PI
#define M_PI 3.14159265
#endif

EtheOrb::EtheOrb(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint modelMtxUniformLocation, GLint normalMtxUniformLocation,
                 GLint materialDiffuseColorUniformLocation, GLint materialSpecularColorUniformLocation,
                 GLint materialAmbientColorUniformLocation, GLint materialGlossUniformLocation) {
    _propAngle = 0.0f;
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

    _colorDiffuseOrb = glm::vec4(0.8f, 0.0f, 0.8f, 0.7f );
    _colorSpecularOrb = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    _colorAmbientOrb = glm::vec4(0.08f, 0.0f, 0.08f, 0.7f );
    _glossOrb = glm::vec4(0.8f, 0.0f, 0.8f, 1.0f );
    _scaleOrb = glm::vec3(2.0f, 0.5f, 1.0f );

    _colorDiffuseEye = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f );
    _colorSpecularEye = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    _colorAmbientEye = glm::vec4(0.0f, 0.05f, 0.0f, 1.0f );
    _glossEye = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f );
    _scaleWing = glm::vec3( 1.0f, 0.5f, 0.7f );
    _rotateWingAngle = M_PI / 2.0f;

    _colorDiffuseRune = glm::vec4(0.9f, 0.5f, 0.8f, 1.0f);
    _colorSpecularRune = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    _colorAmbientRune = glm::vec4(0.09f, 0.05f, 0.08f, 1.0f);
    _glossRune = glm::vec4(0.9f, 0.5f, 0.8f, 1.0f);
    _rotateNoseAngle = M_PI / 2.0f;

    // init randomized cube angles
    runes[0] = {float(2.0f * M_PI * (rand() / float(RAND_MAX))), float(M_PI * (rand() / float(RAND_MAX))), 0.5, 0.1f - float(0.2 * (rand() / float(RAND_MAX))), 0.1f - float(0.2 * (rand() / float(RAND_MAX)))};
    runes[1] = {float(2.0f * M_PI * (rand() / float(RAND_MAX))), float(M_PI * (rand() / float(RAND_MAX))), 0.5, 0.1f - float(0.2 * (rand() / float(RAND_MAX))), 0.1f - float(0.2 * (rand() / float(RAND_MAX)))};
    runes[2] = {float(2.0f * M_PI * (rand() / float(RAND_MAX))), float(M_PI * (rand() / float(RAND_MAX))), 0.5, 0.1f - float(0.2 * (rand() / float(RAND_MAX))), 0.1f - float(0.2 * (rand() / float(RAND_MAX)))};

    _cubePos = glm::vec3(0.0f, 0.0f, 0.0f);
    _ethePos = glm::vec3(0.0f, 0.0f, 0.0f);
}

void EtheOrb::drawEtheOrb(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) {
    glUseProgram( _shaderProgramHandle );

    glm::mat4 modelMtxOrb = glm::translate(  modelMtx, charPos);
    modelMtxOrb = glm::rotate(modelMtxOrb, charRot, CSCI441::Y_AXIS);

    modelMtxOrb = glm::scale(modelMtxOrb, glm::vec3(0.3, 0.3, 0.3));
    modelMtxOrb = glm::rotate(modelMtxOrb, glm::radians(180.0f) + etheRotation, CSCI441::Y_AXIS );

    _drawEyes(modelMtxOrb, viewMtx, projMtx);       // eyes
    modelMtx = glm::translate(  modelMtx, _cubePos);
    _drawRunes(modelMtx, viewMtx, projMtx);      // draws tracing/orbiting runes


    _drawOrb(modelMtxOrb, viewMtx, projMtx);        // transparent "projection" orb
}

void EtheOrb::runeMove() {
    for (int i = 0; i < 3; i++) {
        runes[i].theta += runes[i].dTheta;
        runes[i].phi += runes[i].dPhi;

        if (runes[i].theta > 2.0f * M_PI) {
            runes[i].theta -= 2.0f * M_PI;
        }
        if (runes[i].theta < 0) {
            runes[i].theta += 2.0f * M_PI;
        }
        if (runes[i].phi >  M_PI) {
            runes[i].phi -= 2.0f * M_PI;
        }
        if (runes[i].phi < - M_PI) {
            runes[i].phi += 2.0f * M_PI;
        }
    }
}

void EtheOrb::flyBackward() {
    _propAngle -= _propAngleRotationSpeed;
    if( _propAngle < 0.0f ) _propAngle += 2.0f * M_PI;
}

void EtheOrb::_drawOrb(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform4fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseOrb[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularOrb[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientOrb[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    CSCI441::drawSolidSphere( 1.0f, 10, 10 );
}

void EtheOrb::_drawEyes(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    modelMtx = glm::scale( modelMtx, _scaleWing );

    glUniform4fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseEye[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularEye[0]);
    glUniform4fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientEye[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    glm::mat4 modelMtxL = glm::translate(modelMtx, glm::vec3(-0.75f, -0.8f, 0.2f));
    glm::mat4 modelMtxR = glm::translate(modelMtx, glm::vec3(-0.75f, -0.8f, -0.2f));

    _computeAndSendMatrixUniforms(modelMtxL, viewMtx, projMtx);
    CSCI441::drawSolidSphere(0.1f, 10, 10);

    _computeAndSendMatrixUniforms(modelMtxR, viewMtx, projMtx);
    CSCI441::drawSolidSphere(0.1f, 10, 10);

}

void EtheOrb::_drawRunes(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    // modelMtx = glm::translate( modelMtx, _cubePos - _ethePos );

    for (int i = 0; i < 3; i++) {
        glm::vec3 spherical = glm::vec3(runes[i].r * sin(runes[i].phi) * cos(runes[i].theta), runes[i].r * cos(runes[i].phi), runes[i].r * sin(runes[i].phi) * sin(runes[i].theta));;
        glm::mat4 runeMtx = glm::translate(modelMtx, spherical);

        _computeAndSendMatrixUniforms(runeMtx, viewMtx, projMtx);

        glUniform4fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseRune[0]);
        glUniform4fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularRune[0]);
        glUniform4fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientRune[0]);
        glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

        CSCI441::drawSolidCube(0.1f);
    }

}

void EtheOrb::_drawPlanePropeller(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    glm::mat4 modelMtx1 = glm::translate( modelMtx, _transProp );
    modelMtx1 = glm::rotate( modelMtx1, _propAngle, CSCI441::X_AXIS );
    modelMtx1 = glm::scale( modelMtx1, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx1, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseProp[0]);
    glUniform3fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularProp[0]);
    glUniform3fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientProp[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    CSCI441::drawSolidCube( 0.1 );

    glm::mat4 modelMtx2 = glm::translate( modelMtx, _transProp );
    modelMtx2 = glm::rotate( modelMtx2, static_cast<GLfloat>(M_PI / 2.0f) + _propAngle, CSCI441::X_AXIS );
    modelMtx2 = glm::scale( modelMtx2, _scaleProp );

    _computeAndSendMatrixUniforms(modelMtx2, viewMtx, projMtx);

    CSCI441::drawSolidCube( 0.1 );

}

void EtheOrb::_drawPlaneTail(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColorDiffuse, 1, &_colorDiffuseTail[0]);
    glUniform3fv(_shaderProgramUniformLocations.materialColorSpecular, 1, &_colorSpecularTail[0]);
    glUniform3fv(_shaderProgramUniformLocations.materialColorAmbient, 1, &_colorAmbientTail[0]);
    glUniform1f(_shaderProgramUniformLocations.materialGloss, 1.0f);

    CSCI441::drawSolidCone( 0.02, 0.1, 16, 16 );
}

void EtheOrb::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0] );
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.modelMtx, 1, GL_FALSE, &modelMtx[0][0] );

    glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx )));
    glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, &normalMtx[0][0] );


}

void EtheOrb::cubeLag(glm::vec3 newPos) {
    _ethePos = charPos;
    _cubePos = lerp(_cubePos, _ethePos, 0.02);
}

glm::vec3 EtheOrb::lerp(glm::vec3 a, glm::vec3 b, float t) {
    return a + (b - a) * t;
}

void EtheOrb::moveForward(GLfloat movementFactor) {
    charPos += charDir * movementFactor;
    cubeLag(charPos);
//    recomputeOrientation();
}

void EtheOrb::moveBackward(GLfloat movementFactor) {
    charPos -= charDir * movementFactor;
    cubeLag(charPos);
//    recomputeOrientation();
}

void EtheOrb::turnLeft(GLfloat movementFactor) {
    charRot += movementFactor;
    if (charRot > M_PI) {     // no overflow
        charRot -= 2 * M_PI;
    }
    charDir = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), charRot);
}

void EtheOrb::turnRight(GLfloat movementFactor) {
    charRot -= movementFactor;
    if (charRot < -M_PI) {    // no overflow
        charRot += 2 * M_PI;
    }
    charDir = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), charRot);
}