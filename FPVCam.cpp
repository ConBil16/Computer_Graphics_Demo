//
// Created by Eric on 10/12/2022.
//
#include "FPVCam.h"

void FPVCam::moveForward(GLfloat movementFactor) {
    //Camera moves with the character
    /*
    _lookAtPoint += _direction * movementFactor;
    recomputeOrientation();
     */
}

void FPVCam::moveBackward(GLfloat movementFactor) {
    //Camera moves with the character

    _lookAtPoint -= _direction * movementFactor;
    recomputeOrientation();

}

void FPVCam::recomputeOrientation() {
    glm::vec3 lookDir = glm::rotateY(glm::vec3(1, 0, 0), _dirAngle); //This is probably need to change this
    _position = _centerPosition + lookDir * 1.0f;
    _lookAtPoint = _position + lookDir;
    computeViewMatrix();
}

void FPVCam::turnLeft(GLfloat movementFactor) {
    //Turn the camera left

    _dirAngle += movementFactor;
    if (_dirAngle > M_PI) {     // no overflow
        _dirAngle -= 2 * M_PI;
    }
    _direction = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), _dirAngle);
}

void FPVCam::turnRight(GLfloat movementFactor) {
    //Turn the camera right

    _dirAngle -= movementFactor;
    if (_dirAngle < -M_PI) {    // no overflow
        _dirAngle += 2 * M_PI;
    }
    _direction = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), _dirAngle);
}

void FPVCam::zoom(GLfloat movementFactor) {
    //This will zoom

    _radius += movementFactor * 0.05f;
    if (_radius > 10.0f) {
        _radius = 10.0f;
    }
    if (_radius < 1.0f) {
        _radius = 1.0f;
    }
    recomputeOrientation();
}

void FPVCam::setDirection(float angle){
    _dirAngle = angle;
    recomputeOrientation();
}

void FPVCam::setPosition1(glm::vec3 poop){
    _centerPosition = poop;
    recomputeOrientation();
}

