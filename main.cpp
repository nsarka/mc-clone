
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>

#include <SDL.h>
#include <SDL_image.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>

#include "camera.h"
#include "shader.h"
#include "chunk.h"
#include "texture.h"
#include <cmath>

bool done = false;

const int screen_w = 1024;
const int screen_h = 768;

const float moveSpeed = 8.0f; // units/sec

unsigned int VBO, VAO;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

std::array<float, 42> NZ = {
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 4.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 4.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 4.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 4.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 4.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 4.0f
};

std::array<float, 42> PZ = {
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 4.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 4.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 4.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 4.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 4.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 4.0f
};

std::array<float, 42> NX = {
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 4.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 4.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 4.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 4.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 4.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 4.0f
};

std::array<float, 42> PX = {
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 4.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 4.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 4.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 4.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 4.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 4.0f
};

std::array<float, 42> NY = {
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 4.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 4.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 4.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 4.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 4.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 4.0f
};

std::array<float, 42> PY = {
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 4.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 4.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 4.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 4.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 4.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 4.0f
};


Player player;
Camera camera;
std::vector<Chunk*> chunkList;
const int WORLD_SIZE_IN_CHUNKS = 25;
bool chunk_used[WORLD_SIZE_IN_CHUNKS][WORLD_SIZE_IN_CHUNKS][WORLD_SIZE_IN_CHUNKS] = {false};


void generateTerrain()
{
    glm::vec3& playerPos = player.getPosition();

    // Get the chunk position the player is in
    int chunk_x_start = (((int)playerPos.x) / Chunk::CHUNK_SIZE);
    if (playerPos.x < 0) chunk_x_start--;
    int chunk_y_start = (((int)playerPos.y) / Chunk::CHUNK_SIZE);
    if (playerPos.y < 0) chunk_y_start--;
    int chunk_z_start = (((int)playerPos.z) / Chunk::CHUNK_SIZE);
    if (playerPos.z < 0) chunk_z_start--;

    // generate the chunks near where the player is standing if they dont exist already
    for (int chunk_x = chunk_x_start - 4; chunk_x <= chunk_x_start + 4; chunk_x++) {
        for (int chunk_y = chunk_y_start - 4; chunk_y <= chunk_y_start + 4; chunk_y++) {
            for (int chunk_z = chunk_z_start - 4; chunk_z <= chunk_z_start + 4; chunk_z++) {

                if (chunk_x < 0) continue;
                if (chunk_y < 0) continue;
                if (chunk_z < 0) continue;

                if (!chunk_used[chunk_x][chunk_y][chunk_z]) {
                    Chunk* pChunk = new Chunk();
                    
                    for (int block_x = 0; block_x < Chunk::CHUNK_SIZE; block_x++) {
                        for (int block_z = 0; block_z < Chunk::CHUNK_SIZE; block_z++) {

                            double x = (block_x + (Chunk::CHUNK_SIZE * chunk_x)) / 128.0;
                            double z = (block_z + (Chunk::CHUNK_SIZE * chunk_z)) / 128.0;

                            double height = glm::simplex(glm::vec2(x, z));
                            height = (height + 1.0) / 2.0;
                            height *= 16.0;
                            //double height = 64.0 * glm::sin(x / 64.0) + 64.0;
                            //double height = 5.0;
                            int block_y = (int)floor(height);

                            if (block_y >= (chunk_y * Chunk::CHUNK_SIZE) && (block_y < ((chunk_y + 1) * Chunk::CHUNK_SIZE))) {
                                BlockType type = BlockType::BlockType_Grass;
                                
                                if (height <= 2.0)
                                    type = BlockType::BlockType_Stone;
                                else if (height <= 4.0)
                                    type = BlockType::BlockType_Sand;
                                else if (height <= 6.0)
                                    type = BlockType::BlockType_Dirt;

                                pChunk->m_pBlocks[block_x][block_y % Chunk::CHUNK_SIZE][block_z].m_blockType = type; //(BlockType)(rand() % ((int)BlockType::BlockType_NumTypes));
                            }

                            for (int block_y = 0; block_y < Chunk::CHUNK_SIZE; block_y++) {
                                if (pChunk->getBlockType(block_x, block_y - 1, block_z) == BlockType::BlockType_Stone) {
                                    pChunk->m_pBlocks[block_x][block_y][block_z].m_blockType = BlockType::BlockType_Water;
                                }
                            }
                        }
                    }
                    

                    // All blocks in the chunk set to grass
                    /*
                    for (int block_x = 0; block_x < Chunk::CHUNK_SIZE; block_x++) {
                        for (int block_y = 0; block_y < Chunk::CHUNK_SIZE; block_y++) {
                            for (int block_z = 0; block_z < Chunk::CHUNK_SIZE; block_z++) {
                                pChunk->m_pBlocks[block_x][block_y][block_z].m_blockType = BlockType::BlockType_Dirt; //(BlockType)(rand() % ((int)BlockType::BlockType_NumTypes));
                            }
                        }
                    }
                    */
                    
                    chunkList.push_back(pChunk);
                    glm::vec3 pos = { chunk_x, chunk_y, chunk_z };
                    pChunk->m_position = pos;
                    pChunk->update();

                    chunk_used[chunk_x % WORLD_SIZE_IN_CHUNKS][chunk_y % WORLD_SIZE_IN_CHUNKS][chunk_z % WORLD_SIZE_IN_CHUNKS] = true;

                    //std::cout << "Generating chunk at (" << chunk_x << ", 0, " << chunk_z << ")" << std::endl;
                }
            }
        }
    }
}

