#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#include <vector>
#include <string>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

// ===== TASK STRUCTURE =====
// This defines what a task looks like
struct Task {
    std::string title;      // The task name
    bool completed = false; // Is it done?
};

// ===== GLOBAL STATE =====
// This is our list of tasks
std::vector<Task> tasks;

// Buffer to store text the user types for new tasks
char new_task_input[256] = "";

int main(int, char**)
{
    // ===== SETUP SDL2 =====
    // This initializes SDL and creates the window
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    // ===== SETUP OPENGL =====
    // Tell SDL what OpenGL version we want
#if defined(__APPLE__)
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // ===== CREATE WINDOW =====
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_Window* window = SDL_CreateWindow("Task Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return 1;
    }

    // ===== CREATE OPENGL CONTEXT =====
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr)
    {
        printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
        return 1;
    }
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);  // Enable vsync

    // ===== SETUP IMGUI =====
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    // ===== SETUP IMGUI BACKENDS =====
    // Connect ImGui to SDL2 and OpenGL
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // ===== MAIN LOOP =====
    bool done = false;
    while (!done)
    {
        // Handle user input and window events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // ===== YOUR UI CODE GOES HERE =====
        // This is where we draw the task manager
        ImGui::Begin("Task Manager");

        ImGui::Text("Total tasks: %d", (int)tasks.size());
        ImGui::Separator();

        // Display each task with delete button
        for (size_t i = 0; i < tasks.size(); i++)
        {
            ImGui::Checkbox(("##task" + std::to_string(i)).c_str(), &tasks[i].completed);
            ImGui::SameLine();
            ImGui::Text("%s", tasks[i].title.c_str());
            ImGui::SameLine();

            // Delete button for this task
            if (ImGui::Button(("Delete##" + std::to_string(i)).c_str()))
            {
                tasks.erase(tasks.begin() + i);
                i--;  // Adjust index since we removed an element
            }
        }

        ImGui::Separator();

        // Input field for new task
        ImGui::Text("Add a new task:");
        ImGui::InputText("##new_task", new_task_input, sizeof(new_task_input));
        ImGui::SameLine();

        // Add button
        if (ImGui::Button("Add Task"))
        {
            if (strlen(new_task_input) > 0)
            {
                tasks.push_back({std::string(new_task_input), false});
                new_task_input[0] = '\0';  // Clear the input field
            }
        }
        
        ImGui::Separator();

        // Input field for new reminder
        ImGui::Text("Add a new reminder:");
        

        ImGui::End();

        // ===== RENDER =====
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // ===== CLEANUP =====
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
