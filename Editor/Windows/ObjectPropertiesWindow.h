#ifndef OBJECTPROPERTIESWINDOW_H_INCLUDED
#define OBJECTPROPERTIESWINDOW_H_INCLUDED


#include "EditorWindow.h"


class ObjectPropertiesWindow : public EditorWindow
{
    private:
        virtual void drawWindow() override;

    public:
        ObjectPropertiesWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject, bool isOpen = true);

};


#endif // OBJECTPROPERTIESWINDOW_H_INCLUDED