#ifndef DOORSE_H_INCLUDED
#define DOORSE_H_INCLUDED

#include "Door.h"

enum RotationDir
{
    ERD_CCW = 0,
    ERD_CW
};

class DoorSE : virtual public Door
{
    public:
        DoorSE(RModel* model, PhysicalBodyConvexHull* body,
               RModel* armModel, PhysicalBodyConvexHull* armBody, ConstraintHinge* constraint1, ConstraintHinge* constraint2,
               SoundComponent* openDoor, SoundComponent* closeDoor, RotationDir dir = ERD_CCW, char group = 1)
        : Door(model, body, openDoor, closeDoor, group),
        _armModel(armModel), _armBody(armBody), _constraint1(constraint1), _constraint2(constraint2), _rotationDir(dir)
        {
            close();
        }

        virtual ~DoorSE()
        {

        }

        void open()
        {
            if (_rotationDir == ERD_CCW)
                _constraint1->getBulletConstraint()->enableAngularMotor(true, -1.7f, 2.15f);
            else
                _constraint1->getBulletConstraint()->enableAngularMotor(true, 1.7f, 2.15f);

            _state = EDS_OPENING;
        }

        void close()
        {
            if (_rotationDir == ERD_CCW)
                _constraint1->getBulletConstraint()->enableAngularMotor(true, 1.7f, 2.15f);
            else
                _constraint1->getBulletConstraint()->enableAngularMotor(true, -1.7f, 2.15f);

            _state = EDS_CLOSING;
        }

        RModel* getArmModel() { return _armModel; }
        PhysicalBodyConvexHull* getArmBody() { return _armBody; }

        ConstraintHinge* getConstraint1() { return _constraint1; }

    protected:
        RModel*                 _armModel;
        PhysicalBodyConvexHull* _armBody;
        ConstraintHinge*        _constraint1;   // bus - arm
        ConstraintHinge*        _constraint2;   // arm - door
        RotationDir             _rotationDir;
};

#endif // DOORSE_H_INCLUDED
