#include "BusRaycast.h"

#include "../Utils/Math.h"

#include "../Graphics/GraphicsManager.h"
#include "../Utils/ResourceManager.h"

#include "../Utils/Helpers.hpp"
#include "../Utils/Strings.h"


BusRaycast::BusRaycast(SceneManager* smgr, PhysicsManager* pmgr, SoundManager* sndMgr, std::string filename)
    : _sMgr(smgr), _pMgr(pmgr), _sndMgr(sndMgr),
    _maxSteerAngle(0.55f), _steerAngle(0.0f),
    _steeringWheelObject(NULL), _driverPosition(0.0f, 0.0f, 0.0f),
    _brake(false), _accelerate(false), _handbrake(true), _idle(true),
    _isEnableLights(false), _isEnableHeadlights(false),
    _collidesWith(COL_TERRAIN | COL_ENV)
{
    loadXMLdata(filename);

    /*_modules.push_back(BusRayCastModule());

    _modules[0].sceneObject = _sMgr->addSceneObject("busRayCast");
    _modules[0].sceneObject->setPosition(0,5,0);

    RModel* busModel = ResourceManager::getInstance().loadModel("Buses\\h9\\h9.3ds", "Buses\\h9\\PKS\\");
    GraphicsManager::getInstance().addRenderObject(new RenderObject(busModel), _modules[0].sceneObject);

    PhysicalBodyRaycastVehicle* v = _pMgr->createPhysicalBodyRayCastVehicle(busModel->getCollisionMesh(), busModel->getCollisionMeshSize(), 800.0f, COL_BUS, COL_TERRAIN | COL_ENV);
    v->setWheelCollisionFilter(COL_WHEEL, COL_TERRAIN | COL_ENV);
    v->getRigidBody()->setActivationState( DISABLE_DEACTIVATION );
    _modules[0].sceneObject->addComponent(v);
    _modules[0].rayCastVehicle = v;

    _wheels.push_back(new BusRayCastWheel());
    _wheels.push_back(new BusRayCastWheel());
    _wheels.push_back(new BusRayCastWheel());
    _wheels.push_back(new BusRayCastWheel());

    _wheels[0]->wheel = _pMgr->createPhysicalBodyWheel(v, btVector3(1.0f, -0.45f, 2.6f), 0.6f, 0.5, true);
    _wheels[1]->wheel = _pMgr->createPhysicalBodyWheel(v, btVector3(-1.0f, -0.45f, 2.6f), 0.6f, 0.5, true);
    _wheels[2]->wheel = _pMgr->createPhysicalBodyWheel(v, btVector3(1.0f, -0.45f, -1.85f), 0.6f, 0.5, false);
    _wheels[3]->wheel = _pMgr->createPhysicalBodyWheel(v, btVector3(-1.0f, -0.45f, -1.85), 0.6f, 0.5, false);

    _wheels[0]->steering = true;
    _wheels[1]->steering = true;
    _wheels[2]->steering = false;
    _wheels[3]->steering = false;

    _wheels[0]->powered = false;
    _wheels[1]->powered = false;
    _wheels[2]->powered = true;
    _wheels[3]->powered = true;

    _wheels[0]->handbrake = false;
    _wheels[1]->handbrake = false;
    _wheels[2]->handbrake = true;
    _wheels[3]->handbrake = true;

    _wheels[0]->maxBrakeForce = 40;
    _wheels[1]->maxBrakeForce = 40;
    _wheels[2]->maxBrakeForce = 20;
    _wheels[3]->maxBrakeForce = 20;

    for (int i = 0; i < _wheels.size(); ++i)
	{
		btWheelInfo& wheel = _wheels[i]->wheel->getWheelInfo();

		// Original
		//wheel.m_suspensionStiffness = 20.0f;
		//wheel.m_wheelsDampingCompression = 4.472f;
		//wheel.m_wheelsDampingRelaxation = 3.578f;
		//wheel.m_frictionSlip = 1000.0f;
		//wheel.m_rollInfluence = 0.1f;

		wheel.m_suspensionStiffness = 35.0f;//25.0f;
		wheel.m_wheelsDampingCompression = 0.25f * 2 * sqrt(wheel.m_suspensionStiffness);
		wheel.m_wheelsDampingRelaxation = 0.3f * 2 * sqrt(wheel.m_suspensionStiffness);
		wheel.m_frictionSlip = 0.8f;
		wheel.m_rollInfluence = 0.1f;
	}

	RModel* wheelModel = ResourceManager::getInstance().loadModel("Buses\\h9\\fwheel.3ds", "Buses\\h9\\PKS\\");
	SceneObject* wheel1Object = _sMgr->addSceneObject("wheel1");
	GraphicsManager::getInstance().addRenderObject(new RenderObject(wheelModel), wheel1Object);
	wheel1Object->addComponent(_wheels[0]->wheel);

	SceneObject* wheel2Object = _sMgr->addSceneObject("wheel2");
	GraphicsManager::getInstance().addRenderObject(new RenderObject(wheelModel), wheel2Object);
	wheel2Object->addComponent(_wheels[1]->wheel);

	SceneObject* wheel3Object = _sMgr->addSceneObject("wheel3");
	GraphicsManager::getInstance().addRenderObject(new RenderObject(wheelModel), wheel3Object);
	wheel3Object->addComponent(_wheels[2]->wheel);

	SceneObject* wheel4Object = _sMgr->addSceneObject("wheel4");
	GraphicsManager::getInstance().addRenderObject(new RenderObject(wheelModel), wheel4Object);
	wheel4Object->addComponent(_wheels[3]->wheel);

	_gearbox = std::unique_ptr<Gearbox> (new Gearbox("gbx_ikarus260"));
    _engine = std::unique_ptr<Engine> (new Engine("eng_wskmielec"));

    if (_engine->getSoundFilename() != "")
    {
        SoundComponent* soundComp = new SoundComponent(_engine->getSoundFilename(), EST_PLAYER, true);
        //_sceneObject->addComponent(soundComp);
        _modules[0].sceneObject->addComponent(soundComp);
        soundComp->setGain(_engine->getSoundVolume());

        _sndMgr->addSoundComponent(soundComp);
    }

    _driverPosition = glm::vec3(0.75f, 0.44f, 3.51f);*/
}


