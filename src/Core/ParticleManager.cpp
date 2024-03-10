#include "Core/ParticleManager.h"
#include "Core/App.h"
#include "Core/Physics.h"
#include "Core/Render.h"

#include "Utils/List.h"
#include "box2d/b2_body.h"

Particle::Particle() 
{
    timer = new Timer();

    pbody = app->physics->CreateRectangleSensor(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.x), size, size, bodyType::DYNAMIC);
    
}

Particle::~Particle()
{
    // Release the particle's body
}

void Particle::Update()
{
    if(active)
    {
        // Update the particle's lifetime
        if(timer->ReadSec() <= lifetime)
        {
            pbody->body->ApplyForceToCenter({0, 9.8}, true);
            
            // Update the particle's position
            pbody->GetPosition(position.x, position.y);

            // Draw the particle
            app->render->DrawRectangle({position.x, position.y}, 255, 255, 255);
        }
        else
        {
            app->physics->DestroyBody(pbody);
            active = false;
        }
    }
}


ParticleGenerator::ParticleGenerator() {}

void ParticleGenerator::EmitParticles()
{
   if(emiting)
   {
       for (int i = 0; i < amount; i++)
       {
              // Create a new particle and add it to the particles array
              Particle* particle = new Particle();
              particle->spawnPosition = position;
              particles.Add(particle);
       }
   }
}

void ParticleGenerator::Update()
{
    EmitParticles();
    ListItem<Particle*>* item = particles.start;
    while(item != nullptr)
    {
        if(item->data->active)
        {
            item->data->Update();
        }
        else
        {
            delete item->data;
            item->data = nullptr;
            particles.Del(item);
        }
        item = item->next;
    }
}

ParticleManager::ParticleManager() : Module()
{
    name.Create("particleManager");
}

ParticleManager::ParticleManager(bool startEnabled) : Module(startEnabled)
{
    name.Create("particleManager");
}

ParticleManager::~ParticleManager()
{
}

bool ParticleManager::Awake(pugi::xml_node& conf)
{
    return true;
}

bool ParticleManager::Start()
{
    ParticleGenerator* generator = new ParticleGenerator();
    generator->emiting = true;
    generator->amount = 10;
    generator->position = {600, 1500};
    generators.Add(generator);

    return true;
}

bool ParticleManager::PreUpdate()
{
    ListItem<ParticleGenerator*>* item = generators.start;
    while(item != nullptr)
    {
        item->data->Update();
        item = item->next;
    }
    return true;
}

bool ParticleManager::Update(float dt)
{
    return true;
}

bool ParticleManager::CleanUp()
{
    return true;
}

void ParticleManager::DrawImGui()
{
}