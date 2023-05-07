#include <iostream>
#include <SDL2/SDL.h>
#include "Game.cpp"















int main() {
    // Initialize SDL Video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("My SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, 1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Set the draw color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Clear the screen with the draw color
    SDL_RenderClear(renderer);

    // Update the screen
    SDL_RenderPresent(renderer);

    // Wait for the user to close the window
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    // Clean up and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}