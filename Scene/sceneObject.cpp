#include "SceneObject.h"
#include "SceneManager.h"


SceneObject::SceneObject(std::string name, SceneManager* sceneManager, SceneObject* parent)
    : _parent(parent),
    _id(0), _name(name), _isActive(true),
    _sceneManager(sceneManager),
    _rotationMode(RM_QUATERNION),
    _position(0.0f, 0.0f, 0.0f),
    _rotation(0.0f, 0.0f, 0.0f),
    _scale(1.0f, 1.0f, 1.0f)
    //_transform(this), _globalTransform(this), _transformIsChanged(true)
{
    if (_parent != NULL)
    {
        _parent->addChild(this);
    }

    changedTransform();

    //#ifdef _DEBUG_MODE
        std::cout << "Create SceneObject: " << _name << std::endl ;
    //#endif // _DEBUG_MODE
}


SceneObject::~SceneObject()
{
    //#ifdef _DEBUG_MODE
        std::cout << "Destroy SceneObject: " << _name << std::endl;
    //#endif // _DEBUG_MODE

    for (std::vector<Component*>:: iterator i = _components.begin(); i != _components.end(); ++i)
    {
        /* USUWANIE Z POSZCZEGOLNYCH PODSYSTEMOW */
        switch ((*i)->getType())
        {
            case CT_RENDER_OBJECT:
                //_sceneManager->getGraphicsManager()->removeRenderObject(static_cast<RenderObject*>(*i));
                GraphicsManager::getInstance().removeRenderObject(static_cast<RenderObject*>(*i));
                break;

            case CT_CAMERA:
                //_sceneManager->getGraphicsManager()->removeCamera(static_cast<CameraStatic*>(*i));
                GraphicsManager::getInstance().removeCamera(static_cast<CameraStatic*>(*i));
                break;

            case CT_LIGHT:
                //_sceneManager->getGraphicsManager()->removeLight(static_cast<Light*>(*i));
                GraphicsManager::getInstance().removeLight(static_cast<Light*>(*i));
                break;

            case CT_PHYSICAL_BODY:
                //_sceneManager->getPhysicsManager()->removePhysicalBody(static_cast<PhysicalBody*>(*i));
                //PhysicsManager::getInstance().removePhysicalBody(static_cast<PhysicalBody*>(*i));
                break;

            case CT_TREE_COMPONENT:
                delete *i;
                break;

        }

        /* ------------------------------------- */
    }
}


void SceneObject::changedTransform()
{
    _localTransformMatrixIsCalculated = false;
    _localNormalMatrixIsCalculated = false;
    _globalTransformMatrixIsCalculated = false;
    _globalNormalMatrixIsCalculated = false;

    for (std::list<SceneObject*>::iterator i = _childrens.begin(); i != _childrens.end(); ++i)
    {
        (*i)->changedTransform();
    }

    for (std::vector<Component*>::iterator i = _components.begin(); i != _components.end(); ++i)
    {
        (*i)->changedTransform();
    }
}


