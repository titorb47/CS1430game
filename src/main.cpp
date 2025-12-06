/*
 * Author: (Based on The Builder's 3D projection logic)
 * Assignment Title: 3D Chest Menu
 * Assignment Description: A main menu with a 3D rotating chest, wave background, and 3D floating buttons.
 * Note: Requires SDL2. Compile with -lSDL2.
 */


#include "Game.h"
#include "GameVariables.h"
#include <SDL.h>
#include <SDL_main.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;


// --- 3D MATH & STRUCTURES ---

struct Vec3
{
    float x, y, z;
};

struct Connection
{
    int a, b;
};

// Represents a solid quad (like a button) in 3D space
struct Quad3D
{
    Vec3 p[4]; // 4 corners
    SDL_Color color;
    string label; // ID for clicking
};

// Rotate a point around the X axis
void rotateX(Vec3& p, float angle)
{
    float y = p.y;
    float z = p.z;
    p.y = y * cos(angle) - z * sin(angle);
    p.z = y * sin(angle) + z * cos(angle);
}

// Rotate a point around the Y axis
void rotateY(Vec3& p, float angle)
{
    float x = p.x;
    float z = p.z;
    p.x = x * cos(angle) - z * sin(angle);
    p.z = x * sin(angle) + z * cos(angle);
}

// Rotate a point around a specific pivot point (used for the Lid Hinge)
void rotateAroundPointX(Vec3& p, Vec3 pivot, float angle)
{
    p.y -= pivot.y;
    p.z -= pivot.z;
    rotateX(p, angle);
    p.y += pivot.y;
    p.z += pivot.z;
}

// Project 3D point to 2D screen coordinates
// Using simple perspective projection: x' = x / z
SDL_Point project(Vec3 p)
{
    float fov = 1000.0f;      // Field of view scalar
    float viewerDist = 10.0f; // Distance of camera from object

    // Scale down coordinates for logic, then project
    // (Assuming our model coordinates are roughly -2.0 to 2.0)
    float z_projected = (p.z / 100.0f) + viewerDist;

    // Protect against divide by zero behind camera
    if (z_projected <= 0.1f)
        z_projected = 0.1f;

    float x_2d = (p.x * fov) / z_projected;
    float y_2d = (p.y * fov) / z_projected;

    return {
        (int)(x_2d + WINDOW_WIDTH / 2),
        (int)(y_2d + WINDOW_HEIGHT / 2) };
}

// --- DRAWING HELPERS ---

void drawWaves(SDL_Renderer* renderer, float timeOffset)
{
    // 1. Clear with Deep Ocean color
    SDL_SetRenderDrawColor(renderer, 0, 0, 30, 255);
    SDL_RenderClear(renderer);

    // 2. Back Wave (Darker, Slower, Tall)
    SDL_SetRenderDrawColor(renderer, 0, 50, 100, 255);
    for (int x = 0; x < WINDOW_WIDTH; x++)
    {
        // Amplitude 60, Frequency 0.005 (Wide)
        int y = 350 + (60 * sin(0.005 * x + timeOffset * 0.5));
        SDL_RenderDrawLine(renderer, x, y, x, WINDOW_HEIGHT);
    }

    // 3. Middle Wave (Standard Blue)
    SDL_SetRenderDrawColor(renderer, 0, 105, 148, 255);
    for (int x = 0; x < WINDOW_WIDTH; x++)
    {
        int y = 450 + (30 * sin(0.01 * x + timeOffset)) + (15 * sin(0.03 * x + timeOffset * 2));
        SDL_RenderDrawLine(renderer, x, y, x, WINDOW_HEIGHT);
    }

    // 4. Front Wave (Lighter, Faster)
    SDL_SetRenderDrawColor(renderer, 0, 150, 200, 255);
    for (int x = 0; x < WINDOW_WIDTH; x++)
    {
        int y = 520 + (25 * sin(0.012 * x + timeOffset + 2));
        SDL_RenderDrawLine(renderer, x, y, x, WINDOW_HEIGHT);
    }

    // 5. Foam/Surface Detail (Brightest)
    SDL_SetRenderDrawColor(renderer, 100, 200, 255, 100);
    for (int x = 0; x < WINDOW_WIDTH; x++)
    {
        int y = 540 + (20 * sin(0.02 * x + timeOffset * 1.5 + 4));
        SDL_RenderDrawLine(renderer, x, y, x, WINDOW_HEIGHT);
    }
}

