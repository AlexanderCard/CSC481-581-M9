
# It's the property that counts

In this assignment, you’ll build a small **property-centric** system using **C++**, **CMake**, and **SDL3** to render and move objects;  not by writing custom classes, but by organizing properties as data.

You’ll learn how real game engines evolve from object-centric (OO-style) code to data-oriented, **property-driven** systems, which is the foundation of modern ECS (Entity Component Systems).

## Objectives

By the end of this assignment, you’ll be able to:

 - Understand the **Property-Centric Game Object Model**
    
 - Manage object data using **maps of properties**, not class hierarchies
        
 - Extend the system by adding new game entities and properties
    
 - Recognize the benefits of **data-driven** engine design
    
##  Setup Instructions

Follow these steps to clone, build, and run the project on your machine:

### 1. Clone the repository

	git clone https://github.com/AlexanderCard/CSC481-581-M9.git

### 2. Install SDL3

> SDL3 is the latest version of the Simple DirectMedia Layer.  
> You’ll only need the core SDL3 library for this assignment.

#### On **macOS** (with Homebrew):

	  brew install sdl3 

#### On **Ubuntu/Debian**:

	  sudo apt install libsdl3-dev 

#### On **Arch Linux**:

	  sudo pacman -S sdl3 

#### On **Windows**: (unverified)

Use [MSYS2](https://www.msys2.org/) or [vcpkg](https://vcpkg.io/).

  pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL3 


### 3. Build the project

  `mkdir build && cd build
   cmake ..
   make` 

> If CMake cannot find SDL3, make sure SDL3_DIR is correctly set.

### 4. Run the example

 ./property_model 

You should see a red rectangle in a window.  
Use the arrow keys to move it,  its velocity controls how fast it moves.

----------

## Property-Centric Design

###  What is a Property-Centric Model?

In traditional OOP-based engines, you might define a GameObject class like:

    class  GameObject {
    SDL_FRect shape;
    SDL_Color color; float velocity; bool moveable;
    };

This is **object-centric** — each object owns its properties.

A **property-centric** model flips that idea:  
Each property (like color, shape, or velocity) owns the data for all game objects that have it.

Think of it like a table of properties:

| Property | GameObject ID |       Value      |
|:--------:|:-------------:|:----------------:|
| shape    | rect1         | (x, y, w, h)     |
| color    | rect1         | (255, 0, 0, 255) |
| velocity | rect1         | 4.0f             |
| moveable | rect1         | moveHandler()    |

This separation helps you:

  Easily **add new properties** without modifying object classes
    
  Manage behavior **per property**, not per class
    
  Mimic how **ECS systems** store and process data efficiently
    

----------

###  Architecture Overview

The code uses:

  - propertyRegistry → which objects have which properties
    
  - propertyValues → property data stored as std::variant
    
  - PropertyValue type → defined in PropertyTypes.hpp
    

Example:

    propertyValues["velocity"]["rect1"] = 4.0f;
    propertyValues["color"]["rect1"] = SDL_Color{255, 0, 0, 255};
    propertyValues["moveable"]["rect1"] = moveHandler;

At runtime, each property handler (like moveable) updates its objects.

##  Your Task

Add a **second game object**: a blue circle that is **non-moveable**.

### Requirements

1. Create a new object called circle1  
2. Give it a shape (center + radius) (You can also create a square, circle is optional challenge)
3. Give it a color (blue)  
4. It should **not** respond to keyboard input  
5. Render both rect1 and circle1 together

----------

###  How to Draw a Circle in SDL3

- SDL3 doesn’t have a built-in circle drawing function, but here’s a simple approach:
- A simple algorithm is to iterate from x = -radius to x = radius and y = -radius to y = radius, and if x*x + y*y <= radius*radius, draw a point at (centerX + x, centerY + y).

## Helpful Resources

  [SDL3 Docs (official)](https://wiki.libsdl.org/SDL3/FrontPage)
    
  [Mike Shah SDL3 Setup Guide](https://www.youtube.com/watch?v=kyD5H6w1x-o)

##  Final Notes

Keep your code modular, and try exploring new properties like:

  - "gravity" (applies to moveable objects)
    
  - "bounce" (reverses direction on collision)
    
  - "rotation" (changes orientation over time)
    

Each new property should fit naturally into this system, no new class definitions required.