BusRaycast::~BusRaycast()
{
    std::cout << "Bus Destruktor" << std::endl;

    std::vector<BusRayCastWheel*>::iterator wit = _wheels.begin();

    for (; wit != _wheels.end(); ++wit)
    {
        delete (*wit);
    }
    _wheels.clear();

    DoorList::iterator dit = _doors.begin();

    for (; dit != _doors.end(); ++dit)
    {

        delete (*dit);
    }
    _doors.clear();

    _headlights.clear();
}


void BusRaycast::loadXMLdata(std::string busname)
{
    std::string filename = "Buses/" + busname + "/config.xml";

    XMLDocument doc;
    doc.LoadFile( filename.c_str() );

    XMLElement* objElement = doc.FirstChildElement("Gearbox");

    std::string gearboxFile(objElement->Attribute("model"));
    _gearbox = std::unique_ptr<Gearbox> (new Gearbox(gearboxFile));

    objElement = doc.FirstChildElement("Engine");
    std::string engineFile(objElement->Attribute("model"));
    _engine = std::unique_ptr<Engine> (new Engine(engineFile));

    XMLElement* busElement = doc.FirstChildElement("Bus");

    std::cout << "XML DATA" << std::endl;

    std::string sObjName(busElement->Attribute("name"));


    std::string sTextureFolder(busElement->Attribute("textures"));

    std::cout<< sObjName << std::endl;

    RModel* busModel = 0;

    glm::vec3 busPosition = glm::vec3(0,0,0);
    //glm::vec3 busRotation = glm::vec3(0,0,0);

    std::string texturePath = "Buses/" + busname + "/" + sTextureFolder + "/";


    BusRayCastModule busModule;



    // Reading bus body configuration
    XMLElement* moduleElement = busElement->FirstChildElement("Module");

    while (moduleElement != nullptr)
    {

        //_sceneObject = _sMgr->addSceneObject(sObjName);
        busModule.sceneObject = _sMgr->addSceneObject(sObjName);

        std::string modelFile = std::string(moduleElement->Attribute("model"));
        std::string modelPath = "Buses/" + busname + "/" + modelFile;

        glm::vec3 modulePosition = glm::vec3(0,0,0);
        modulePosition = XMLstringToVec3(moduleElement->Attribute("position"));

        busModule.sceneObject->setPosition(modulePosition);

        busModel = ResourceManager::getInstance().loadModel(modelPath, texturePath);
        //GraphicsManager::getInstance().addRenderObject(new RenderObject(busModel), _sceneObject);
        GraphicsManager::getInstance().addRenderObject(new RenderObject(busModel), busModule.sceneObject);

        //_sceneObject->addComponent(renderObj);

        // Tworzenie fizycznego obiektu karoserii
        const char* cMass = moduleElement->Attribute("mass");
        float fMass = (float)atof(cMass);

        if (busModel->getCollisionMeshSize() > 0)
        {
            int wheelCollidesWith = COL_TERRAIN | COL_ENV;

            busModule.rayCastVehicle = _pMgr->createPhysicalBodyRayCastVehicle(busModel->getCollisionMesh(), busModel->getCollisionMeshSize(), fMass, COL_BUS, _collidesWith);
            busModule.rayCastVehicle->setWheelCollisionFilter(COL_WHEEL, wheelCollidesWith);
            busModule.rayCastVehicle->getRigidBody()->setActivationState( DISABLE_DEACTIVATION );

            //_sceneObject->addComponent(_chasisBody);
            busModule.sceneObject->addComponent(busModule.rayCastVehicle);

            //btVector3 dirVec(0,0,1);
            //btTransform tmpTransf = _chasisBody->getRigidBody()->getWorldTransform();
            //btVector3 dir = tmpTransf.getBasis() * dirVec;
        }
        else
        {
            std::cout << "ERROR: Collision mesh not found in bus model!\n" << std::endl;
            return;
        }

        // ########### WHEELS ###########
        XMLElement* wheelElement = moduleElement->FirstChildElement("Wheel");
        while (wheelElement != nullptr)
        {
            std::cout << "XML: Wheel data" << std::endl;

            std::string wheelName(wheelElement->Attribute("name"));
            std::cout << wheelName << std::endl;
            std::string wheelModel(wheelElement->Attribute("model"));
            std::string side(wheelElement->Attribute("side"));
            float radius = (float)atof(wheelElement->Attribute("radius"));

            int steering = (int)atoi(wheelElement->Attribute("steering"));
            int powered = (int)atoi(wheelElement->Attribute("powered"));
            int handbrake = (int)atoi(wheelElement->Attribute("handbrake"));

            float suspensionRestLength = (float)atof(wheelElement->Attribute("suspensionRestLength"));
            float suspensionStiffness = (float)atof(wheelElement->Attribute("suspensionStiffness"));
            float dampingCompression = (float)atof(wheelElement->Attribute("dampingCompression"));
            float dampingRelaxation = (float)atof(wheelElement->Attribute("dampingRelaxation"));
            float frictionSlip = (float)atof(wheelElement->Attribute("frictionSlip"));
            float rollInfluence = (float)atof(wheelElement->Attribute("rollInfluence"));
            float brakeForce = (float)atof(wheelElement->Attribute("brakeForce"));

            glm::vec3 wheelPosition = XMLstringToVec3(wheelElement->Attribute("position"));

            SceneObject* wheelObj = _sMgr->addSceneObject(wheelName);

            // obracamy model kola je¿li jest po lewej stronie
            WheelSide wheelSide;
            if (side == "right")
            {
                wheelSide = WS_RIGHT;
            }
            else
                wheelSide = WS_LEFT;

            btVector3 btWheelPos(wheelPosition.x, wheelPosition.y, wheelPosition.z);

            std::string modelPath = "Buses/" + busname + "/" + wheelModel;
            RModel* wheel = ResourceManager::getInstance().loadModel(modelPath, texturePath);
            GraphicsManager::getInstance().addRenderObject(new RenderObject(wheel), wheelObj);

            //wheelObj->addComponent(wheelRender);

            PhysicalBodyWheel* wheelBody = _pMgr->createPhysicalBodyWheel(busModule.rayCastVehicle, btWheelPos, suspensionRestLength, radius, steering);
            wheelObj->addComponent(wheelBody);

            btWheelInfo& wheelInfo = wheelBody->getWheelInfo();
            wheelInfo.m_suspensionStiffness = suspensionStiffness;
            wheelInfo.m_wheelsDampingCompression = dampingCompression * 2 * sqrt(wheelInfo.m_suspensionStiffness);
            wheelInfo.m_wheelsDampingRelaxation = dampingRelaxation * 2 * sqrt(wheelInfo.m_suspensionStiffness);
            wheelInfo.m_frictionSlip = frictionSlip;
            wheelInfo.m_rollInfluence = rollInfluence;

            BusRayCastWheel* w = new BusRayCastWheel;
            w->wheel = wheelBody;
            w->steering = steering;
            w->powered = powered;
            w->handbrake = handbrake;
            w->maxBrakeForce = brakeForce;
            w->wheelSide = wheelSide;

            _wheels.push_back(w);

            wheelElement = wheelElement->NextSiblingElement("Wheel");
        }

        // ########### INTERIOR LIGHTS ###########
        XMLElement* lightElement = moduleElement->FirstChildElement("Light");
        while (lightElement != nullptr)
        {
            //std::cout << "XML: Light" << std::endl;

            const char* cPosition = lightElement->Attribute("position");
            glm::vec3 position = XMLstringToVec3(cPosition);

            const char* cColor = lightElement->Attribute("color");
            glm::vec3 color = XMLstringToVec3(cColor);

            const char* cAttenuation = lightElement->Attribute("attenuation");
            glm::vec3 attenuation = XMLstringToVec3(cAttenuation);

            float ambientIntensity = (float)atof(lightElement->Attribute("ambientIntensity"));
            float diffuseIntensity = (float)atof(lightElement->Attribute("diffuseIntensity"));


            SceneObject* light = _sMgr->addSceneObject("busLight" + toString(_lights.size()));
            Light* lightComponent = GraphicsManager::getInstance().addPointLight(color, ambientIntensity,
                                                                                 diffuseIntensity,
                                                                                 LightAttenuation(attenuation.x, attenuation.y, attenuation.z));
            light->addComponent(lightComponent);
            light->setPosition(position);
            //_sceneObject->addChild(light);
            busModule.sceneObject->addChild(light);

            lightComponent->setIsActive(_isEnableLights);

            _lights.push_back(lightComponent);

            lightElement = lightElement->NextSiblingElement("Light");
        }

        // ########### DRIVER'S POSITION ###########
        XMLElement* driverPositionElement = busElement->FirstChildElement("Driver");
        if (driverPositionElement != nullptr)
        {
            const char* cPosition = driverPositionElement->Attribute("position");
            _driverPosition = XMLstringToVec3(cPosition);

            driverPositionElement = driverPositionElement->NextSiblingElement("Driver");
        }

        // ########### STEERING WHEEL ###########
        XMLElement* steeringWheelElement = moduleElement->FirstChildElement("SteeringWheel");
        if (steeringWheelElement != nullptr)
        {
            //std::cout << "XML: Steering wheel" << std::endl;

            std::string modelFile = std::string(steeringWheelElement->Attribute("model"));
            std::string modelPath = "Buses/" + busname + "/" + modelFile;

            _steeringWheelObject = _sMgr->addSceneObject("steeringWheel");

            RModel* steeringWheelModel = ResourceManager::getInstance().loadModel(modelPath, "./");
            GraphicsManager::getInstance().addRenderObject(new RenderObject(steeringWheelModel), _steeringWheelObject);

            const char* cPosition = steeringWheelElement->Attribute("position");
            glm::vec3 position = XMLstringToVec3(cPosition);
            _steeringWheelObject->setPosition(position);

            const char* cRotation = steeringWheelElement->Attribute("rotation");
            glm::vec3 rotation(XMLstringToVec3(cRotation));
            _steeringWheelObject->setRotation(glm::vec3(rotation.x * PI, rotation.y * PI, rotation.z * PI) );

            const char* cScale = steeringWheelElement->Attribute("scale");
            glm::vec3 scale(XMLstringToVec3(cScale));
            _steeringWheelObject->setScale(scale);

            //_sceneObject->addChild(_steeringWheelObject);
            busModule.sceneObject->addChild(_steeringWheelObject);
        }

        // ########### HEADLIGHTS ###########
        XMLElement* headlightElement = moduleElement->FirstChildElement("Headlight");
        while (headlightElement != nullptr)
        {
            //std::cout << "XML: Headlights" << std::endl;

            std::string headlightName(headlightElement->Attribute("name"));
            const char* cPosition = headlightElement->Attribute("position");
            glm::vec3 position = XMLstringToVec3(cPosition);

            const char* cRotation = headlightElement->Attribute("rotation");
            glm::vec3 rotation = XMLstringToVec3(cRotation);

            const char* cColor = headlightElement->Attribute("color");
            glm::vec3 color = XMLstringToVec3(cColor);

            const char* cAttenuation = headlightElement->Attribute("attenuation");
            glm::vec3 attenuation = XMLstringToVec3(cAttenuation);

            float ambientIntensity = (float)atof(headlightElement->Attribute("ambientIntensity"));
            float diffuseIntensity = (float)atof(headlightElement->Attribute("diffuseIntensity"));
            float cutoff = (float)atof(headlightElement->Attribute("cutoff"));


            SceneObject* light = _sMgr->addSceneObject(headlightName);
            Light* lightComponent = GraphicsManager::getInstance().addSpotLight(color, ambientIntensity,
                                                                                diffuseIntensity, cutoff,
                                                                                LightAttenuation(attenuation.x, attenuation.y, attenuation.z));
            light->addComponent(lightComponent);
            light->setPosition(position);
            light->setRotation(rotation);
            //_sceneObject->addChild(light);
            busModule.sceneObject->addChild(light);

            lightComponent->setIsActive(_isEnableHeadlights);

            _headlights.push_back(lightComponent);

            headlightElement = headlightElement->NextSiblingElement("Headlight");
        }

        // ########### DOORS ###########
        XMLElement* doorElement = moduleElement->FirstChildElement("Door");
        while (doorElement != nullptr)
        {
            std::cout << "XML: Door data" << std::endl;

            std::string doorName(doorElement->Attribute("name"));
            std::string doorType(doorElement->Attribute("type"));
            std::string doorModel(doorElement->Attribute("model"));
            float mass = (float)atof(doorElement->Attribute("mass"));
            char group = (char)atoi(doorElement->Attribute("group"));

            std::string openSound = "Buses/" + busname + "/" + std::string(doorElement->Attribute("doorOpenSound"));
            std::string closeSound = "Buses/" + busname + "/" + std::string(doorElement->Attribute("doorCloseSound"));

            std::cout << "Door open sound:" << openSound << std::endl;
            std::cout << "Door open sound:" << closeSound <<std::endl;


            // Create sound component
            SoundComponent* openSoundComp = new SoundComponent(openSound, EST_PLAYER);
            _sndMgr->addSoundComponent(openSoundComp);

            SoundComponent* closeSoundComp = new SoundComponent(closeSound, EST_PLAYER);
            _sndMgr->addSoundComponent(closeSoundComp);

            SceneObject* doorObj = 0;

            if (doorType == "s")
            {
                glm::vec3 doorPosition = XMLstringToVec3(doorElement->Attribute("position"));
                glm::vec3 relativePos = busModule.sceneObject->transformLocalPointToGlobal(doorPosition);

                // poczatek IF
                btVector3 busPivot = XMLstringToBtVec3(doorElement->Attribute("pivotA"));
                btVector3 doorPivot = XMLstringToBtVec3(doorElement->Attribute("pivotB"));

                doorObj = _sMgr->addSceneObject(doorName);
                doorObj->setPosition(doorPosition);

                std::string modelPath = "Buses/" + busname + "/" + doorModel;

                RModel* dr = ResourceManager::getInstance().loadModel(modelPath, texturePath);
                GraphicsManager::getInstance().addRenderObject(new RenderObject(dr), doorObj);

                btVector3 btDoorPos(relativePos.x, relativePos.y, relativePos.z);

                int collidesWith = COL_ENV | COL_TERRAIN;
                PhysicalBodyConvexHull* doorBody = _pMgr->createPhysicalBodyConvexHull(dr->getCollisionMesh(), dr->getCollisionMeshSize(), mass, COL_DOOR, collidesWith);
                doorObj->addComponent(doorBody);

                //ConstraintHinge* doorHinge = _pMgr->createConstraintHinge(_chasisBody, doorBody, busPivot, doorPivot, btVector3(0,1,0), btVector3(0,1,0));
                ConstraintHinge* doorHinge = _pMgr->createConstraintHinge(busModule.rayCastVehicle, doorBody, busPivot, doorPivot, btVector3(0,1,0), btVector3(0,1,0));

                doorHinge->getBulletConstraint()->setLimit(-1.5,0);

                Door* d = 0;
                d = new DoorSimple(dr, doorBody, doorHinge, openSoundComp, closeSoundComp, group);
                d->close();
                _doors.push_back(d);

            } // IF "S"
            else
            if (doorType == "se")
            {
                std::string armName(doorElement->Attribute("armName"));
                std::string armModel(doorElement->Attribute("arm"));
                float armMass = (float)atof(doorElement->Attribute("armMass"));
                char group = (char)atoi(doorElement->Attribute("group"));

                glm::vec2 arm1limits = XMLstringToVec2(doorElement->Attribute("arm1limits"));

                std::string rotDir(doorElement->Attribute("rotationDir"));

                RotationDir rdir;

                if (rotDir == "CCW")
                    rdir = ERD_CCW;
                else
                    rdir = ERD_CW;

                glm::vec3 armPosition = XMLstringToVec3(doorElement->Attribute("armPosition"));
                glm::vec3 armRelPos = glm::vec3(busPosition.x + armPosition.x, busPosition.y + armPosition.y, busPosition.z + armPosition.z);

                btVector3 armPivotA = XMLstringToBtVec3(doorElement->Attribute("armPivotA"));
                btVector3 armPivotB = XMLstringToBtVec3(doorElement->Attribute("armPivotB"));

                SceneObject* armObj = _sMgr->addSceneObject(armName);
                armObj->setPosition(armRelPos);

                std::string armPath = "Buses/" + busname + "/" + armModel;

                RModel* arm = ResourceManager::getInstance().loadModel(armPath, texturePath);
                GraphicsManager::getInstance().addRenderObject(new RenderObject(arm), armObj);

                btVector3 btArmPos(armRelPos.x, armRelPos.y, armRelPos.z);

                int collidesWith = COL_ENV | COL_TERRAIN;
                PhysicalBodyConvexHull* armBody = _pMgr->createPhysicalBodyConvexHull(arm->getCollisionMesh(), arm->getCollisionMeshSize(), armMass, COL_DOOR, collidesWith);
                armObj->addComponent(armBody);

                ConstraintHinge* busArmHinge = _pMgr->createConstraintHinge(busModule.rayCastVehicle, armBody, armPivotA, armPivotB, btVector3(0,1,0), btVector3(0,1,0));

                busArmHinge->getBulletConstraint()->setLimit(arm1limits.x,arm1limits.y);

                // arm 2

                std::string arm2Name(doorElement->Attribute("arm2Name"));
                std::string arm2Model(doorElement->Attribute("arm2"));
                float arm2Mass = (float)atof(doorElement->Attribute("arm2Mass"));

                glm::vec3 arm2Position = XMLstringToVec3(doorElement->Attribute("arm2Position"));
                glm::vec3 arm2RelPos = glm::vec3(busPosition.x + arm2Position.x, busPosition.y + arm2Position.y, busPosition.z + arm2Position.z);

                btVector3 arm2PivotA = XMLstringToBtVec3(doorElement->Attribute("arm2PivotA"));
                btVector3 arm2PivotB = XMLstringToBtVec3(doorElement->Attribute("arm2PivotB"));

                SceneObject* arm2Obj = _sMgr->addSceneObject(arm2Name);
                arm2Obj->setPosition(arm2RelPos);

                std::string arm2Path = "Buses/" + busname + "/" + arm2Model;

                RModel* arm2 = ResourceManager::getInstance().loadModel(arm2Path, texturePath);
                GraphicsManager::getInstance().addRenderObject(new RenderObject(arm2), arm2Obj);

                btVector3 btArm2Pos(arm2RelPos.x, arm2RelPos.y, arm2RelPos.z);

                collidesWith = COL_NOTHING;
                PhysicalBodyConvexHull* arm2Body = _pMgr->createPhysicalBodyConvexHull(arm2->getCollisionMesh(), arm2->getCollisionMeshSize(), arm2Mass, COL_DOOR, collidesWith);
                arm2Obj->addComponent(arm2Body);

                _pMgr->createConstraintHinge(busModule.rayCastVehicle, arm2Body, arm2PivotA, arm2PivotB, btVector3(0,1,0), btVector3(0,1,0));

                // door model

                std::string doorName(doorElement->Attribute("name"));
                std::string doorModel(doorElement->Attribute("model"));
                float doorMass = (float)atof(doorElement->Attribute("mass"));

                glm::vec3 doorPosition = XMLstringToVec3(doorElement->Attribute("position"));
                glm::vec3 relativePos = glm::vec3(armPosition.x + doorPosition.x, armPosition.y + doorPosition.y, armPosition.z + doorPosition.z);

                btVector3 doorPivotA = XMLstringToBtVec3(doorElement->Attribute("pivotA"));
                btVector3 doorPivotB = XMLstringToBtVec3(doorElement->Attribute("pivotB"));

                doorObj = _sMgr->addSceneObject(doorName);
                doorObj->setPosition(relativePos);

                std::string doorPath = "Buses/" + busname + "/" + doorModel;

                RModel* door = ResourceManager::getInstance().loadModel(doorPath, texturePath);
                GraphicsManager::getInstance().addRenderObject(new RenderObject(door), doorObj);

                btVector3 btDoorPos(relativePos.x, relativePos.y, relativePos.z);

                collidesWith = COL_ENV;
                PhysicalBodyConvexHull* doorBody = _pMgr->createPhysicalBodyConvexHull(door->getCollisionMesh(), door->getCollisionMeshSize(), doorMass, COL_DOOR, collidesWith);
                doorObj->addComponent(doorBody);

                ConstraintHinge* armDoorHinge = _pMgr->createConstraintHinge(armBody, doorBody, doorPivotA, doorPivotB, btVector3(0,1,0), btVector3(0,1,0));

                if (rotDir == "CCW")
                    armDoorHinge->getBulletConstraint()->setLimit(-1.9,0.0);
                else
                    armDoorHinge->getBulletConstraint()->setLimit(0.0,1.9);

                btVector3 pivotC = XMLstringToBtVec3(doorElement->Attribute("pivotC"));
                btVector3 pivotD = XMLstringToBtVec3(doorElement->Attribute("pivotD"));

                _pMgr->createConstraintHinge(arm2Body, doorBody, pivotC, pivotD, btVector3(0,1,0), btVector3(0,1,0));


                Door* d = 0;
                d = new DoorSE(0, 0, arm, armBody, busArmHinge, 0, openSoundComp, closeSoundComp, rdir, group);
                d->close();
                _doors.push_back(d);
            }


            doorObj->addComponent(openSoundComp);
            doorObj->addComponent(closeSoundComp);

            doorElement = doorElement->NextSiblingElement("Door");
        }


        // Modules connection point
        XMLElement* jointElement = moduleElement->FirstChildElement("Joint");
        while (jointElement != nullptr)
        {
            busModule.jointPosition = XMLstringToBtVec3(jointElement->Attribute("position"));

            jointElement = jointElement->NextSiblingElement("Joint");
        }

        _modules.push_back(busModule);

        moduleElement = moduleElement->NextSiblingElement("Module");


    }

    // Create Sound Component if sound filename is defined in Engine XML config file
    if (_engine->getSoundFilename() != "")
    {
        SoundComponent* soundComp = new SoundComponent(_engine->getSoundFilename(), EST_PLAYER, true);
        //_sceneObject->addComponent(soundComp);
        _modules[0].sceneObject->addComponent(soundComp);
        soundComp->setGain(_engine->getSoundVolume());
        soundComp->setPlayDistance(20.0f);

        _sndMgr->addSoundComponent(soundComp);
    }

    // If it's articulated bus - connect modules with a ball joint
    if (_modules.size() > 1)
    {
        _pMgr->createConstraintBall(_modules[0].rayCastVehicle, _modules[1].rayCastVehicle, _modules[0].jointPosition, _modules[1].jointPosition);
    }

}


