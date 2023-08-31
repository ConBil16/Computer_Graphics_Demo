//
// Created by Eric on 10/6/2022.
//

#ifndef A3_ARCBALLCAM_H
#define A3_ARCBALLCAM_H

#include "CSCI441/Camera.hpp"
#include "glm/gtx/rotate_vector.hpp"


class ArcballCam : public CSCI441::Camera {
public:
    ArcballCam() : CSCI441::Camera() {
        _lookAtPoint = glm::vec3(0.0f, 0.5f, 0.0f);
        _direction = glm::vec3(1.0f, 0.0f, 0.0f);
        _dirAngle = 0.0f;
        recomputeOrientation();
    }
    void moveForward(GLfloat movementFactor) override;
    void moveBackward(GLfloat movementFactor) override;
    void recomputeOrientation() override;

    void turnLeft(GLfloat movementFactor);
    void turnRight(GLfloat movementFactor);
    void zoom(GLfloat movementFactor);
    void setPostion1(glm::vec3 position);

    glm::vec3 getDirection() { return _direction; };
    float getDirAngle() { return _dirAngle; };

private:
    float _dirAngle;

};


#endif //A3_ARCBALLCAM_H
