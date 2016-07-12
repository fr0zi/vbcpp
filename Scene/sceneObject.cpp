#include "SceneObject.h"
#include "SceneManager.h"


SceneObject::SceneObject(unsigned int id, SceneManager* sceneManager)
    : _id(id), _isActive(true),
    _sceneManager(sceneManager)
{
    #ifdef _DEBUG_MODE
        std::cout << "Create SceneObject\n";
    #endif // _DEBUG_MODE
}


SceneObject::~SceneObject()
{
    #ifdef _DEBUG_MODE
        std::cout << "Destroy SceneObject\n";
    #endif // _DEBUG_MODE

    for (std::vector<Component*>:: iterator i = _components.begin(); i != _components.end(); ++i)
    {
        /* USUWANIE Z POSZCZEGOLNYCH PODSYSTEMOW */
        switch ((*i)->getType())
        {
            case CT_RENDER_OBJECT:
                _sceneManager->getGraphicsManager()->removeRenderObject(static_cast<RenderObject*>(*i));
                break;

            case CT_PHYSICAL_BODY:
                _sceneManager->getPhysicsManager()->removePhysicalBody(static_cast<PhysicalBody*>(*i));
                break;

        }

        /* ------------------------------------- */
    }
}


void SceneObject::addComponent(Component* component)
{
    if (component != NULL)
    {
        component->setSceneObject(this);

        _components.push_back(component);
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
                    _sceneManager->getGraphicsManager()->removeRenderObject(static_cast<RenderObject*>(*i));
                    break;

                case CT_PHYSICAL_BODY:
                    _sceneManager->getPhysicsManager()->removePhysicalBody(static_cast<PhysicalBody*>(*i));
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
}


unsigned int SceneObject::getId()
{
    return _id;
}


Transform* SceneObject::getTransform()
{
    return &_transform;
}


Component* SceneObject::getComponent(unsigned int index)
{
    if (index < _components.size())
        return _components[index];

    return NULL;
}


bool SceneObject::isActive()
{
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