// Helper to draw a filled 3D Quad using SDL Geometry
void drawFilledQuad(SDL_Renderer* renderer, vector<SDL_Point>& projPoints, SDL_Color c)
{
    if (projPoints.size() != 4)
        return;

    // Convert SDL_Point to SDL_Vertex for Geometry rendering
    SDL_Vertex verts[4];
    for (int i = 0; i < 4; i++)
    {
        verts[i].position.x = (float)projPoints[i].x;
        verts[i].position.y = (float)projPoints[i].y;
        verts[i].color = c;
        verts[i].tex_coord = { 0, 0 };
    }

    // Define two triangles to make the quad (0-1-2 and 0-2-3)
    int indices[6] = { 0, 1, 2, 0, 2, 3 };
    SDL_RenderGeometry(renderer, nullptr, verts, 4, indices, 6);

    // Draw outline
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black outline
    for (int i = 0; i < 4; i++)
    {
        SDL_RenderDrawLine(renderer, projPoints[i].x, projPoints[i].y,
            projPoints[(i + 1) % 4].x, projPoints[(i + 1) % 4].y);
    }
}


//This initializes the game's memory location
Game *game = nullptr;



// --- MAIN PROGRAM ---
int main(int argc, char * args[]) {

	game = new Game();
	game->setRunning(false);

    // 1. Setup
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        return 1;
    SDL_Window* window = SDL_CreateWindow("3D Chest Menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    RENDERER = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    //Can use window like a bool
    if (window) {
        cout << "Window created!" << endl;
    }

    //SDL_CreateRenderer(window, index, flags)
    if (RENDERER) {
        cout << "Renderer created!" << endl;
    }

    bool quit = false;
    SDL_Event e;

    // 2. Define 3D Models
    // Chest Body Dimensions
    float w = 2.0f; // Width
    float h = 1.2f; // Height
    float d = 1.2f; // Depth

    // Vertices for the BODY (Box)
    // 0-3: Front Face, 4-7: Back Face
    vector<Vec3> bodyVerts = {
        {-w, -h, -d}, {w, -h, -d}, {w, h, -d}, {-w, h, -d}, // Front
        {-w, -h, d},
        {w, -h, d},
        {w, h, d},
        {-w, h, d} // Back
    };

    // Connections (Wireframe lines)
    vector<Connection> bodyEdges = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Front face
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4}, // Back face
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7} // Connecting lines
    };

    // Vertices for the LID (Box on top)
    float lh = 0.5f; // Lid Height
    vector<Vec3> lidVerts = {
        {-w, -lh - h, -d}, {w, -lh - h, -d}, {w, -h, -d}, {-w, -h, -d}, // Front Lid
        {-w, -lh - h, d},
        {w, -lh - h, d},
        {w, -h, d},
        {-w, -h, d} // Back Lid
    };
    // Note: The hinge is at lidVerts[6] and lidVerts[7] (Top-Back of body)
    Vec3 hingePoint = { 0, -h, d };

    vector<Connection> lidEdges = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7} };

    // 3. Define Buttons (as 3D Quads inside the chest)
    // Z is 0 (Center of chest), Y is slightly down
    vector<Quad3D> buttons;

    // Start Button (Top, Wide)
    buttons.push_back({
        {{-1.2f, -0.2f, 0}, {1.2f, -0.2f, 0}, {1.2f, 0.3f, 0}, {-1.2f, 0.3f, 0}},
        {218, 165, 32, 255},
        "Start" // Gold
        });

    // Difficulty Buttons (Bottom Row)
    // Easy (Green)
    buttons.push_back({ {{-1.4f, 0.5f, 0}, {-0.6f, 0.5f, 0}, {-0.6f, 1.0f, 0}, {-1.4f, 1.0f, 0}},
                       {50, 205, 50, 255},
                       "Easy" });
    // Medium (Yellow)
    buttons.push_back({ {{-0.4f, 0.5f, 0}, {0.4f, 0.5f, 0}, {0.4f, 1.0f, 0}, {-0.4f, 1.0f, 0}},
                       {255, 255, 0, 255},
                       "Medium" });
    // Hard (Red)
    buttons.push_back({ {{0.6f, 0.5f, 0}, {1.4f, 0.5f, 0}, {1.4f, 1.0f, 0}, {0.6f, 1.0f, 0}},
                       {220, 20, 60, 255},
                       "Hard" });

    // Animation State
    float angleY = 0.0f;   // Chest rotation
    float lidAngle = 0.0f; // Opening animation
    float waveTime = 0.0f;

    // --- LOOP ---
    while (!quit)
    {
        // Input
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        SDL_PollEvent(&e);
        {
            if (e.type == SDL_QUIT)
                quit = true;

            // Basic Hit Detection for 3D buttons
            // (We check against the *last rendered* 2D position)
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                // Logic is handled in render loop where we know the 2D coords
            }
        }

        // Logic
        waveTime += 0.05f;
        // Sway the chest slightly left and right
        angleY = 0.2f * sin(waveTime * 0.5f);

        // Open the lid
        if (lidAngle > -1.5f)
            lidAngle -= 0.02f; // Rotate back ~85 degrees

        // Render
        drawWaves(RENDERER, waveTime);

        // --- 3D PIPELINE ---

        // 1. Transform Body
        vector<SDL_Point> projBody;
        for (auto& v : bodyVerts)
        {
            Vec3 p = v;
            rotateY(p, angleY); // Rotate entire chest
            projBody.push_back(project(p));
        }

        // 2. Transform Lid
        vector<SDL_Point> projLid;
        for (auto& v : lidVerts)
        {
            Vec3 p = v;
            rotateAroundPointX(p, hingePoint, lidAngle); // Open lid
            rotateY(p, angleY);                          // Rotate with chest
            projLid.push_back(project(p));
        }

        // 3. Draw Body "Back" lines (indices 4,5,6,7 are back face)
        SDL_SetRenderDrawColor(RENDERER, 60, 40, 20, 255);
        for (auto& edge : bodyEdges)
        {
            // Simple depth hack: if both points are back indices, draw dim
            if (edge.a >= 4 && edge.b >= 4)
            {
                SDL_RenderDrawLine(RENDERER, projBody[edge.a].x, projBody[edge.a].y,
                    projBody[edge.b].x, projBody[edge.b].y);
            }
        }

        // 4. Draw & Handle Buttons (Inside the chest)
        // They are positioned at Z=0, so they are naturally "between" front and back
        bool clicked = (e.type == SDL_MOUSEBUTTONDOWN);
        /*FIXME*/

        for (auto& btn : buttons)
        {
            vector<SDL_Point> projQuad;
            int minX = 10000, maxX = -10000, minY = 10000, maxY = -10000;

            // Project all 4 corners of the button
            for (int i = 0; i < 4; i++)
            {
                Vec3 p = btn.p[i];
                rotateY(p, angleY); // Rotate buttons with the chest!
                SDL_Point sc = project(p);
                projQuad.push_back(sc);

                // Update bounding box for mouse detection
                if (sc.x < minX)
                    minX = sc.x;
                if (sc.x > maxX)
                    maxX = sc.x;
                if (sc.y < minY)
                    minY = sc.y;
                if (sc.y > maxY)
                    maxY = sc.y;
            }

            // Check Mouse Hover
            SDL_Color drawCol = btn.color;
            if (mx >= minX && mx <= maxX && my >= minY && my <= maxY && lidAngle < -1.0f)
            {
                // Highlight
                drawCol.r = min(255, drawCol.r + 50);
                drawCol.g = min(255, drawCol.g + 50);
                drawCol.b = min(255, drawCol.b + 50);

                if (clicked)
                {
                    cout << "Clicked: " << btn.label << endl;
                    
                    if (btn.label == "Start") {
                        game->init();
                        Game::setRunning(true);
                        quit = true;
                        ENEMY_SPAWNRATE_MS -= ENEMY_SPAWNRATE_MODIFIER;
						TIME_STARTED = SDL_GetTicks();
                    }
                    if (btn.label == "Easy") {
                        ENEMY_SPAWNRATE_MODIFIER = 0;
                    }
                    else if (btn.label == "Medium") {
                        ENEMY_SPAWNRATE_MODIFIER = 100;
                    }
                    else if (btn.label == "Hard") {
                        ENEMY_SPAWNRATE_MODIFIER = 250;
                    }
                }
            }

            // Draw the solid button quad
            // Only draw if lid is opening
            if (lidAngle < -0.2f)
            {
                drawFilledQuad(RENDERER, projQuad, drawCol);
            }
        }

        // 5. Draw Chest "Front" Lines and Lid
        SDL_SetRenderDrawColor(RENDERER, 139, 69, 19, 255); // Standard wood

        // Draw rest of Body Wireframe
        for (auto& edge : bodyEdges)
        {
            // Redraw all lines to ensure front lines cover buttons if needed
            // (Wireframe is transparent, so this just reinforces the lines)
            SDL_RenderDrawLine(RENDERER, projBody[edge.a].x, projBody[edge.a].y,
                projBody[edge.b].x, projBody[edge.b].y);
        }

        // Draw Lid Wireframe
        SDL_SetRenderDrawColor(RENDERER, 160, 82, 45, 255);
        for (auto& edge : lidEdges)
        {
            SDL_RenderDrawLine(RENDERER, projLid[edge.a].x, projLid[edge.a].y,
                projLid[edge.b].x, projLid[edge.b].y);
        }

        SDL_RenderPresent(RENDERER);
    }

    while (game->running() == true) {
        game->runGame();
    }

    game->clean();
	return 0;
}
