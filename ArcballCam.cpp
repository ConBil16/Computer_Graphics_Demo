//
// Created by Eric on 10/6/2022.
//

#include "ArcballCam.h"

void ArcballCam::moveForward(GLfloat movementFactor) {
    _lookAtPoint += _direction * movementFactor;
    recomputeOrientation();
}

void ArcballCam::moveBackward(GLfloat movementFactor) {
    _lookAtPoint -= _direction * movementFactor;
    recomputeOrientation();
}

void ArcballCam::recomputeOrientation() {
    glm::vec3 lookDir = glm::vec3(_radius * sin(_phi) * cos(_theta), _radius * cos(_phi), _radius * sin(_phi) * sin(_theta));
    _position = _lookAtPoint - lookDir;
    computeViewMatrix();
}

void ArcballCam::turnLeft(GLfloat movementFactor) {
    _dirAngle += movementFactor;
    if (_dirAngle > M_PI) {     // no overflow
        _dirAngle -= 2 * M_PI;
    }
    _direction = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), _dirAngle);
}

void ArcballCam::turnRight(GLfloat movementFactor) {
    _dirAngle -= movementFactor;
    if (_dirAngle < -M_PI) {    // no overflow
        _dirAngle += 2 * M_PI;
    }
    _direction = glm::rotateY(glm::vec3(1.0f, 0.0f, 0.0f), _dirAngle);
}

void ArcballCam::zoom(GLfloat movementFactor) {
    _radius += movementFactor * 0.05f;
    if (_radius > 10.0f) {
        _radius = 10.0f;
    }
    if (_radius < 1.0f) {
        _radius = 1.0f;
    }
    recomputeOrientation();
}

void ArcballCam::setPostion1(glm::vec3 position) {
    _lookAtPoint = position;
    recomputeOrientation();
}


