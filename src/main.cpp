#ifdef _WIN32
#include <windows.h>
#include <SDL_main.h>
#endif

#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include "Canis/Canis.hpp"
#include "Canis/Entity.hpp"
#include "Canis/Graphics.hpp"
#include "Canis/Window.hpp"
#include "Canis/Shader.hpp"
#include "Canis/Debug.hpp"
#include "Canis/IOManager.hpp"
#include "Canis/InputManager.hpp"
#include "Canis/Camera.hpp"
#include "Canis/Model.hpp"
#include "Canis/World.hpp"
#include "Canis/Editor.hpp"
#include "Canis/FrameRateManager.hpp"

using namespace glm;

// number of fire textures (for frames)
int fire_frames = 31;
// tracks how much time has passed since the last texture change
float fireElapsedTime = 0.0f;
// Index of the current fire texture
int fireTextureIndex = 1;

// git restore .
// git fetch
// git pull

// 3d array
std::vector<std::vector<std::vector<unsigned int>>> map = {};

// declaring functions
void SpawnLights(Canis::World &_world);
void LoadMap(std::string _path);
void Rotate(Canis::World &_world, Canis::Entity &_entity, float _deltaTime);

#ifdef _WIN32
#define main SDL_main
extern "C" int main(int argc, char* argv[])
#else
int main(int argc, char* argv[])
#endif
{
    Canis::Init();

    // Seed the random number generator once
    srand((unsigned int)time(NULL));

    Canis::InputManager inputManager;
    Canis::FrameRateManager frameRateManager;
    frameRateManager.Init(60);

    /// SETUP WINDOW
    Canis::Window window;
    window.MouseLock(true);

    unsigned int flags = 0;

    if (Canis::GetConfig().fullscreen)
        flags |= Canis::WindowFlags::FULLSCREEN;

    window.Create("Hello Graphics", Canis::GetConfig().width, Canis::GetConfig().heigth, flags);
    /// END OF WINDOW SETUP

    Canis::World world(&window, &inputManager, "assets/textures/lowpoly-skybox/");
    SpawnLights(world);

    Canis::Editor editor(&window, &world, &inputManager);

    Canis::Graphics::EnableAlphaChannel();
    Canis::Graphics::EnableDepthTest();

    /// SETUP SHADER
    // added shader for each texture
    Canis::Shader shader;
    shader.Compile("assets/shaders/hello_shader.vs", "assets/shaders/hello_shader.fs");
    shader.AddAttribute("aPosition");
    shader.Link();
    shader.Use();
    shader.SetInt("MATERIAL.diffuse", 0);
    shader.SetInt("MATERIAL.specular", 1);
    shader.SetFloat("MATERIAL.shininess", 64);
    shader.SetBool("WIND", false);
    shader.UnUse();

    Canis::Shader brickShader;
    brickShader.Compile("assets/shaders/hello_shader.vs", "assets/shaders/hello_shader.fs");
    brickShader.AddAttribute("aPosition");
    brickShader.Link();
    brickShader.Use();
    brickShader.SetInt("MATERIAL.diffuse", 0);
    brickShader.SetInt("MATERIAL.specular", 1);
    brickShader.SetFloat("MATERIAL.shininess", 32);
    brickShader.SetBool("WIND", false);
    brickShader.UnUse();

    Canis::Shader dirtShader;
    dirtShader.Compile("assets/shaders/hello_shader.vs", "assets/shaders/hello_shader.fs");
    dirtShader.AddAttribute("aPosition");
    dirtShader.Link();
    dirtShader.Use();
    dirtShader.SetInt("MATERIAL.diffuse", 0);
    dirtShader.SetInt("MATERIAL.specular", 1);
    dirtShader.SetFloat("MATERIAL.shininess", 32);
    dirtShader.SetBool("WIND", false);
    dirtShader.UnUse();

    Canis::Shader cobblestoneShader;
    cobblestoneShader.Compile("assets/shaders/hello_shader.vs", "assets/shaders/hello_shader.fs");
    cobblestoneShader.AddAttribute("aPosition");
    cobblestoneShader.Link();
    cobblestoneShader.Use();
    cobblestoneShader.SetInt("MATERIAL.diffuse", 0);
    cobblestoneShader.SetInt("MATERIAL.specular", 1);
    cobblestoneShader.SetFloat("MATERIAL.shininess", 64);
    cobblestoneShader.SetBool("WIND", false);
    cobblestoneShader.UnUse();

    Canis::Shader grassblockShader;
    grassblockShader.Compile("assets/shaders/hello_shader.vs", "assets/shaders/hello_shader.fs");
    grassblockShader.AddAttribute("aPosition");
    grassblockShader.Link();
    grassblockShader.Use();
    grassblockShader.SetInt("MATERIAL.diffuse", 0);
    grassblockShader.SetInt("MATERIAL.specular", 1);
    grassblockShader.SetFloat("MATERIAL.shininess", 32);
    grassblockShader.SetBool("WIND", false);
    grassblockShader.UnUse();

    Canis::Shader oakplankShader;
    oakplankShader.Compile("assets/shaders/hello_shader.vs", "assets/shaders/hello_shader.fs");
    oakplankShader.AddAttribute("aPosition");
    oakplankShader.Link();
    oakplankShader.Use();
    oakplankShader.SetInt("MATERIAL.diffuse", 0);
    oakplankShader.SetInt("MATERIAL.specular", 1);
    oakplankShader.SetFloat("MATERIAL.shininess", 64);
    oakplankShader.SetBool("WIND", false);
    oakplankShader.UnUse();

    Canis::Shader oaklogShader;
    oaklogShader.Compile("assets/shaders/hello_shader.vs", "assets/shaders/hello_shader.fs");
    oaklogShader.AddAttribute("aPosition");
    oaklogShader.Link();
    oaklogShader.Use();
    oaklogShader.SetInt("MATERIAL.diffuse", 0);
    oaklogShader.SetInt("MATERIAL.specular", 1);
    oaklogShader.SetFloat("MATERIAL.shininess", 32);
    oaklogShader.SetBool("WIND", false);
    oaklogShader.UnUse();

    Canis::Shader netherrackShader;
    netherrackShader.Compile("assets/shaders/hello_shader.vs", "assets/shaders/hello_shader.fs");
    netherrackShader.AddAttribute("aPosition");
    netherrackShader.Link();
    netherrackShader.Use();
    netherrackShader.SetInt("MATERIAL.diffuse", 0);
    netherrackShader.SetInt("MATERIAL.specular", 1);
    netherrackShader.SetFloat("MATERIAL.shininess", 64);
    netherrackShader.SetBool("WIND", false);
    netherrackShader.UnUse();

    Canis::Shader grassShader;
    grassShader.Compile("assets/shaders/hello_shader.vs", "assets/shaders/hello_shader.fs");
    grassShader.AddAttribute("aPosition");
    grassShader.Link();
    grassShader.Use();
    grassShader.SetInt("MATERIAL.diffuse", 0);
    grassShader.SetInt("MATERIAL.specular", 1);
    grassShader.SetFloat("MATERIAL.shininess", 64);
    grassShader.SetBool("WIND", true);
    grassShader.SetFloat("WINDEFFECT", 0.2);
    grassShader.UnUse();

    Canis::Shader flowerShader;
    flowerShader.Compile("assets/shaders/hello_shader.vs", "assets/shaders/hello_shader.fs");
    flowerShader.AddAttribute("aPosition");
    flowerShader.Link();
    flowerShader.Use();
    flowerShader.SetInt("MATERIAL.diffuse", 0);
    flowerShader.SetInt("MATERIAL.specular", 1);
    flowerShader.SetFloat("MATERIAL.shininess", 64);
    flowerShader.SetBool("WIND", true);
    flowerShader.SetFloat("WINDEFFECT", 0.2);
    flowerShader.UnUse();


    /// END OF SHADER

    /// Load Image

    // all plant textures used
    Canis::GLTexture grassTexture = Canis::LoadImageGL("assets/textures/grass.png", false);
    Canis::GLTexture blue_orchidTexture = Canis::LoadImageGL("assets/textures/blue_orchid.png", false);
    Canis::GLTexture poppyTexture = Canis::LoadImageGL("assets/textures/poppy.png", false);
    Canis::GLTexture azure_bluetTexture = Canis::LoadImageGL("assets/textures/azure_bluet.png", false);
    Canis::GLTexture dandelionTexture = Canis::LoadImageGL("assets/textures/dandelion.png", false);
    Canis::GLTexture cornflowerTexture = Canis::LoadImageGL("assets/textures/cornflower.png", false);

    // all block textures used
    Canis::GLTexture glassTexture = Canis::LoadImageGL("assets/textures/glass.png", true);
    Canis::GLTexture brickTexture = Canis::LoadImageGL("assets/textures/bricks.png", true);
    Canis::GLTexture dirtTexture = Canis::LoadImageGL("assets/textures/dirt.png", true);
    Canis::GLTexture cobblestoneTexture = Canis::LoadImageGL("assets/textures/cobblestone.png", true);
    Canis::GLTexture grass_block_topTexture = Canis::LoadImageGL("assets/textures/grass_block_top.png", true);
    Canis::GLTexture oak_plank_floorTexture = Canis::LoadImageGL("assets/textures/oak_planks_floor.png", true);
    Canis::GLTexture oak_logTexture = Canis::LoadImageGL("assets/textures/oak_log.png", true);
    Canis::GLTexture oak_planksTexture = Canis::LoadImageGL("assets/textures/oak_planks.png", true);
    Canis::GLTexture netherrackTexture = Canis::LoadImageGL("assets/textures/netherrack.png", true);
    Canis::GLTexture textureSpecular = Canis::LoadImageGL("assets/textures/container2_specular.png", true);
    /// End of Image Loading

    /// Load Models
    Canis::Model cubeModel = Canis::LoadModel("assets/models/cube.obj");
    Canis::Model grassModel = Canis::LoadModel("assets/models/plants.obj");
    //load fire model
    Canis::Model fireModel = Canis::LoadModel("assets/models/fire.obj");
    /// END OF LOADING MODEL
    
    // loads all 31 fire textures 
    std::vector<Canis::GLTexture> fireTextures;
    for (int i = 1; i <= fire_frames; i++) {
        fireTextures.push_back(Canis::LoadImageGL("assets/textures/fire_textures/fire_" + std::to_string(i) + ".png", true));
    }

    // Load Map into 3d array
    LoadMap("assets/maps/level.map");

    // Loop map and spawn objects
    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[y].size(); x++)
        {
            for (int z = 0; z < map[y][x].size(); z++)
            {
                Canis::Entity entity;
                entity.active = true;

                switch (map[y][x][z])
                {
                case 1: // places a glass block
                    entity.tag = "glass";
                    entity.albedo = &glassTexture;
                    entity.specular = &textureSpecular;
                    entity.model = &cubeModel;
                    entity.shader = &shader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                case 2: // places grass 
                {
                    int r = rand() % 100; // 0 - 99
                    if (r < 50){ // 50% chance to place nothing
                        break;
                    }

                    int plantChance = rand() % 100;
                    if (plantChance < 75){ // 75% of the 50% chance to place a plant will place grass
                        entity.tag = "grass";
                        entity.albedo = &grassTexture;
                    }
                    else{ // 25% of the 50% chance to place a plant will place 1 of the 5 plant textures
                    // plants are blue orchid, poppy, azure bullet, cornflower, and dandelion
                        int flowerIndex = rand() % 5;
                        switch (flowerIndex){
                            case 0:
                                entity.tag = "blue orchid";
                                entity.albedo = &blue_orchidTexture;
                                break;
                            case 1:
                                entity.tag = "poppy";
                                entity.albedo = &poppyTexture;
                                break;
                            case 2:
                                entity.tag = "azure bullet";
                                entity.albedo = &azure_bluetTexture;
                                break;
                            case 3:
                                entity.tag = "cornflower";
                                entity.albedo = &cornflowerTexture;
                                break;
                            case 4:
                                entity.tag = "dandelion";
                                entity.albedo = &dandelionTexture;
                                break;
                        }
                    }
                    entity.specular = &textureSpecular;
                    entity.model = &grassModel;
                    entity.shader = &flowerShader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    entity.Update = &Rotate;
                    world.Spawn(entity);
                    break;
                }  
                case 3: // places a brick block
                    entity.tag = "brick";
                    entity.albedo = &brickTexture;
                    entity.specular = &textureSpecular;
                    entity.model = &cubeModel;
                    entity.shader = &brickShader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                case 4: // places a dirt block
                    entity.tag = "dirt";
                    entity.albedo = &dirtTexture;
                    entity.specular = &textureSpecular;
                    entity.model = &cubeModel;
                    entity.shader = &dirtShader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                case 5: // places a cobblestone block
                    entity.tag = "cobblestone";
                    entity.albedo = &cobblestoneTexture;
                    entity.specular = &textureSpecular;
                    entity.model = &cubeModel;
                    entity.shader = &cobblestoneShader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                case 6: // places a grass block
                    entity.tag = "grass block";
                    entity.albedo = &grass_block_topTexture;
                    entity.specular = &textureSpecular;
                    entity.model = &cubeModel;
                    entity.shader = &grassblockShader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                case 7: // places an oak plank block (for the floor only) 
                    entity.tag = "oak plank floor";
                    entity.albedo = &oak_plank_floorTexture;
                    entity.specular = &textureSpecular;
                    entity.model = &cubeModel;
                    entity.shader = &oakplankShader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                case 8: // places a oak log block
                    entity.tag = "oak log";
                    entity.albedo = &oak_logTexture;
                    entity.specular = &textureSpecular;
                    entity.model = &cubeModel;
                    entity.shader = &oaklogShader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                case 9: // places an oak plank block
                    entity.tag = "oak plank ";
                    entity.albedo = &oak_planksTexture;
                    entity.specular = &textureSpecular;
                    entity.model = &cubeModel;
                    entity.shader = &oakplankShader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                case 10: // places a netherrack block
                    entity.tag = "netherrack";
                    entity.albedo = &netherrackTexture;
                    entity.specular = &textureSpecular;
                    entity.model = &cubeModel;
                    entity.shader = &netherrackShader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                case 11: // places fire
                    entity.tag = "fire";
                    entity.albedo = &fireTextures[0]; // start with the first frame
                    entity.specular = &textureSpecular;
                    entity.model = &fireModel;
                    entity.shader = &shader;
                    entity.transform.position = vec3(x + 0.0f, y + 0.0f, z + 0.0f);
                    world.Spawn(entity);
                    break;
                default:
                    break;
                }
            }
        }
    }

    double deltaTime = 0.0;
    double fps = 0.0;

    // Application loop
    while (inputManager.Update(Canis::GetConfig().width, Canis::GetConfig().heigth))
    {
        deltaTime = frameRateManager.StartFrame();
        Canis::Graphics::ClearBuffer(COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT);

        world.Update(deltaTime);
        
        // Update the elapsed time
        fireElapsedTime += deltaTime;

        // if 0.5 seconds have passed then switch to the next fire texture
        if (fireElapsedTime >= 0.5f)
        {
            // resets timer
            fireElapsedTime = 0.0f;
            // moves texture index back to 1 after 31
            fireTextureIndex++;
            if (fireTextureIndex > 31)
                fireTextureIndex = 1;
            // goes to nexxt fire texture
            world.UpdateFireTexture(fireTextureIndex);
        }
        world.Draw(deltaTime);

        editor.Draw();

        window.SwapBuffer();

        // EndFrame will pause the app when running faster than frame limit
        fps = frameRateManager.EndFrame();

        Canis::Log("FPS: " + std::to_string(fps) + " DeltaTime: " + std::to_string(deltaTime));
    }

    return 0;
}

