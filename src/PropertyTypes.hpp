#pragma once
#include <SDL3/SDL.h>
#include <functional>
#include <string>
#include <variant>

// This File contains the property types that are supported, to support extensibility
// By using std::variant we remain type-safe and avoid heap allocation overhead of std::any
using PropertyValue = std::variant<
    int,
    float,
    SDL_FRect,
    SDL_Color,
    std::function<void(const std::string&)>,
    std::string // currently unused in the code
>;
