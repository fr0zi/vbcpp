#ifndef GRAPHICSMANAGER_H_INCLUDED
#define GRAPHICSMANAGER_H_INCLUDED


#include <list>
#include <vector>

#include <glm/glm.hpp>

#include "RenderObject.h"
#include "RenderData.h"
#include "Camera.h"
//#include "OGLDriver.h"
#include "Light.h"
#include "../Utils/RModel.h"

#include "CameraStatic.hpp"
#include "CameraFPS.hpp"

#include <memory>

class GraphicsManager
{
    private:
        std::list<RenderObject*>    _renderObjects;
        std::vector<CameraStatic*>  _cameras;
        std::list<Light*>           _lights;

        glm::vec3   _windDirection;
        float       _windVelocity;
        float       _windValue;
        glm::vec3   _windVector;

        GraphicsManager();

    public:
        ~GraphicsManager();

        static GraphicsManager& getInstance();


        //RenderObject*   qddRenderObject(RModel* model = NULL/*, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        //                                glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)*/);

        RenderObject*   addRenderObject(RenderObject* object); //Model* model = NULL);

        CameraStatic*   addCameraStatic(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue);
        CameraFPS*      addCameraFPS(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue);
        Light*          addDirectionalLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity);
        Light*          addPointLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity, LightAttenuation attenuation = LightAttenuation());
        Light*          addSpotLight(glm::vec3 color, float ambientIntensity, float diffuseIntensity, float cutoff, LightAttenuation attenuation = LightAttenuation());


        // Funkcje wywolywana przez SceneObject, nie wywolywac recznie
        void removeRenderObject(RenderObject* object);
        void removeCamera(CameraStatic* camera);
        void removeLight(Light* light);


        void setWindDirection(glm::vec3 direction);
        void setWindVelocity(float velocity);

        glm::vec3   getWindDirection();
        float       getWindVelocity();
        glm::vec3   getWindVector();
        float       getWindValue();


        void update(float deltaTime);


        RenderData* getRenderData();
};


#endif // GRAPHICSMANAGER_H_INCLUDED
