#include <SDL3/SDL.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include "PropertyTypes.hpp"

/* Property Registry
   * This map stores the properties available in the game
   and which objects have that property
   * You can move the property registration to another file
   but for sake of better understanding, all registrations 
   are in this main file
*/
std::map<std::string, std::vector<std::string>> propertyRegistry;

/* Property Values
   * This map stores the values for each property per game object that has that property
   * Above disclaimer for moving to another file applies for this too
*/
std::map<std::string, std::map<std::string, PropertyValue>> propertyValues;

/* Get Property Helper
   * This function returns the property value for a particular game object
   * This does not handle data validation or graceful error handling, make sure your engine does
*/
template <typename T>
T& GetProperty(const std::string& propertyName, const std::string& gameId) {
    return std::get<T>(propertyValues[propertyName][gameId]);
}

/* Set Property Helper
   * This function sets the property value for a particular game object
   * This does not handle data validation or graceful error handling, make sure your engine does
*/
template <typename T>
void SetProperty(const std::string& propertyName, const std::string& gameId, const T& value) {
    propertyValues[propertyName][gameId] = value;
}

/* Movement Handler Function
   * This Function handles user input and applies movement to that game object 
     using its velocity to determine how fast it should move
*/
void moveHandler(const std::string& gameId) {
    const bool* keys = SDL_GetKeyboardState(nullptr);
    float speed = GetProperty<float>("velocity", gameId);
    SDL_FRect& rect = GetProperty<SDL_FRect>("shape", gameId);

    if (keys[SDL_SCANCODE_UP]) rect.y -= speed;
    if (keys[SDL_SCANCODE_DOWN]) rect.y += speed;
    if (keys[SDL_SCANCODE_LEFT]) rect.x -= speed;
    if (keys[SDL_SCANCODE_RIGHT]) rect.x += speed;
}

/* Main Function
   * This handles the property registration, alloting property values
     and the main game loop
*/
int main(int argc, char* argv[]) {

    // Initializing SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Initializing a SDL window
    SDL_Window* window = SDL_CreateWindow("It's the property that counts",
                                          800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Initializing a SDL renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    /* Registering the properties
       * This is just an example, you can define these properties in many different ways
       * You can even use numbers instead of strings, lots of freedom available!
       * For string it's recommended to use enums to avoid annoying bugs
    */
    propertyRegistry["shape"] = {"rect1"}; // "shape" is the property identifier and "rect1" is the guid of the game object
    propertyRegistry["color"] = {"rect1"};
    propertyRegistry["velocity"] = {"rect1"};
    propertyRegistry["moveable"] = {"rect1"};

    /* Assigning Values to the properties
    */
    SetProperty("shape", "rect1", SDL_FRect{100.0f, 100.0f, 100.0f, 80.0f}); // shape property, which is assigned to rect1 should have this value SDL_FRect{100.0f, 100.0f, 100.0f, 80.0f}
    SetProperty("color", "rect1", SDL_Color{255, 0, 0, 255});
    SetProperty("velocity", "rect1", 4.0f);
    SetProperty("moveable", "rect1", std::function<void(const std::string&)>(moveHandler));

    // Main Game Loop
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Handling the user input for movement
        for (const auto& gameId : propertyRegistry["moveable"]) {
            auto& handler = GetProperty<std::function<void(const std::string&)>>("moveable", gameId);
            handler(gameId);
        }

        // Rendering the scene 
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White Background
        SDL_RenderClear(renderer);

        // Rendering all game objects with shape property
        // Dislaimer: You can also have a property called visible, which decides which objects are visible and which aren't
        for (const auto& gameId : propertyRegistry["shape"]) {
            SDL_FRect rect = GetProperty<SDL_FRect>("shape", gameId);
            SDL_Color color = GetProperty<SDL_Color>("color", gameId);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &rect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Limiting frame rate 
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