void SceneObject::calculateLocalTransformMatrix() const
{
    glm::mat4 pos = glm::translate(_position);

    glm::mat4 rot;

    if (_rotationMode == RM_EULER_ANGLES)
    {
        rot = glm::rotate(_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        rot *= glm::rotate(_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        rot *= glm::rotate(_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    }
    else
    {
        rot = glm::mat4_cast(_rotationQuaternion);
    }

    glm::mat4 s = glm::scale(_scale);

    _localTransformMatrix = pos * rot * s;
}


void SceneObject::calculateGlobalTransformMatrix() const
{
    _globalTransformMatrix = getLocalTransformMatrix();

    if (_parent != NULL)
    {
        _globalTransformMatrix = _parent->getGlobalTransformMatrix() * _globalTransformMatrix;
    }
}


void SceneObject::removeParent()
{
    if (_parent != NULL)
    {
        _parent->removeChild(this);
        _parent = NULL;
    }
}


bool SceneObject::hasParent()
{
    return static_cast<bool>(_parent);
}


SceneObject* SceneObject::getParent()
{
    return _parent;
}


void SceneObject::addChild(SceneObject* child)
{
    if (child == NULL)
    {
        return;
    }

    _childrens.push_back(child);

    child->removeParent();
    child->_parent = this;

    //child->setIsActive(_isActive);
}


bool SceneObject::removeChild(SceneObject* child)
{
    if (child == NULL)
    {
        return false;
    }

    for (std::list<SceneObject*>::iterator i = _childrens.begin(); i != _childrens.end(); ++i)
    {
        if ((*i) == child)
        {
            child->_parent = NULL;
            _childrens.erase(i);

            return true;
        }
    }

    return false;
}


void SceneObject::removeAllChildren()
{
    for (std::list<SceneObject*>::iterator i = _childrens.begin(); i != _childrens.end(); ++i)
    {
        (*i)->_parent = NULL;
    }

    _childrens.clear();
}


void SceneObject::addComponent(Component* component)
{
    if (component != NULL)
    {
        component->setSceneObject(this);

        _components.push_back(component);

        component->changedTransform();
    }
}


void SceneObject::removeComponent(Component* component)
{
    for (std::vector<Component*>::iterator i = _components.begin(); i != _components.end(); ++i)
    {
        if (*i == component)
        {
            i = _components.erase(i);

            /* USUWANIE Z POSZCZEGOLNYCH PODSYSTEMOW */
            switch ((*i)->getType())
            {
                case CT_RENDER_OBJECT:
                    //_sceneManager->getGraphicsManager()->removeRenderObject(static_cast<RenderObject*>(*i));
                    GraphicsManager::getInstance().removeRenderObject(static_cast<RenderObject*>(*i));
                    break;

                case CT_CAMERA:
                    //_sceneManager->getGraphicsManager()->removeCamera(static_cast<CameraStatic*>(*i));
                    GraphicsManager::getInstance().removeCamera(static_cast<CameraStatic*>(*i));
                    break;

                case CT_LIGHT:
                    //_sceneManager->getGraphicsManager()->removeLight(static_cast<Light*>(*i));
                    GraphicsManager::getInstance().removeLight(static_cast<Light*>(*i));
                    break;

                case CT_PHYSICAL_BODY:
                    //_sceneManager->getPhysicsManager()->removePhysicalBody(static_cast<PhysicalBody*>(*i));
                    //PhysicsManager::getInstance().removePhysicalBody(static_cast<PhysicalBody*>(*i));
                    break;

                case CT_TREE_COMPONENT:
                    delete *i;
                    break;

            }

            /* ------------------------------------- */

            return;
        }
    }
}


void SceneObject::setIsActive(bool is)
{
    _isActive = is;

    /*for (std::list<SceneObject*>::iterator i = _childrens.begin(); i != _childrens.end(); ++i)
    {
        (*i)->setIsActive(_isActive);
    }*/
}


std::string SceneObject::getName()
{
    return _name;
}

unsigned int SceneObject::getId()
{
    return _id;
}


Component* SceneObject::getComponent(unsigned int index)
{
    if (index < _components.size())
        return _components[index];

    return NULL;
}


Component* SceneObject::getComponent(ComponentType type)
{
    for (int i = 0; i < _components.size(); ++i)
    {
        if (_components[i]->getType() == type)
            return _components[i];
    }

    return NULL;
}


bool SceneObject::isActive()
{
    if (_parent != NULL)
        return _parent->isActive() && _isActive;

    return _isActive;
}


unsigned int SceneObject::getComponentsCount()
{
    return _components.size();
}


SceneManager* SceneObject::getSceneManager()
{
    return _sceneManager;
}


void SceneObject::setPosition(glm::vec3 position)
{
    _position = position;

    changedTransform();
}


void SceneObject::setPosition(float x, float y, float z)
{
    setPosition(glm::vec3(x, y, z));
}


void SceneObject::setRotation(glm::vec3 rotation)
{
    _rotation = rotation;
    _rotationQuaternion = glm::quat(rotation);

    _rotationMode = RM_EULER_ANGLES;

    changedTransform();
}


void SceneObject::setRotation(float x, float y, float z)
{
    setRotation(glm::vec3(x, y, z));
}


void SceneObject::setRotationQuaternion(glm::quat rotation)
{
    _rotationQuaternion = rotation;
    _rotation = glm::eulerAngles(rotation);

    _rotationMode = RM_QUATERNION;

    changedTransform();
}


void SceneObject::setRotationQuaternion(float x, float y, float z, float w)
{
    _rotationQuaternion.x = x;
    _rotationQuaternion.y = y;
    _rotationQuaternion.z = z;
    _rotationQuaternion.w = w;

    _rotationMode = RM_QUATERNION;

    changedTransform();
}


void SceneObject::setScale(glm::vec3 scale)
{
    _scale = scale;

    changedTransform();
}


void SceneObject::setScale(float x, float y, float z)
{
    setScale(glm::vec3(x, y, z));
}


void SceneObject::setScale(float scale)
{
    setScale(glm::vec3(scale, scale, scale));
}


void SceneObject::move(glm::vec3 deltaPosition)
{
    _position += deltaPosition;

    changedTransform();
}


void SceneObject::move(float dx, float dy, float dz)
{
    move(glm::vec3(dx, dy, dz));
}


void SceneObject::rotate(glm::vec3 deltaRotation)
{
    _rotation += deltaRotation;

    changedTransform();
}


void SceneObject::rotate(float dx, float dy, float dz)
{
    rotate(glm::vec3(dx, dy, dz));
}


void SceneObject::scale(glm::vec3 scale)
{
    _scale *= scale;

    changedTransform();
}


void SceneObject::scale(float x, float y, float z)
{
    scale(glm::vec3(x, y, z));
}


glm::vec3 SceneObject::getPosition() const
{
    return _position;
}


glm::vec3 SceneObject::getRotation() const
{
    return _rotation;
}


glm::quat SceneObject::getRotationQuaternion() const
{
    return _rotationQuaternion;
}


glm::vec3 SceneObject::getScale() const
{
    return _scale;
}


glm::mat4& SceneObject::getLocalTransformMatrix() const
{
    if (!_localTransformMatrixIsCalculated)
    {
        calculateLocalTransformMatrix();

        _localTransformMatrixIsCalculated = true;
    }

    return _localTransformMatrix;
}


glm::mat4& SceneObject::getLocalNormalMatrix() const
{
    if (!_localNormalMatrixIsCalculated)
    {
        _localNormalMatrix = glm::transpose(glm::inverse(getLocalTransformMatrix()));

        _localNormalMatrixIsCalculated = true;
    }

    return _localNormalMatrix;
}


glm::mat4& SceneObject::getGlobalTransformMatrix() const
{
    if (!_globalTransformMatrixIsCalculated)
    {
        calculateGlobalTransformMatrix();

        _globalTransformMatrixIsCalculated = true;
    }

    return _globalTransformMatrix;
}


glm::mat4& SceneObject::getGlobalNormalMatrix() const
{
    if (!_globalNormalMatrixIsCalculated)
    {
        _globalNormalMatrix = glm::transpose(glm::inverse(getGlobalTransformMatrix()));

        _globalNormalMatrixIsCalculated = true;
    }

    return _globalNormalMatrix;
}
