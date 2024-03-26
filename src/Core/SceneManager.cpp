#include "Core/SceneManager.h"
#include "Core/App.h"
#include "Core/DebugUI.h"
#include "Gameplay/MainMenu.h"
#include "Gameplay/Scene.h"
#include "Gameplay/TestScene.h"
#include "Utils/Defs.h"
#include "Utils/List.h"
#include "Utils/Log.h"
#include <imgui.h>

SceneManager::SceneManager() : Module()
{
    name.Create("scenemanager");
}

SceneManager::SceneManager(bool startEnabled) : Module(startEnabled)
{
    name.Create("scenemanager");
}

// Destructor
SceneManager::~SceneManager()
{
}

// Called before render is available
bool SceneManager::Awake(pugi::xml_node& config)
{
    LOG("Loading Scene Manager");

    bool ret = true;

    // iterate all children of scenemanager node and load all scenes taking the name of the scene
    for (pugi::xml_node sceneNode : config.children())
    {
        // Get the name of the scene
        std::string sceneName = sceneNode.name();

        // Create a new scene with the name
        Scene* scene = CreateScene(sceneName.c_str());
        if(scene == nullptr)
        {
            LOG("Scene %s not found\n", sceneName.c_str());
            ret = false;
            break;
        }

        scene->parameters = sceneNode;

        if(sceneName == config.attribute("startScene").as_string()) //no se si es lo mejor hacerlo asi
        {
            currentScene = scene;
        }

        // Add the scene to the list of scenes
        scenes.PushBack(scene);
    }
    
    if(currentScene == nullptr)
    {
        LOG("No start scene found");
        ret = false;
    }

    return ret;
}

// Called before the first frame
bool SceneManager::Start()
{
    bool ret = true;

    ret = currentScene->Enter();

    return ret;
}

// Called each loop iteration
bool SceneManager::PreUpdate()
{
    bool ret = true;

    ret = currentScene->PreUpdate();

    return ret;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
    bool ret = true;

    ret = currentScene->Update(dt);

    return ret;
}

bool SceneManager::PostUpdate()
{
    bool ret = true;

    ret = currentScene->PostUpdate();

    return ret;
}

// Called before quitting
bool SceneManager::CleanUp()
{
    bool ret = true;

    for(int i = 0; i < scenes.Count(); i++)
    {
        scenes[i]->CleanUp();
        RELEASE(scenes[i]);
    }

    scenes.Clear();

    return ret;
}

void SceneManager::DrawImGui()
{
    if(app->debugUI->sceneManagerLoadScene){
        ImGui::Begin("Load Scene", &app->debugUI->sceneManagerLoadScene, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Scene List:");

        // Create an array of scene names
        std::vector<const char*> sceneNames;
        for (int i = 0; i < scenes.Count(); i++)
        {
            sceneNames.push_back(scenes[i]->name.GetString());
        }

        // Create a static variable to store the current selection
        static int currentSelection = 0;

        // Create the list box
        ImGui::ListBox("##Scenes", &currentSelection, &sceneNames[0], sceneNames.size());

        // If the selected scene is not the current scene, change the scene
        if (ImGui::Button("Load Scene") && scenes[currentSelection]->name != currentScene->name)
        {
            ChangeScene(scenes[currentSelection]->name);
        }

        ImGui::End();
    /* 
        ImGui::Begin("Scene Manager");
        ImGui::Text("Scene List:");

        // Create an array of scene names
        std::vector<const char*> sceneNames;
        for (int i = 0; i < scenes.Count(); i++)
        {
            sceneNames.push_back(scenes[i]->name.GetString());
        }

        // Create a static variable to store the current selection
        static int currentSelection = 0;

        // Create the list box
        if (ImGui::ListBox("##Scenes", &currentSelection, &sceneNames[0], sceneNames.size()))
        {
            // If the selected scene is not the current scene, change the scene
            if (scenes[currentSelection]->name != currentScene->name)
            {
                ChangeScene(scenes[currentSelection]->name);
            }
        }

        ImGui::End(); */
    }
}

Scene* SceneManager::CreateScene(SString sceneName)
{
    if(sceneName == "mainmenu")
    {
        return new MainMenu(sceneName);
    }
    else if(sceneName == "testscene")
    {
        return new TestScene(sceneName);
    }
    else
    {
        LOG("Scene %s not found\n", sceneName.GetString());
        return nullptr;
    }
}

Scene* SceneManager::FindScene(SString sceneName) const
{
    Scene* scene = nullptr;

    for(int i = 0; i < scenes.Count(); i++)
    {
        if(scenes[i]->name == sceneName)
        {
            scene = scenes[i];
            break;
        }
    }

    return scene;
}

void SceneManager::ChangeScene(SString sceneName)
{
    currentScene->Exit();
    Scene* newScene = FindScene(sceneName);
    if (newScene != nullptr){
        currentScene = newScene;
    }
    else{
        LOG("Scene %s not found\n", sceneName.GetString());
    }
    currentScene->Enter();
}

// Load / Save
bool SceneManager::LoadState(pugi::xml_node& node)
{
    bool ret = true;

    ret = currentScene->LoadState(node);

    return ret;
}

bool SceneManager::SaveState(pugi::xml_node& node) const
{
    bool ret = true;

    ret = currentScene->SaveState(node);

    return ret;
}
