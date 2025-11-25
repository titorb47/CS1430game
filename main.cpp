#include <SDL2/SDL_mixer.h>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "SDL_Plotter.h"

using namespace std;

struct Obstacle {
    int x, y, w, h;
    int r, g, b;
};

struct Particle {
    float x, y, vx, vy;
    int r, g, b;
    float life;
};

class SubwaySurferGame {
   private:
    SDL_Plotter& plotter;
    int screenWidth, screenHeight;
    int playerX, playerY;
    vector<Obstacle> obstacles;
    vector<Particle> particles;
    int score, lives, level;
    float gameTime;
    int gameState;  // 0: playing, 1: paused, 2: game over

   public:
    SubwaySurferGame(SDL_Plotter& g)
        : plotter(g), screenWidth(1000), screenHeight(600), playerX(450), playerY(500), score(0), lives(3), level(1), gameTime(0), gameState(0) {
        generateObstacles();
    }

    void generateObstacles() {
        obstacles.clear();
        for (int i = 1; i < 8; i++) {
            int lane = rand() % 3;
            int x = 200 + lane * 250;
            int y = i * 150 - (int)(gameTime * 3);

            if (rand() % 2 == 0) {
                obstacles.push_back({x, y, 120, 50, 220, 80, 80});
            } else {
                obstacles.push_back({x, y, 120, 50, 80, 180, 220});
            }
        }
    }

    void drawTerrain() {
        // Sky gradient
        for (int y = 0; y < 250; y++) {
            float ratio = (float)y / 250;
            int r = (int)(100 + ratio * 30);
            int g = (int)(180 + ratio * 40);
            int b = (int)(255 - ratio * 50);

            for (int x = 0; x < screenWidth; x++) {
                plotter.plotPixel(x, y, r, g, b);
            }
        }

        // Buildings in background
        drawBuilding(50, 80, 120, 150, 40, 50, 70);
        drawBuilding(250, 120, 100, 100, 35, 45, 65);
        drawBuilding(500, 70, 140, 180, 45, 55, 75);
        drawBuilding(750, 100, 130, 140, 38, 48, 68);

        // Tracks/Road
        for (int y = 250; y < screenHeight; y++) {
            float ratio = (float)(y - 250) / (screenHeight - 250);
            int r = (int)(60 + ratio * 30);
            int g = (int)(65 + ratio * 30);
            int b = (int)(80 + ratio * 40);

            for (int x = 0; x < screenWidth; x++) {
                plotter.plotPixel(x, y, r, g, b);
            }
        }

        // Track lines moving
        int lineOffset = (int)(gameTime * 2) % 100;
        for (int y = 250 + lineOffset; y < screenHeight; y += 100) {
            for (int x = 0; x < screenWidth; x++) {
                if (x % 20 < 10) {
                    plotter.plotPixel(x, y, 150, 150, 50);
                }
            }
        }

        // Lane dividers
        for (int y = 250; y < screenHeight; y++) {
            plotter.plotPixel(200, y, 200, 200, 100);
            plotter.plotPixel(450, y, 200, 200, 100);
            plotter.plotPixel(700, y, 200, 200, 100);
        }
    }

    void drawBuilding(int x, int y, int w, int h, int r, int g, int b) {
        // Building body
        for (int py = y; py < y + h; py++) {
            for (int px = x; px < x + w; px++) {
                plotter.plotPixel(px, py, r, g, b);
            }
        }

        // Windows
        for (int wy = y + 10; wy < y + h; wy += 25) {
            for (int wx = x + 10; wx < x + w; wx += 20) {
                for (int pwy = wy; pwy < wy + 15; pwy++) {
                    for (int pwx = wx; pwx < wx + 12; pwx++) {
                        plotter.plotPixel(pwx, pwy, 255, 255, 100);
                    }
                }
            }
        }
    }

