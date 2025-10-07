#include <SDL3/SDL.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include "PropertyTypes.hpp"


std::map<std::string, std::vector<std::string>> propertyRegistry;
std::map<std::string, std::map<std::string, PropertyValue>> propertyValues;

template <typename T>
T& GetProperty(const std::string& propertyName, const std::string& gameId) {
    return std::get<T>(propertyValues[propertyName][gameId]);
}

template <typename T>
void SetProperty(const std::string& propertyName, const std::string& gameId, const T& value) {
    propertyValues[propertyName][gameId] = value;
}

void moveHandler(const std::string& gameId) {
    const bool* keys = SDL_GetKeyboardState(nullptr);
    float speed = GetProperty<float>("velocity", gameId);
    SDL_FRect& rect = GetProperty<SDL_FRect>("shape", gameId);

    if (keys[SDL_SCANCODE_UP]) rect.y -= speed;
    if (keys[SDL_SCANCODE_DOWN]) rect.y += speed;
    if (keys[SDL_SCANCODE_LEFT]) rect.x -= speed;
    if (keys[SDL_SCANCODE_RIGHT]) rect.x += speed;
}

void drawFilledCircle(SDL_Renderer* renderer, float centerX, float centerY, float radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}



int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("It's the property that counts", 
                                          800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }


    propertyRegistry["shape"] = {"rect1"};
    propertyRegistry["color"] = {"rect1"};
    propertyRegistry["velocity"] = {"rect1"};
    propertyRegistry["moveable"] = {"rect1"};

    propertyRegistry["shape"].push_back("circle1");
    propertyRegistry["color"].push_back("circle1");

    propertyRegistry["shape_type"] = {"rect1", "circle1"};


    SetProperty("shape", "rect1", SDL_FRect{100.0f, 100.0f, 100.0f, 80.0f});
    SetProperty("color", "rect1", SDL_Color{255, 0, 0, 255});
    SetProperty("velocity", "rect1", 4.0f);
    SetProperty("moveable", "rect1", std::function<void(const std::string&)>(moveHandler));
    SetProperty("shape_type", "rect1", std::string("rectangle")); 


    SetProperty("shape", "circle1", SDL_FRect{300.0f, 200.0f, 100.0f, 100.0f});
    SetProperty("color", "circle1", SDL_Color{0, 0, 255, 255});
    SetProperty("shape_type", "circle1", std::string("circle")); 



    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        for (const auto& gameId : propertyRegistry["moveable"]) {
            auto& handler = GetProperty<std::function<void(const std::string&)>>("moveable", gameId);
            handler(gameId);
        }

        // Render scene
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for (const auto& gameId : propertyRegistry["shape"]) {
            SDL_FRect rect = GetProperty<SDL_FRect>("shape", gameId);
            SDL_Color color = GetProperty<SDL_Color>("color", gameId);
            std::string type = GetProperty<std::string>("shape_type", gameId);

            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            
            if (type == "rectangle") {
                SDL_RenderFillRect(renderer, &rect);
            } else if (type == "circle") {
                float centerX = rect.x + rect.w / 2.0f;
                float centerY = rect.y + rect.h / 2.0f;
                float radius = rect.w / 2.0f; 
                drawFilledCircle(renderer, centerX, centerY, radius);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}