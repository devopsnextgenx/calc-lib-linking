# Shape System Usage Guide

## Overview

This C++ shape system provides a complete object-oriented framework for creating, managing, and interacting with geometric shapes in SDL-based applications. The system supports inheritance, runtime action assignment, event handling, and comprehensive shape management.

## Key Features

- **Object-Oriented Design**: Base `Shape` class with derived classes for Circle, Rectangle, and Triangle
- **Runtime Action Assignment**: Attach callbacks for click, double-click, drag, and hover events
- **Event Management**: Separate `EventHandler` class for processing mouse events
- **Shape Management**: `ShapeManager` class for organizing shapes with filtering and z-order support
- **Flexible Options**: Configure shapes as selectable, draggable, clickable, or visible

## Basic Usage

### 1. Create Shape Manager and Event Handler

```cpp
#include "graphics/graphics.h"

// Create the core components
graphics::ShapeManager shapeManager;
graphics::EventHandler eventHandler(&shapeManager);
```

### 2. Define Action Callbacks

```cpp
// Callback for click events
void onShapeClick(graphics::Shape* shape, const graphics::MouseEventData& event) {
    printf("Shape clicked: %s at (%.2f, %.2f)\n", 
           shape->getType().c_str(), event.x, event.y);
    shape->setSelected(!shape->isSelected());
}

// Callback for drag events
void onShapeDrag(graphics::Shape* shape, const graphics::MouseEventData& event) {
    printf("Shape dragged by (%.2f, %.2f)\n", event.deltaX, event.deltaY);
}
```

### 3. Create Shapes with Options

```cpp
// Configure shape behavior
graphics::ShapeOptions options;
options.selectable = true;
options.draggable = true;
options.clickable = true;
options.onClickAction = onShapeClick;
options.onDragAction = onShapeDrag;

// Create different types of shapes
auto circle = shapeManager.createCircle(150, 150, 50, 0xFF00FF00, options);
auto rectangle = shapeManager.createRectangle(300, 200, 100, 80, 0xFFFF0000, options);
auto triangle = shapeManager.createTriangle(400, 100, 450, 200, 350, 200, 0xFF0000FF, options);
```

### 4. Handle Events in Main Loop

```cpp
SDL_Event event;
while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
        quit = true;
    } else {
        eventHandler.handleEvent(event);  // Process all mouse events
    }
}

// Update and render
eventHandler.update();
SDL_FillSurfaceRect(surface, NULL, 0xFF000000);  // Clear screen
shapeManager.drawAll(surface);  // Draw all shapes
SDL_UpdateWindowSurface(window);
```

## Shape Options Configuration

### ShapeOptions Structure

```cpp
struct ShapeOptions {
    bool selectable = true;        // Can be selected
    bool draggable = false;        // Can be dragged
    bool clickable = true;         // Responds to clicks
    bool visible = true;           // Is visible and rendered
    int zOrder = 0;               // Z-order for layering
    ActionCallback onClickAction = nullptr;
    ActionCallback onDoubleClickAction = nullptr;
    ActionCallback onDragAction = nullptr;
    ActionCallback onHoverAction = nullptr;
};
```

### Example Configurations

```cpp
// Read-only background shape
graphics::ShapeOptions bgOptions;
bgOptions.selectable = false;
bgOptions.draggable = false;
bgOptions.clickable = false;
bgOptions.zOrder = -10;  // Behind everything

// Interactive UI element
graphics::ShapeOptions uiOptions;
uiOptions.selectable = true;
uiOptions.draggable = true;
uiOptions.clickable = true;
uiOptions.zOrder = 10;   // In front
uiOptions.onClickAction = onButtonClick;
uiOptions.onHoverAction = onButtonHover;

// Draggable game object
graphics::ShapeOptions gameOptions;
gameOptions.selectable = false;
gameOptions.draggable = true;
gameOptions.clickable = true;
gameOptions.onDragAction = onGameObjectMove;
```

## Shape Management Features

### Filtering and Querying

```cpp
// Get shapes by criteria
auto visibleShapes = shapeManager.getVisibleShapes();
auto selectableShapes = shapeManager.getSelectableShapes();
auto selectedShapes = shapeManager.getSelectedShapes();

// Find shapes at specific location
auto shapesAtPoint = shapeManager.getShapesAt(mouseX, mouseY);
auto topShape = shapeManager.getTopShapeAt(mouseX, mouseY);
```

### Selection Management

```cpp
// Select/deselect shapes
shapeManager.selectShape(myShape);
shapeManager.deselectShape(myShape);
shapeManager.selectAll();
shapeManager.deselectAll();
```

### Z-Order Management

```cpp
// Control layering
shapeManager.bringToFront(importantShape);
shapeManager.sendToBack(backgroundShape);
shapeManager.moveUp(shape);
shapeManager.moveDown(shape);
```

## Event Types and Data

### MouseEventType Enum

```cpp
enum class MouseEventType {
    CLICK,           // Single click
    DOUBLE_CLICK,    // Double click
    DRAG_START,      // Drag begins
    DRAG,            // During drag
    DRAG_END,        // Drag ends
    HOVER,           // Mouse enters shape
    LEAVE            // Mouse leaves shape
};
```

### MouseEventData Structure

```cpp
struct MouseEventData {
    double x, y;              // Current mouse position
    double deltaX, deltaY;    // Movement delta (for drag)
    Uint8 button;            // Mouse button (SDL constants)
    bool isPressed;          // Button state
    MouseEventType type;     // Event type
};
```

## Advanced Usage Patterns

### Dynamic Action Assignment

```cpp
// Change behavior at runtime
if (gameState == EDIT_MODE) {
    shape->setDragAction(editModeDrag);
    shape->setClickAction(editModeClick);
} else {
    shape->setDragAction(nullptr);
    shape->setClickAction(playModeClick);
}
```

### Generic Shape Creation

```cpp
// Use template for custom types
template<typename ShapeType, typename... Args>
auto createCustomShape(Args&&... args) {
    return shapeManager.createShape<ShapeType>(std::forward<Args>(args)...);
}
```

### Hierarchical Management

```cpp
// Group related shapes with z-order
const int UI_LAYER = 100;
const int GAME_LAYER = 50;
const int BACKGROUND_LAYER = 0;

// Create UI elements
auto button = shapeManager.createCircle(x, y, r, color, uiOptions);
button->setZOrder(UI_LAYER + 1);

// Create game objects
auto player = shapeManager.createTriangle(x1, y1, x2, y2, x3, y3, color, gameOptions);
player->setZOrder(GAME_LAYER + 1);
```

## Classes vs Structs Recommendation

**Use Classes** (as implemented) because:
- Better encapsulation with private/protected members
- Virtual functions for polymorphism
- Inheritance hierarchy (Shape → Circle/Rectangle/Triangle)
- Complex behavior with methods and state management
- Runtime action assignment requires function pointers/callbacks

**Structs** would be suitable only for simple data containers without behavior.

## Performance Considerations

- Shapes are managed using `std::shared_ptr` for automatic memory management
- Z-order sorting occurs when needed, not every frame
- Event handling uses efficient hit-testing (contains() method)
- Drawing skips invisible shapes automatically

## Building and Running

The system integrates with your existing CMake build:

```bash
# Build with shape demo
cmake -B build -DBUILD_SHAPE_DEMO=ON
cmake --build build
./build/Debug/shape_demo  # or shape_demo.exe on Windows
```

This shape system provides a solid foundation for complex interactive graphics applications while maintaining clean separation of concerns and extensibility.