BusRayCastModule& BusRaycast::getModule(int index)
{
    return _modules[index];
}


glm::vec3 BusRaycast::getDriverPosition()
{
    return _driverPosition;
}


SceneObject* BusRaycast::getSteeringWheelObject()
{
    return _steeringWheelObject;
}


void BusRaycast::setIsEnableLights(bool is)
{
    if (_lights.size() > 0)
    {
        for (LightsList::iterator i = _lights.begin(); i != _lights.end(); ++i)
        {
            (*i)->setIsActive(is);
        }
        _isEnableLights = is;
    }
}


bool BusRaycast::isEnableLights()
{
    return _isEnableLights;
}


void BusRaycast::setIsEnableHeadlights(bool is)
{
    if (_headlights.size() > 0)
    {
        for (LightsList::iterator i = _headlights.begin(); i != _headlights.end(); ++i)
        {
            (*i)->setIsActive(is);
        }

        _isEnableHeadlights = is;
    }
}


bool BusRaycast::isEnableHeadlights()
{
    return _isEnableHeadlights;
}


void BusRaycast::turnLeft(float dt)
{
    if (_steerAngle < _maxSteerAngle)
    {
        _steerAngle += dt * STEER_STEP;

        if (_steeringWheelObject)
        {
            _steeringWheelObject->rotate(0.0f, 2.5f * dt * STEER_STEP * 2 * PI / _maxSteerAngle, 0.0f);
        }
    }

    std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();
    for (; it != _wheels.end(); ++it)
    {
        BusRayCastWheel* wheel = (*it);

        if ( wheel->steering )
        {
            wheel->wheel->setSteeringValue(_steerAngle);
        }
    }
}


