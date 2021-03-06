#include "GraphicsManager.h"

#include "../Scene/SceneObject.h"


static std::unique_ptr<GraphicsManager> gmInstance;

GraphicsManager::GraphicsManager()
    : _windDirection(0.0f, 0.0f, 0.0f), _windVelocity(0.0f), _windValue(0.0f), _windVector(0.0f, 0.0f, 0.0f)
{

}


GraphicsManager::~GraphicsManager()
{
    for (std::list<RenderObject*>::iterator i = _renderObjects.begin(); i != _renderObjects.end(); ++i)
    {
        delete *i;
    }

    for (std::vector<CameraStatic*>::iterator i = _cameras.begin(); i != _cameras.end(); ++i)
    {
        delete *i;
    }

    for (std::list<Light*>::iterator i = _lights.begin(); i != _lights.end(); ++i)
    {
        delete *i;
    }
}

GraphicsManager& GraphicsManager::getInstance()
{
    if ( !gmInstance )
        gmInstance = std::unique_ptr<GraphicsManager>(new GraphicsManager);

    return *gmInstance;
}


RenderObject* GraphicsManager::addRenderObject(RenderObject* object )//RModel* model/*, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale*/)
{
    //RenderObject* object = new RenderObject(model);

    _renderObjects.push_back(object);

    return object;
}

CameraStatic* GraphicsManager::addCameraStatic(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue)
{
    CameraStatic* camera = new CameraStatic(width, height, viewAngle, nearValue, farValue);

    _cameras.push_back(camera);

    return camera;
}

CameraFPS* GraphicsManager::addCameraFPS(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue)
{
    CameraFPS* camera = new CameraFPS(width, height, viewAngle, nearValue, farValue);

    _cameras.push_back(camera);

    return camera;
}

Light* GraphicsManager::addDirectionalLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity)
{
    Light* light = new Light(LT_DIRECTIONAL, color, ambientIntensity, diffuseIntensity);

    _lights.push_back(light);

    return light;
}


Light* GraphicsManager::addPointLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity, LightAttenuation attenuation)
{
    Light* light = new Light(LT_POINT, color, ambientIntensity, diffuseIntensity);
    light->setAttenuation(attenuation);

    _lights.push_back(light);

    return light;
}


Light* GraphicsManager::addSpotLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity, float cutoff, LightAttenuation attenuation)
{
    Light* light = new Light(LT_SPOT, color, ambientIntensity, diffuseIntensity);
    light->setAttenuation(attenuation);
    light->setCutoff(cutoff);

    _lights.push_back(light);

    return light;
}


void GraphicsManager::removeRenderObject(RenderObject* object)
{
    for (std::list<RenderObject*>::iterator i = _renderObjects.begin(); i != _renderObjects.end(); ++i)
    {
        if (*i == object)
        {
            i = _renderObjects.erase(i);

            delete object;

            return;
        }
    }
}

void GraphicsManager::removeCamera(CameraStatic* camera)
{
    for (std::vector<CameraStatic*>::iterator i = _cameras.begin(); i != _cameras.end(); ++i)
    {
        if (*i == camera)
        {
            i = _cameras.erase(i);

            delete camera;

            return;
        }
    }
}

void GraphicsManager::removeLight(Light* light)
{
    for (std::list<Light*>::iterator i = _lights.begin(); i != _lights.end(); ++i)
    {
        if (*i == light)
        {
            i = _lights.erase(i);

            delete light;

            return;
        }
    }
}


void GraphicsManager::setWindDirection(glm::vec3 direction)
{
    _windDirection = direction;
}


void GraphicsManager::setWindVelocity(float velocity)
{
    _windVelocity = velocity;
}


glm::vec3 GraphicsManager::getWindDirection()
{
    return _windDirection;
}


float GraphicsManager::getWindVelocity()
{
    return _windVelocity;
}


glm::vec3 GraphicsManager::getWindVector()
{
    return _windVector;
}


float GraphicsManager::getWindValue()
{
    return _windValue;
}


void GraphicsManager::update(float deltaTime)
{
    _windValue += _windVelocity * deltaTime;
    _windVector = _windDirection * sinf(_windValue);
}


RenderData* GraphicsManager::getRenderData()
{
    RenderData* renderData = new RenderData;
    renderData->camera = _cameras[0];
    //renderData->light = *(_lights.begin());

    for (std::list<Light*>::iterator i = _lights.begin(); i != _lights.end(); ++i)
    {
        if ((*i)->isActive())
            renderData->lights.push_back(*i);
    }

    for (std::list<RenderObject*>::iterator i = _renderObjects.begin(); i != _renderObjects.end(); ++i)
    {
        RenderObject* object = *i;

        if (!(*i)->isActive())
            continue;

        for (int j = 0; j < object->getModel()->getQuantumOfMeshes(); ++j)
        {
            RenderListElement renderElement(object->getModel(), object->getModel()->getMesh(j), TransformMatrices(object->getSceneObject()->getGlobalTransformMatrix(), object->getSceneObject()->getGlobalNormalMatrix()),
                                            glm::length(renderData->camera->getPosition() - object->getSceneObject()->getPosition()), object->getSceneObject());
            if (object->getModel()->getMesh(j)->material.transparency == 0.0f)
                renderData->renderList.insert(renderData->renderList.begin(), renderElement);
            else
                renderData->renderList.push_back(renderElement);
        }
    }

    return renderData;
}
