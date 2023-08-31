//
// Created by Eric on 10/12/2022.
//

#ifndef INC_441MIDTERM_FPVCAM_H
#define INC_441MIDTERM_FPVCAM_H

#include "CSCI441/Camera.hpp"
#include "glm/gtx/rotate_vector.hpp"

class FPVCam : public CSCI441::Camera {
public:
    FPVCam() : CSCI441::Camera() {
        _lookAtPoint = glm::vec3(0.0f, 0.5f, 0.0f);
        _direction = glm::vec3(1.0f, 0.0f, 0.0f);
        _dirAngle = 0.0f;
        _upVector = glm::vec3(0,1,0);
        _centerPosition = glm::vec3(0,0,0);
        recomputeOrientation();
    }
    void moveForward(GLfloat movementFactor) override;
    void moveBackward(GLfloat movementFactor) override;
    void recomputeOrientation() override;

    void turnLeft(GLfloat movementFactor);
    void turnRight(GLfloat movementFactor);
    void zoom(GLfloat movementFactor);

    glm::vec3 getDirection() { return _direction; };
    float getDirAngle() { return _dirAngle; };
    void setPosition1(glm::vec3 poop);
    void setDirection(float angle);


private:
    glm::vec3 _centerPosition;
    float _dirAngle;

};



#endif //INC_441MIDTERM_FPVCAM_H
