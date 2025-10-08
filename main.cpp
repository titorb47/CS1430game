#include "SDL_Plotter.h"

int main(int argc, char **argv)
{
    // Create a plotter window
    SDL_Plotter plotter(500, 500);

    // Main loop to keep the window open
    while (!plotter.getQuit())
    {
        // Draw a diagonal red line
        for (int i = 0; i < 500; i++)
        {
            plotter.plotPixel(i, i, 255, 0, 0); // (x, y, R, G, B)
        }
        // Update the display
        plotter.update();
    }
    return 0;
}