void BusRaycast::turnRight(float dt)
{
    if (_steerAngle > -_maxSteerAngle)
    {
        _steerAngle -= dt * STEER_STEP;

        if (_steeringWheelObject)
        {
            _steeringWheelObject->rotate(0.0f, 2.5f * -dt * STEER_STEP * 2 * PI / _maxSteerAngle, 0.0f);
        }
    }

    std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();
    for (; it != _wheels.end(); ++it)
    {
        BusRayCastWheel* wheel = (*it);

        if ( wheel->steering )
        {
            wheel->wheel->setSteeringValue(_steerAngle);
        }
    }
}


void BusRaycast::centerSteringWheel(float dt)
{
    if (_steerAngle > DEAD_ZONE)
    {
        _steerAngle -= dt * STEER_STEP;

        if (_steeringWheelObject)
        {
            _steeringWheelObject->rotate(0.0f, 2.5f * -dt * STEER_STEP * 2 * PI / _maxSteerAngle, 0.0f);
        }
    }
    else if (_steerAngle < -DEAD_ZONE)
    {
        _steerAngle += dt * STEER_STEP;

        if (_steeringWheelObject)
        {
            _steeringWheelObject->rotate(0.0f, 2.5f * dt * STEER_STEP * 2 * PI / _maxSteerAngle, 0.0f);
        }
    }

    std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();
    for (; it != _wheels.end(); ++it)
    {
        BusRayCastWheel* wheel = (*it);

        if ( wheel->steering )
        {
            wheel->wheel->setSteeringValue(_steerAngle);
        }
    }
}


