#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>

int main(int argc, char* args[])
{
    // Initialize everything SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window that can be resized, center it, and check if it is created
    SDL_Window* window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a renderer and check if it was created
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL)
    {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 1;
    }

    // Load an image from textures/smiles.png
    SDL_Surface* surface = IMG_Load("textures/smiles.png");
    if (surface == NULL)
    {
        printf("IMG_Load Error: %s\n", IMG_GetError());
        return 1;
    }

    // Create a texture from the surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
    {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        return 1;
    }

    // Free the surface
    SDL_FreeSurface(surface);

    // Set the color of the renderer to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Draw the entire screen black
    SDL_RenderClear(renderer);

    SDL_Rect rect = {0, 0, 128, 128};

    //Create a timer to control the animation
    Uint32 start;
    Uint32 delay = 1000 / 60;
    // Start the timer
    start = SDL_GetTicks();

    // Main loop
    bool quit = false;
    int state = 0;

    int windowWidth = 640;
    int windowHeight = 480;
    
    while (!quit)
    {
        // Handle events
        SDL_Event event;

        // bounce the texture around the screen after a delay
        if (SDL_GetTicks() - start > delay)
        {
            // Move the texture with a state machine to determine the direction. Also check if it hits the side or bottom to switch direction
            switch (state)
            {
                case 0:
                    rect.x += 1;
                    rect.y += 1;
                    if (rect.x >= windowWidth - rect.w)
                    {
                        state = 1;
                    }
                    else if(rect.y >= windowHeight - rect.h)
                    {
                        state = 3;
                    }
                    break;
                case 1:
                    rect.x -= 1;
                    rect.y += 1;
                    if (rect.x <= 0)
                    {
                        state = 0;
                    }

                    else if(rect.y >= windowHeight - rect.h)
                    {
                        state = 2;
                    }
                    break;
                case 2:
                    rect.x -= 1;
                    rect.y -= 1;
                    if (rect.x <= 0)
                    {
                        state = 3;
                    }

                    else if(rect.y <= 0)
                    {
                        state = 1;
                    }
                    break;
                case 3:
                    rect.x += 1;
                    rect.y -= 1;
                    if (rect.x >= windowWidth - rect.w)
                    {
                        state = 2;
                    }

                    else if(rect.y <= 0)
                    {
                        state = 0;
                    }
                    break;
            }

            // Restart the timer
            start = SDL_GetTicks();
        }


        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    printf("Mouse button down at (%d, %d)\n", event.button.x, event.button.y);
                    // Move the texture to the mouse position
                    rect.x = event.button.x - rect.w / 2;
                    rect.y = event.button.y - rect.h / 2;
                    break;
                
                // Handle window resizing
                case SDL_WINDOWEVENT:
                    switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_RESIZED:
                            windowWidth = event.window.data1;
                            windowHeight = event.window.data2;
                            // keep the texture inside the window
                            if (rect.x > windowWidth - rect.w)
                            {
                                rect.x = windowWidth - rect.w;
                            }
                            else if(rect.x < 0)
                            {
                                rect.x = 0;
                            }
                            if (rect.y > windowHeight - rect.h)
                            {
                                rect.y = windowHeight - rect.h;
                            }
                            else if(rect.y < 0)
                            {
                                rect.y = 0;
                            }
                            break;
                    }
                    break;
            }
        }

        // Clear the renderer
        SDL_RenderClear(renderer);

        // Draw the texture
        SDL_RenderCopy(renderer, texture, NULL, &rect);

        // Update the screen
        SDL_RenderPresent(renderer);
    }

}