#ifndef PHYSICALBODY_HPP_INCLUDED
#define PHYSICALBODY_HPP_INCLUDED

#include <bullet/btBulletDynamicsCommon.h>

#include "../Scene/Component.h"
#include "../Scene/SceneObject.h"

#include <memory>

class PhysicsManager;

#define BIT(x) (1<<(x))
enum collisiontypes {
    COL_NOTHING = 0, //<Collide with nothing
    COL_TERRAIN = BIT(0), //<Collide with terrain
    COL_BUS = BIT(1), //<Collide with bus chassis
    COL_DOOR = BIT(2), //<Collide with bus door
    COL_WHEEL = BIT(3), // Collide with bus wheel
    COL_ENV = BIT(4)
};

class PhysicalBody : public Component
{
    public:
        PhysicalBody(btScalar m, btVector3 pos = btVector3(0,0,0), btVector3 rot = btVector3(0,0,0));
        virtual ~PhysicalBody();

        btRigidBody* getRigidBody() { return _rigidBody.get(); }
        btDefaultMotionState* getMotionState() { return _motionState.get(); }
        btCollisionShape*   getCollisionShape() { return _collShape.get(); }

        btScalar getMass() { return _mass; }
        //void getTransform(btTransform& t);

        void setRotation(btVector3 rot);

        void setRestitution(btScalar rest) { _rigidBody->setRestitution(rest); }

        void update();


        virtual void changedTransform(glm::vec3 position, glm::vec3 rotation)
        {
            btTransform transf;
            transf.setIdentity();

            btQuaternion quat;
            quat.setEulerZYX(rotation.x, rotation.y, rotation.z);

            btVector3 pos(position.x, position.y, position.z);
            transf.setOrigin(pos);

            transf.setRotation(quat);
            _rigidBody->setCenterOfMassTransform(transf);
            _motionState->setWorldTransform(transf);

            _rigidBody->activate();
        }

    protected:
        std::unique_ptr<btRigidBody>            _rigidBody;
        std::unique_ptr<btCollisionShape>       _collShape;
        std::unique_ptr<btDefaultMotionState>   _motionState;
        btScalar                _mass;
        btVector3               _position;
        btVector3               _rotation;

        virtual void updateBody() { }
};

#endif // PHYSICALBODY_HPP_INCLUDED
