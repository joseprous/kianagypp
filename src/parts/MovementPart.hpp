#ifndef MOVEMENTPART_HPP
#define MOVEMENTPART_HPP

struct MovementPart
{
    //max speed
    float speedLeft, speedRight, speedForward, speedBackward, speedUp, speedDown;

    //current speed
    float left, right, forward, backward, up, down;
};

#endif // MOVEMENTPART_HPP