// Main code
int main(int, char**)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // GL 3.3 + GLSL 130
    const char* glsl_version = "#version 330 core";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_w, screen_h, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        throw(std::string("Failed to initialize GLAD"));
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;   // handle mouse cursor via sdl2 funcs myself

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    ImVec4 clear_color = ImVec4(0.05f, 0.55f, 0.95f, 1.00f);

    Texture::setup();
    Shader ourShader("box.vs", "box.fs");

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Main loop
    bool forward = false;
    bool backward = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;

    //std::thread terrainThread(generateTerrain);


    while (!done)
    {
        float currentFrame = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float cameraSpeed = static_cast<float>(moveSpeed * deltaTime);

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type) {
                /* Look for a keypress */
            case SDL_KEYDOWN:
                /* Check the SDLKey values and move change the coords */
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                case SDLK_w:
                    forward = true;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    backward = true;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    left = true;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    right = true;
                    break;
                case SDLK_SPACE:
                    up = true;
                    break;
                case SDLK_c:
                case SDLK_LCTRL:
                    down = true;
                    break;
                case SDLK_ESCAPE:
                    done = true;
                    break;
                default:
                    break;
                }
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                case SDLK_w:
                    forward = false;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    backward = false;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    left = false;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    right = false;
                    break;
                case SDLK_SPACE:
                    up = false;
                    break;
                case SDLK_c:
                case SDLK_LCTRL:
                    down = false;
                    break;
                default:
                    break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                camera.handleMouseButtonDown(event.button);
                break;

            case SDL_MOUSEMOTION:
            {
                camera.handleMouseMotion(event.motion, player.getEulerAngles());
                break;
            }

            default:
                break;
            }

            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        glm::vec3& playerVel = player.getVelocity();
        glm::vec3& playerPos = player.getPosition();
        glm::vec2& eulerAngles = player.getEulerAngles();

        playerVel = glm::vec3(0.0f, 0.0f, 0.0f);

        if (forward) {
            playerVel += camera.getFront();
        }
        if (backward) {
            playerVel += -camera.getFront();
        }
        if (left) {
            playerVel += -camera.getRight();
        }
        if (right) {
            playerVel += camera.getRight();
        }
        if (up) {
            playerVel += glm::vec3(0.0f, 1.0f, 0.0f);
        }
        if (down) {
            playerVel += -glm::vec3(0.0f, 1.0f, 0.0f);
        }

        playerPos += playerVel * cameraSpeed;

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("Debug");

            ImGui::Text("Player Position: %f, %f, %f", playerPos.x, playerPos.y, playerPos.z);
            ImGui::Text("Player Velocity: %f, %f, %f", playerVel.x, playerVel.y, playerVel.z);
            ImGui::Text("Player Euler Angles: %f, %f", eulerAngles.x, eulerAngles.y);
            ImGui::Text("Camera Front: %f, %f, %f", camera.getFront().x, camera.getFront().y, camera.getFront().z);
            ImGui::Text("Current Frame: %f", currentFrame);
            ImGui::Text("Delta Time: %f", deltaTime);
            ImGui::SliderFloat("FOV", &camera.getFov(), 1.0f, 160.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            //ImGui::SameLine();

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        ImGui::Render(); // doesnt actually draw anything

        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        generateTerrain();

        // Start of game rendering
        for (auto chunk : chunkList) {
            chunk->render(ourShader);
        }
        
        // End of game rendering

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    //terrainThread.join();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