void BusRaycast::accelerate()
{
    _accelerate = true;
    _brake = false;

    _engine->throttleUp();
}


void BusRaycast::idle()
{
    _accelerate = false;
    _brake = false;

    _engine->throttleDown();
}


void BusRaycast::brakeOn()
{
    _accelerate = false;
    _brake = true;

    _engine->throttleDown();
}


void BusRaycast::brakeOff()
{
    _brake = false;
}


void BusRaycast::toggleHandbrake()
{
    _handbrake = !_handbrake;
}


Gearbox* BusRaycast::getGearbox()
{
    if (_gearbox) return _gearbox.get(); else return 0;
}


Engine* BusRaycast::getEngine()
{
    if (_engine) return _engine.get(); else return 0;
}


void BusRaycast::startEngine()
{
    if (_engine)
    {
        _engine->turnOn();

        //SoundComponent* sndC = dynamic_cast<SoundComponent*>(_sceneObject->getComponent(CT_SOUND));
        SoundComponent* sndC = dynamic_cast<SoundComponent*>(_modules[0].sceneObject->getComponent(CT_SOUND));
        sndC->play();
    }
}


void BusRaycast::stopEngine()
{
    if (_engine)
    {
        _engine->turnOff();

        SoundComponent* sndC = dynamic_cast<SoundComponent*>(_modules[0].sceneObject->getComponent(CT_SOUND));
        sndC->stop();
    }
}



