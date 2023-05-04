
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <assert.h>


#include <SDL.h>
#include <SDL_image.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shader.h"
#include "chunk.h"

bool done = false;

const int screen_w = 1024;
const int screen_h = 768;

const float moveSpeed = 16.0f; // units/sec

unsigned int VBO, VAO;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

std::array<float, 30> NZ = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f
};

std::array<float, 30> PZ = {
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f
};

std::array<float, 30> NX = {
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f
};

std::array<float, 30> PX = {
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f
};

std::array<float, 30> NY = {
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f
};

std::array<float, 30> PY = {
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

std::array<float, 180> vertices = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

Player player;
Camera camera;
unsigned int texture_array;
std::vector<Chunk*> chunkList;
const int WORLD_SIZE_IN_CHUNKS = 500;
bool chunk_used[WORLD_SIZE_IN_CHUNKS][WORLD_SIZE_IN_CHUNKS] = {false};

void setupTextures()
{
    IMG_Init(IMG_INIT_PNG);
    const int textureSize = 16;
    const int numTextures = 4;
    
    glGenTextures(1, &texture_array);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, textureSize, textureSize,
        numTextures, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    SDL_Surface* surface = IMG_Load("texture_packs/default/voxels/grass.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/sand.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 1, textureSize, textureSize, 1, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels); // even though sand.png is a png, it's GL_RGB instead of GL_RGBA
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/dirt.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 2, textureSize, textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    SDL_FreeSurface(surface);

    surface = IMG_Load("texture_packs/default/voxels/stone.png");
    assert(surface->w == 16);
    assert(surface->h == 16);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 3, textureSize, textureSize, 1, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels); // even though stone.png is a png, it's GL_RGB instead of GL_RGBA
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    SDL_FreeSurface(surface);
}

void generateTerrain()
{
    //while (!done) {
        glm::vec3& playerPos = player.getPosition();

        // Get the chunk position the player is in
        int chunk_x_center = (((int)playerPos.x) / Chunk::CHUNK_SIZE);
        if (playerPos.x < 0) chunk_x_center--;
        //int chunk_y = playerPos.y / Chunk::CHUNK_SIZE;
        int chunk_z_center = (((int)playerPos.z) / Chunk::CHUNK_SIZE);
        if (playerPos.z < 0) chunk_z_center--;

        // generate the chunks near where the player is standing if they dont exist already
        for (int chunk_x = chunk_x_center - 1; chunk_x <= chunk_x_center + 1; chunk_x++) {
            for (int chunk_z = chunk_z_center - 1; chunk_z <= chunk_z_center + 1; chunk_z++) {
                if (!chunk_used[chunk_x][chunk_z]) {
                    Chunk* pChunk = new Chunk();
                    for (int block_x = 0; block_x < Chunk::CHUNK_SIZE; block_x++) {
                        for (int block_z = 0; block_z < Chunk::CHUNK_SIZE; block_z++) {
                            // Make this chunk flat ground
                            pChunk->m_pBlocks[block_x][0][block_z].m_blockType = BlockType::BlockType_Grass;
                        }
                    }
                    chunkList.push_back(pChunk);
                    glm::vec3 pos = { chunk_x, 0, chunk_z };
                    pChunk->m_position = pos;
                    pChunk->m_position.y = 0;
                    pChunk->update();
                    chunk_used[chunk_x][chunk_z] = true;

                    std::cout << "Generating chunk at (" << chunk_x << ", 0, " << chunk_z << ")" << std::endl;
                }
            }
        }
    //}
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
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    setupTextures();
    Shader ourShader("box.vs", "box.fs");

    glEnable(GL_DEPTH_TEST);

    // Main loop
    bool forward = false;
    bool backward = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;

    //std::thread terrainThread(generateTerrain);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);

        ourShader.use();

        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        unsigned int layerLoc = glGetUniformLocation(ourShader.ID, "layer");

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0, 0.0, -5.0));
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.getViewMatrix(playerPos);
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.getFov()), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 trans = glm::mat4(1.0f);
        trans = projection * view * model;

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glUniform1i(layerLoc, 1); // force the block texture for now
        glDrawArrays(GL_TRIANGLES, 0, 36);

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