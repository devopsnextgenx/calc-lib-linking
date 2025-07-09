#include "graphics/graphics.h"
#include <algorithm>
#include <cmath>
#include <SDL3/SDL.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace graphics {

    uint32_t generateRandomUint32Color() {
        // RAND_MAX is typically at least 32767 (2^15 - 1).
        // To get a 32-bit number, we need to combine multiple rand() calls.
        // A common way is to shift and OR, ensuring all 32 bits are covered.
        // This approach assumes RAND_MAX is at least 32767.

        uint32_t r1 = rand();
        uint32_t r2 = rand();
        uint32_t r3 = rand();

        // Combine the random parts to form a 32-bit number.
        // The exact combination depends on RAND_MAX.
        // A simple way to get 32 bits if RAND_MAX is 0x7FFF (15 bits) is:
        // (r1 << 20) | (r2 << 5) | r3
        // However, a more robust way that covers cases where RAND_MAX is larger
        // or to ensure full 32-bit coverage is to use bitwise operations carefully.

        // A common and robust way to generate a 32-bit random number
        // by combining two 16-bit random numbers (assuming RAND_MAX >= 32767)
        // or three for better distribution if RAND_MAX is small.
        // For simplicity and assuming RAND_MAX is sufficient (or combining multiple calls):
        uint32_t random_value = ((uint32_t)rand() << 16) | (uint32_t)rand();

        // If RAND_MAX is less than 65535, you might need more calls:
        // uint32_t random_value = ((uint32_t)rand() << 24) | ((uint32_t)rand() << 12) | (uint32_t)rand();

        return random_value;
    }
    //=============================================================================
    // Shape Base Class Implementation
    //=============================================================================

    Shape::Shape(double x, double y, Uint32 color, const ShapeOptions& options)
        : x_(x), y_(y), color_(color), isSelected_(false), visible_(options.visible),
          selectable_(options.selectable), draggable_(options.draggable), 
          clickable_(options.clickable), zOrder_(options.zOrder), isDragging_(false),
          onClickAction_(options.onClickAction), onDoubleClickAction_(options.onDoubleClickAction),
          onDragAction_(options.onDragAction), onHoverAction_(options.onHoverAction) {
            // generate random highlight color if not set
            if (colorHighlight_ == 0) {
                // Generate a highlight color by randomizing the alpha channel
                // randomize even rgb values
                colorHighlight_ = generateRandomUint32Color();
            }
    }

    void Shape::setPosition(double x, double y) {
        x_ = x;
        y_ = y;
    }

    void Shape::move(double deltaX, double deltaY) {
        x_ += deltaX;
        y_ += deltaY;
    }

    void Shape::drawShape(SDL_Surface* surface) {
        // Default implementation calls draw()
        draw(surface);
    }

    void Shape::onClick(const MouseEventData& eventData) {
        if (clickable_ && onClickAction_) {
            onClickAction_(this, eventData);
        }
    }

    void Shape::onDoubleClick(const MouseEventData& eventData) {
        if (clickable_ && onDoubleClickAction_) {
            onDoubleClickAction_(this, eventData);
        }
    }

    void Shape::onDragStart(const MouseEventData& eventData) {
        if (draggable_) {
            isDragging_ = true;
        }
    }

    void Shape::onDrag(const MouseEventData& eventData) {
        if (draggable_ && isDragging_) {
            move(eventData.deltaX, eventData.deltaY);
            if (onDragAction_) {
                onDragAction_(this, eventData);
            }
        }
    }

    void Shape::onDragEnd(const MouseEventData& eventData) {
        isDragging_ = false;
    }

    void Shape::onHover(const MouseEventData& eventData) {
        if (onHoverAction_) {
            onHoverAction_(this, eventData);
        }
    }

    void Shape::onLeave(const MouseEventData& eventData) {
        // Default implementation - can be overridden
    }

    void Shape::setColorHighlight(Uint32 color) {
        colorHighlight_ = color;
    }

    //=============================================================================
    // Circle Class Implementation
    //=============================================================================

    Circle::Circle(double x, double y, double radius, Uint32 color, const ShapeOptions& options)
        : Shape(x, y, color, options), radius_(radius) {
    }

    void Circle::drawShape(SDL_Surface* surface) {
        // Use the draw method to render the circle
        draw(surface);
    }

    void Circle::draw(SDL_Surface* surface) {
        if (!visible_) return;

        // Draw filled circle using the same algorithm as legacy drawCircle
        double rsq = radius_ * radius_;
        double x_end = x_ + radius_;
        double y_end = y_ + radius_;
        
        for (double x = x_ - radius_; x <= x_end; x++) {
            for (double y = y_ - radius_; y <= y_end; y++) {
                if ((x - x_) * (x - x_) + (y - y_) * (y - y_) <= rsq) {
                    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
                        SDL_Rect pixel = {(int)x, (int)y, 1, 1};
                        Uint32 drawColor = isSelected_ ? (colorHighlight_) : color_; // Highlight selected
                        SDL_FillSurfaceRect(surface, &pixel, drawColor);
                    }
                }
            }
        }
    }

    bool Circle::contains(double x, double y) const {
        double dx = x - x_;
        double dy = y - y_;
        return (dx * dx + dy * dy) <= (radius_ * radius_);
    }

    Shape* Circle::clone() const {
        ShapeOptions options;
        options.selectable = selectable_;
        options.draggable = draggable_;
        options.clickable = clickable_;
        options.visible = visible_;
        options.zOrder = zOrder_;
        options.onClickAction = onClickAction_;
        options.onDoubleClickAction = onDoubleClickAction_;
        options.onDragAction = onDragAction_;
        options.onHoverAction = onHoverAction_;
        
        return new Circle(x_, y_, radius_, color_, options);
    }

    //=============================================================================
    // Rectangle Class Implementation
    //=============================================================================

    Rectangle::Rectangle(double x, double y, double width, double height, Uint32 color, const ShapeOptions& options)
        : Shape(x, y, color, options), width_(width), height_(height) {
    }

    void Rectangle::drawShape(SDL_Surface* surface) {
        // Use the draw method to render the rectangle
        draw(surface);
    }

    void Rectangle::draw(SDL_Surface* surface) {
        if (!visible_) return;

        SDL_Rect rect = {
            (int)(x_ - width_ / 2),
            (int)(y_ - height_ / 2),
            (int)width_,
            (int)height_
        };
        
        Uint32 drawColor = isSelected_ ? (color_ | 0xFF000000) : color_; // Highlight selected
        SDL_FillSurfaceRect(surface, &rect, drawColor);
    }

    bool Rectangle::contains(double x, double y) const {
        double left = x_ - width_ / 2;
        double right = x_ + width_ / 2;
        double top = y_ - height_ / 2;
        double bottom = y_ + height_ / 2;
        
        return x >= left && x <= right && y >= top && y <= bottom;
    }

    Shape* Rectangle::clone() const {
        ShapeOptions options;
        options.selectable = selectable_;
        options.draggable = draggable_;
        options.clickable = clickable_;
        options.visible = visible_;
        options.zOrder = zOrder_;
        options.onClickAction = onClickAction_;
        options.onDoubleClickAction = onDoubleClickAction_;
        options.onDragAction = onDragAction_;
        options.onHoverAction = onHoverAction_;
        
        return new Rectangle(x_, y_, width_, height_, color_, options);
    }

    //=============================================================================
    // Triangle Class Implementation
    //=============================================================================

    Triangle::Triangle(double x1, double y1, double x2, double y2, double x3, double y3, Uint32 color, const ShapeOptions& options)
        : Shape(0, 0, color, options), x1_(x1), y1_(y1), x2_(x2), y2_(y2), x3_(x3), y3_(y3) {
        updateCentroid();
    }

    void Triangle::drawShape(SDL_Surface* surface) {
        // Use the draw method to render the triangle
        draw(surface);
    }

    void Triangle::updateCentroid() {
        x_ = (x1_ + x2_ + x3_) / 3.0;
        y_ = (y1_ + y2_ + y3_) / 3.0;
    }

    void Triangle::setPosition(double x, double y) {
        double deltaX = x - x_;
        double deltaY = y - y_;
        move(deltaX, deltaY);
    }

    void Triangle::move(double deltaX, double deltaY) {
        x1_ += deltaX;
        y1_ += deltaY;
        x2_ += deltaX;
        y2_ += deltaY;
        x3_ += deltaX;
        y3_ += deltaY;
        updateCentroid();
    }

    void Triangle::getVertices(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3) const {
        x1 = x1_; y1 = y1_;
        x2 = x2_; y2 = y2_;
        x3 = x3_; y3 = y3_;
    }

    void Triangle::draw(SDL_Surface* surface) {
        if (!visible_) return;

        // Simple triangle drawing using barycentric coordinates
        int minX = (int)std::min({x1_, x2_, x3_});
        int maxX = (int)std::max({x1_, x2_, x3_});
        int minY = (int)std::min({y1_, y2_, y3_});
        int maxY = (int)std::max({y1_, y2_, y3_});

        // Clamp to surface bounds
        minX = std::max(0, minX);
        maxX = std::min(surface->w - 1, maxX);
        minY = std::max(0, minY);
        maxY = std::min(surface->h - 1, maxY);

        Uint32 drawColor = isSelected_ ? (color_ | 0xFF000000) : color_;

        for (int y = minY; y <= maxY; y++) {
            for (int x = minX; x <= maxX; x++) {
                if (contains(x, y)) {
                    SDL_Rect pixel = {x, y, 1, 1};
                    SDL_FillSurfaceRect(surface, &pixel, drawColor);
                }
            }
        }
    }

    bool Triangle::contains(double x, double y) const {
        // Use barycentric coordinates to check if point is inside triangle
        double denom = (y2_ - y3_) * (x1_ - x3_) + (x3_ - x2_) * (y1_ - y3_);
        if (std::abs(denom) < 1e-10) return false; // Degenerate triangle
        
        double a = ((y2_ - y3_) * (x - x3_) + (x3_ - x2_) * (y - y3_)) / denom;
        double b = ((y3_ - y1_) * (x - x3_) + (x1_ - x3_) * (y - y3_)) / denom;
        double c = 1 - a - b;
        
        return a >= 0 && b >= 0 && c >= 0;
    }

    Shape* Triangle::clone() const {
        ShapeOptions options;
        options.selectable = selectable_;
        options.draggable = draggable_;
        options.clickable = clickable_;
        options.visible = visible_;
        options.zOrder = zOrder_;
        options.onClickAction = onClickAction_;
        options.onDoubleClickAction = onDoubleClickAction_;
        options.onDragAction = onDragAction_;
        options.onHoverAction = onHoverAction_;
        
        return new Triangle(x1_, y1_, x2_, y2_, x3_, y3_, color_, options);
    }

    //=============================================================================
    // EventHandler Implementation
    //=============================================================================

    EventHandler::EventHandler(ShapeManager* shapeManager)
        : shapeManager_(shapeManager), draggedShape_(nullptr), hoveredShape_(nullptr),
          lastClickTime_(0), lastClickX_(0), lastClickY_(0), dragStartX_(0), dragStartY_(0) {
    }

    void EventHandler::handleEvent(const SDL_Event& event) {
        switch (event.type) {
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                handleMouseButtonDown(event);
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                handleMouseButtonUp(event);
                break;
            case SDL_EVENT_MOUSE_MOTION:
                handleMouseMotion(event);
                break;
        }
    }

    void EventHandler::handleMouseButtonDown(const SDL_Event& event) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            double x = event.button.x;
            double y = event.button.y;
            Shape* shape = getShapeAt(x, y);
            
            if (shape) {
                Uint32 currentTime = SDL_GetTicks();
                bool isDoubleClick = (currentTime - lastClickTime_ < DOUBLE_CLICK_TIME) &&
                                   (std::abs(x - lastClickX_) < 5) &&
                                   (std::abs(y - lastClickY_) < 5);

                MouseEventData eventData;
                eventData.x = x;
                eventData.y = y;
                eventData.deltaX = 0;
                eventData.deltaY = 0;
                eventData.button = event.button.button;
                eventData.isPressed = true;

                if (isDoubleClick) {
                    eventData.type = MouseEventType::DOUBLE_CLICK;
                    shape->onDoubleClick(eventData);
                } else {
                    eventData.type = MouseEventType::CLICK;
                    shape->onClick(eventData);
                    
                    if (shape->isDraggable()) {
                        draggedShape_ = shape;
                        dragStartX_ = x;
                        dragStartY_ = y;
                        eventData.type = MouseEventType::DRAG_START;
                        shape->onDragStart(eventData);
                    }
                }

                lastClickTime_ = currentTime;
                lastClickX_ = x;
                lastClickY_ = y;
            }
        }
    }

    void EventHandler::handleMouseButtonUp(const SDL_Event& event) {
        if (event.button.button == SDL_BUTTON_LEFT && draggedShape_) {
            MouseEventData eventData;
            eventData.x = event.button.x;
            eventData.y = event.button.y;
            eventData.deltaX = event.button.x - dragStartX_;
            eventData.deltaY = event.button.y - dragStartY_;
            eventData.button = event.button.button;
            eventData.isPressed = false;
            eventData.type = MouseEventType::DRAG_END;
            
            draggedShape_->onDragEnd(eventData);
            draggedShape_ = nullptr;
        }
    }

    void EventHandler::handleMouseMotion(const SDL_Event& event) {
        double x = event.motion.x;
        double y = event.motion.y;
        
        // Handle dragging
        if (draggedShape_) {
            MouseEventData eventData;
            eventData.x = x;
            eventData.y = y;
            eventData.deltaX = event.motion.xrel;
            eventData.deltaY = event.motion.yrel;
            eventData.button = 0;
            eventData.isPressed = true;
            eventData.type = MouseEventType::DRAG;
            
            draggedShape_->onDrag(eventData);
        }

        // Handle hover
        Shape* shape = getShapeAt(x, y);
        if (shape != hoveredShape_) {
            if (hoveredShape_) {
                MouseEventData leaveData;
                leaveData.x = x;
                leaveData.y = y;
                leaveData.type = MouseEventType::LEAVE;
                hoveredShape_->onLeave(leaveData);
            }
            
            hoveredShape_ = shape;
            
            if (hoveredShape_) {
                MouseEventData hoverData;
                hoverData.x = x;
                hoverData.y = y;
                hoverData.type = MouseEventType::HOVER;
                hoveredShape_->onHover(hoverData);
            }
        }
    }

    Shape* EventHandler::getShapeAt(double x, double y) {
        return shapeManager_->getTopShapeAt(x, y).get();
    }

    void EventHandler::update() {
        // This method can be called each frame for any time-based updates
        // Currently no time-based logic is needed
    }

    //=============================================================================
    // ShapeManager Implementation
    //=============================================================================

    ShapeManager::ShapeManager() {
    }

    std::shared_ptr<Circle> ShapeManager::createCircle(double x, double y, double radius, Uint32 color, const ShapeOptions& options) {
        auto circle = std::make_shared<Circle>(x, y, radius, color, options);
        addShape(circle);
        return circle;
    }

    std::shared_ptr<Rectangle> ShapeManager::createRectangle(double x, double y, double width, double height, Uint32 color, const ShapeOptions& options) {
        auto rectangle = std::make_shared<Rectangle>(x, y, width, height, color, options);
        addShape(rectangle);
        return rectangle;
    }

    std::shared_ptr<Triangle> ShapeManager::createTriangle(double x1, double y1, double x2, double y2, double x3, double y3, Uint32 color, const ShapeOptions& options) {
        auto triangle = std::make_shared<Triangle>(x1, y1, x2, y2, x3, y3, color, options);
        addShape(triangle);
        return triangle;
    }

    void ShapeManager::addShape(std::shared_ptr<Shape> shape) {
        shapes_.push_back(shape);
        sortByZOrder();
    }

    void ShapeManager::removeShape(std::shared_ptr<Shape> shape) {
        shapes_.erase(std::remove(shapes_.begin(), shapes_.end(), shape), shapes_.end());
    }

    void ShapeManager::removeShapeAt(size_t index) {
        if (index < shapes_.size()) {
            shapes_.erase(shapes_.begin() + index);
        }
    }

    void ShapeManager::clear() {
        shapes_.clear();
    }

    std::vector<std::shared_ptr<Shape>> ShapeManager::getVisibleShapes() const {
        std::vector<std::shared_ptr<Shape>> visibleShapes;
        for (const auto& shape : shapes_) {
            if (shape->isVisible()) {
                visibleShapes.push_back(shape);
            }
        }
        return visibleShapes;
    }

    std::vector<std::shared_ptr<Shape>> ShapeManager::getSelectableShapes() const {
        std::vector<std::shared_ptr<Shape>> selectableShapes;
        for (const auto& shape : shapes_) {
            if (shape->isSelectable()) {
                selectableShapes.push_back(shape);
            }
        }
        return selectableShapes;
    }

    std::vector<std::shared_ptr<Shape>> ShapeManager::getShapesAt(double x, double y) const {
        std::vector<std::shared_ptr<Shape>> shapesAt;
        for (const auto& shape : shapes_) {
            if (shape->isVisible() && shape->contains(x, y)) {
                shapesAt.push_back(shape);
            }
        }
        return shapesAt;
    }

    std::shared_ptr<Shape> ShapeManager::getTopShapeAt(double x, double y) const {
        // Since shapes are sorted by z-order (highest first), return the first match
        for (const auto& shape : shapes_) {
            if (shape->isVisible() && shape->contains(x, y)) {
                return shape;
            }
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<Shape>> ShapeManager::getSelectedShapes() const {
        std::vector<std::shared_ptr<Shape>> selectedShapes;
        for (const auto& shape : shapes_) {
            if (shape->isSelected()) {
                selectedShapes.push_back(shape);
            }
        }
        return selectedShapes;
    }

    void ShapeManager::selectShape(std::shared_ptr<Shape> shape) {
        if (shape && shape->isSelectable()) {
            shape->setSelected(true);
        }
    }

    void ShapeManager::deselectShape(std::shared_ptr<Shape> shape) {
        if (shape) {
            shape->setSelected(false);
        }
    }

    void ShapeManager::deselectAll() {
        for (auto& shape : shapes_) {
            shape->setSelected(false);
        }
    }

    void ShapeManager::selectAll() {
        for (auto& shape : shapes_) {
            if (shape->isSelectable()) {
                shape->setSelected(true);
            }
        }
    }

    void ShapeManager::drawAll(SDL_Surface* surface) {
        for (const auto& shape : shapes_) {
            shape->draw(surface);
        }
    }

    void ShapeManager::bringToFront(std::shared_ptr<Shape> shape) {
        if (!shape) return;
        
        int maxZ = 0;
        for (const auto& s : shapes_) {
            if (s != shape) {
                maxZ = std::max(maxZ, s->getZOrder());
            }
        }
        shape->setZOrder(maxZ + 1);
        sortByZOrder();
    }

    void ShapeManager::sendToBack(std::shared_ptr<Shape> shape) {
        if (!shape) return;
        
        int minZ = 0;
        for (const auto& s : shapes_) {
            if (s != shape) {
                minZ = std::min(minZ, s->getZOrder());
            }
        }
        shape->setZOrder(minZ - 1);
        sortByZOrder();
    }

    void ShapeManager::moveUp(std::shared_ptr<Shape> shape) {
        if (!shape) return;
        shape->setZOrder(shape->getZOrder() + 1);
        sortByZOrder();
    }

    void ShapeManager::moveDown(std::shared_ptr<Shape> shape) {
        if (!shape) return;
        shape->setZOrder(shape->getZOrder() - 1);
        sortByZOrder();
    }

    void ShapeManager::sortByZOrder() {
        std::sort(shapes_.begin(), shapes_.end(), 
                  [](const std::shared_ptr<Shape>& a, const std::shared_ptr<Shape>& b) {
                      return a->getZOrder() > b->getZOrder(); // Higher Z-order first
                  });
    }

} // namespace graphics