void BusRaycast::doorOpenClose(char doorGroup)
{
    for (unsigned char i = 0; i < _doors.size(); i++)
    {
        if (_doors[i]->getGroup() == doorGroup)
        {
            if (_doors[i]->getState() == EDS_CLOSING)
            {
                _doors[i]->open();
            }
            else
            if (_doors[i]->getState() == EDS_OPENING)
            {
                _doors[i]->close();
            }
        }
    }
}


Door* BusRaycast::getDoor(unsigned char doorIndex)
{
    _doors[doorIndex];
}


void BusRaycast::update(float deltaTime)
{
    _engine->update(deltaTime);

    SoundComponent* sndC = dynamic_cast<SoundComponent*>(_modules[0].sceneObject->getComponent(CT_SOUND));
    sndC->setPitch(_engine->getCurrentRPM() / 1000);

    btScalar wheelAngularVelocity = 0.0f;

    // przyspieszanie
    if (_accelerate)
    {
        std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            BusRayCastWheel* wheel = (*it);

            if ( wheel->powered )
            {
                wheel->wheel->applyEngineForce(_gearbox->currentRatio() * _engine->getCurrentTorque() * 0.32f);;
            }
        }
    }
    // bus is braking
    else if (_brake)
    {
        std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            BusRayCastWheel* wheel = (*it);

            wheel->wheel->applyEngineForce(0.0f);
            wheel->wheel->setBrake(wheel->maxBrakeForce);
        }
    } // bus engine is idle
    else if (!_accelerate && !_brake && !_handbrake)
    {
        std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            BusRayCastWheel* wheel = (*it);

            wheel->wheel->applyEngineForce(0.0f);
            wheel->wheel->setBrake(0.13f * abs(_gearbox->currentRatio()) ); // we set a little damping to simulate air and rolling resistance
        }
    }
    else if (!_handbrake && !_brake)
    {
        std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            BusRayCastWheel* wheel = (*it);

            wheel->wheel->setBrake(0.0f);
        }
    } // bus is on handbrake
    if (_handbrake)
    {
        std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            BusRayCastWheel* wheel = (*it);

            if (wheel->handbrake)
            {
                wheel->wheel->setBrake(wheel->maxBrakeForce);
            }
        }
    }

    char poweredWheels = 0;

    // If we do not accelerate at the moment - we have to calculate and simulate engine braking
    // Get wheel linear speed to calculate RPM of the engine
    std::vector<BusRayCastWheel*>::iterator it = _wheels.begin();
    for (; it != _wheels.end(); ++it)
    {
        BusRayCastWheel* wheel = (*it);

        if ( wheel->powered )
        {
            wheelAngularVelocity += wheel->wheel->getWheelInfo().m_deltaRotation / deltaTime;
            poweredWheels++;
            //break;
        }
    }

    _engine->setRPM(wheelAngularVelocity/poweredWheels, _gearbox->currentRatio());
}