void Rotate(Canis::World &_world, Canis::Entity &_entity, float _deltaTime)
{
    //_entity.transform.rotation.y += _deltaTime;
}

void LoadMap(std::string _path)
{
    std::ifstream file;
    file.open(_path);

    if (!file.is_open())
    {
        printf("file not found at: %s \n", _path.c_str());
        exit(1);
    }

    int number = 0;
    int layer = 0;

    map.push_back(std::vector<std::vector<unsigned int>>());
    map[layer].push_back(std::vector<unsigned int>());

    while (file >> number)
    {
        if (number == -2) // add new layer
        {
            layer++;
            map.push_back(std::vector<std::vector<unsigned int>>());
            map[map.size() - 1].push_back(std::vector<unsigned int>());
            continue;
        }

        if (number == -1) // add new row
        {
            map[map.size() - 1].push_back(std::vector<unsigned int>());
            continue;
        }

        map[map.size() - 1][map[map.size() - 1].size() - 1].push_back((unsigned int)number);
    }
}

void SpawnLights(Canis::World &_world)
{
    Canis::DirectionalLight directionalLight;
    directionalLight.ambient = vec3(0.2f, 0.2f, 0.2f);
    _world.SpawnDirectionalLight(directionalLight);

    Canis::PointLight pointLight;
    pointLight.position = vec3(0.0f);
    pointLight.ambient = vec3(0.2f);
    pointLight.diffuse = vec3(0.5f);
    pointLight.specular = vec3(1.0f);
    pointLight.constant = 1.0f;
    pointLight.linear = 0.09f;
    pointLight.quadratic = 0.032f;

    _world.SpawnPointLight(pointLight);

    // left side of house
    pointLight.position = vec3(5.0f, 6.0f, 4.0f);
    _world.SpawnPointLight(pointLight);

    // right side of house
    pointLight.position = vec3(5.0f, 6.0f, 14.0f);
    _world.SpawnPointLight(pointLight);
   
    // light for fireplace
    pointLight.position = vec3(15.0f, 4.5f, 9.0f); // slightly higer than the position of the fire
    pointLight.ambient = vec3(0.3f, 0.1f, 0.0f);
    pointLight.diffuse = vec3(0.8f, 0.4f, 0.1f);
    pointLight.specular = vec3(1.0f, 0.5f, 0.2f);
    _world.SpawnPointLight(pointLight);
}