    void drawPlayer() {
        // Body
        for (int py = playerY; py < playerY + 50; py++) {
            for (int px = playerX; px < playerX + 40; px++) {
                plotter.plotPixel(px, py, 0, 200, 255);
            }
        }

        // Head
        for (int py = playerY - 30; py < playerY; py++) {
            for (int px = playerX + 5; px < playerX + 35; px++) {
                int dist = (int)sqrt(pow(px - playerX - 20, 2) + pow(py - playerY + 15, 2));
                if (dist < 15) {
                    plotter.plotPixel(px, py, 255, 150, 50);
                }
            }
        }

        // Eyes
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                plotter.plotPixel(playerX + 12 + i, playerY - 15 + j, 0, 0, 0);
                plotter.plotPixel(playerX + 28 + i, playerY - 15 + j, 0, 0, 0);
            }
        }

        // Running legs animation
        int legOffset = (int)(gameTime * 10) % 20;
        for (int py = playerY + 50; py < playerY + 70; py++) {
            for (int px = playerX + 8 + legOffset; px < playerX + 15 + legOffset; px++) {
                plotter.plotPixel(px, py, 100, 50, 150);
            }
            for (int px = playerX + 25 - legOffset; px < playerX + 32 - legOffset; px++) {
                plotter.plotPixel(px, py, 100, 50, 150);
            }
        }
    }

    void drawObstacles() {
        for (auto& obs : obstacles) {
            // Obstacle body with gradient
            for (int py = obs.y; py < obs.y + obs.h; py++) {
                for (int px = obs.x; px < obs.x + obs.w; px++) {
                    plotter.plotPixel(px, py, obs.r, obs.g, obs.b);
                }
            }

            // Obstacle shine effect
            for (int py = obs.y; py < obs.y + 8; py++) {
                for (int px = obs.x; px < obs.x + obs.w; px++) {
                    if ((px + py) % 3 == 0) {
                        plotter.plotPixel(px, py,
                                          min(255, obs.r + 60),
                                          min(255, obs.g + 60),
                                          min(255, obs.b + 60));
                    }
                }
            }

            // Border
            for (int i = 0; i < obs.w; i++) {
                plotter.plotPixel(obs.x + i, obs.y, 0, 0, 0);
                plotter.plotPixel(obs.x + i, obs.y + obs.h, 0, 0, 0);
            }
            for (int i = 0; i < obs.h; i++) {
                plotter.plotPixel(obs.x, obs.y + i, 0, 0, 0);
                plotter.plotPixel(obs.x + obs.w, obs.y + i, 0, 0, 0);
            }
        }
    }

    void drawParticles() {
        for (auto& p : particles) {
            if (p.life > 0) {
                if (p.x > 0 && p.x < screenWidth && p.y > 0 && p.y < screenHeight) {
                    int alpha = (int)(255 * (p.life / 1.0f));
                    plotter.plotPixel((int)p.x, (int)p.y,
                                      (p.r * alpha) / 255,
                                      (p.g * alpha) / 255,
                                      (p.b * alpha) / 255);
                }
            }
        }
    }

    void drawHUD() {
        // Score box top-left
        drawBox(10, 10, 200, 60, 30, 40, 60);
        drawTextSimple(25, 20, "SCORE: " + to_string(score), 255, 200, 100);

        // Lives box top-middle
        drawBox(350, 10, 200, 60, 30, 40, 60);
        drawTextSimple(365, 20, "LIVES: " + to_string(lives), 255, 100, 100);

        // Level box top-right
        drawBox(690, 10, 200, 60, 30, 40, 60);
        drawTextSimple(705, 20, "LEVEL: " + to_string(level), 100, 200, 255);
    }

    void drawBox(int x, int y, int w, int h, int r, int g, int b) {
        for (int py = y; py < y + h; py++) {
            for (int px = x; px < x + w; px++) {
                plotter.plotPixel(px, py, r, g, b);
            }
        }
        for (int i = 0; i < w; i++) {
            plotter.plotPixel(x + i, y, 200, 200, 150);
            plotter.plotPixel(x + i, y + h, 200, 200, 150);
        }
        for (int i = 0; i < h; i++) {
            plotter.plotPixel(x, y + i, 200, 200, 150);
            plotter.plotPixel(x + w, y + i, 200, 200, 150);
        }
    }

    void drawTextSimple(int x, int y, const string& text, int r, int g, int b) {
        for (char c : text) {
            for (int dy = 0; dy < 8; dy++) {
                for (int dx = 0; dx < 6; dx++) {
                    if ((dx + dy) % 2 == 0) {
                        plotter.plotPixel(x + dx, y + dy, r, g, b);
                    }
                }
            }
            x += 8;
        }
    }

    void drawGameOverScreen() {
        // Dark overlay
        for (int y = 150; y < 450; y++) {
            for (int x = 200; x < 800; x++) {
                plotter.plotPixel(x, y, 0, 0, 0);
            }
        }

        // Panel
        for (int y = 160; y < 440; y++) {
            for (int x = 210; x < 790; x++) {
                plotter.plotPixel(x, y, 50, 30, 80);
            }
        }

        // Border
        for (int i = 0; i < 580; i++) {
            plotter.plotPixel(210 + i, 160, 255, 100, 50);
            plotter.plotPixel(210 + i, 440, 255, 100, 50);
        }
        for (int i = 0; i < 280; i++) {
            plotter.plotPixel(210, 160 + i, 255, 100, 50);
            plotter.plotPixel(790, 160 + i, 255, 100, 50);
        }

        drawTextSimple(350, 180, "GAME OVER", 255, 100, 50);
        drawTextSimple(300, 260, "FINAL SCORE: " + to_string(score), 255, 200, 100);
        drawTextSimple(300, 320, "Press R to Restart or Q to Quit", 200, 200, 255);
    }

    void drawPauseScreen() {
        for (int y = 200; y < 400; y++) {
            for (int x = 300; x < 700; x++) {
                if ((x + y) % 4 == 0) {
                    plotter.plotPixel(x, y, 0, 0, 0);
                }
            }
        }
        drawTextSimple(350, 250, "PAUSED - Press P to Resume", 100, 200, 255);
    }

    void update(char key) {
        if (key == 'p') {
            gameState = (gameState == 0) ? 1 : 0;
        }
        if (key == 'q') {
            plotter.setQuit(true);
        }
        if (key == 'r' && gameState == 2) {
            score = 0;
            lives = 3;
            level = 1;
            gameTime = 0;
            gameState = 0;
            generateObstacles();
        }

        if (gameState == 0) {
            // Player movement
            if (key == 'a' && playerX > 150) playerX -= 120;
            if (key == 'd' && playerX < 850) playerX += 120;

            // Update game
            gameTime += 0.016f;

            // Update obstacles
            for (auto& obs : obstacles) {
                obs.y += 5;
                if (obs.y > screenHeight) {
                    obs.y = 0;
                    obs.x = 200 + (rand() % 3) * 250;
                    score += 10;
                    if (score % 100 == 0) level++;
                }

                // Collision detection
                if (playerX < obs.x + obs.w && playerX + 40 > obs.x &&
                    playerY < obs.y + obs.h && playerY + 70 > obs.y) {
                    lives--;
                    if (lives <= 0) {
                        gameState = 2;
                    }
                    obs.y = 0;
                    obs.x = 200 + (rand() % 3) * 250;
                }
            }

            // Particle update
            for (auto& p : particles) {
                p.x += p.vx;
                p.y += p.vy;
                p.vy += 0.1f;
                p.life -= 0.016f;
            }
        }
    }

    void render() {
        drawTerrain();
        drawObstacles();
        drawPlayer();
        drawParticles();
        drawHUD();

        if (gameState == 1) {
            drawPauseScreen();
        } else if (gameState == 2) {
            drawGameOverScreen();
        }
    }
};

int main(int argc, char** argv) {
    SDL_Plotter g(1000, 600);
    SubwaySurferGame game(g);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "SDL_mixer Error: " << Mix_GetError() << endl;
        return 1;
    }

    while (!g.getQuit()) {
        char key = '\0';
        if (g.kbhit()) {
            key = g.getKey();
        }

        game.update(key);
        game.render();
        g.update();
    }

    Mix_CloseAudio();
    return 0;
}
