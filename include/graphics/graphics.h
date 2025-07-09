#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_events.h>
#include <cmath>
#include <functional>
#include <vector>
#include <memory>
#include <string>

#ifdef _WIN32
    #ifdef GRAPHICS_BUILDING_DLL
        #define GRAPHICS_API __declspec(dllexport)
    #elif defined(GRAPHICS_USING_DLL)
        #define GRAPHICS_API __declspec(dllimport)
    #else
        #define GRAPHICS_API
    #endif
#else
    #define GRAPHICS_API
#endif

#define RAY_COUNT 180
#define PLANET_COUNT 2

namespace graphics {
    GRAPHICS_API uint32_t generateRandomUint32Color();
    struct Ray {
        // angle in radians
        double x, y, a;
    };
    // Forward declarations
    class Shape;
    class ShapeManager;
    class EventHandler;

    // Event types
    enum class MouseEventType {
        CLICK,
        DOUBLE_CLICK,
        DRAG_START,
        DRAG,
        DRAG_END,
        HOVER,
        LEAVE
    };

    // Mouse event data
    struct MouseEventData {
        double x, y;
        double deltaX, deltaY;
        Uint8 button;
        bool isPressed;
        MouseEventType type;
    };

    // Action callback type
    using ActionCallback = std::function<void(Shape*, const MouseEventData&)>;

    // Shape creation options
    struct ShapeOptions {
        bool selectable = true;
        bool draggable = false;
        bool clickable = true;
        bool visible = true;
        int zOrder = 0;
        ActionCallback onClickAction = nullptr;
        ActionCallback onDoubleClickAction = nullptr;
        ActionCallback onDragAction = nullptr;
        ActionCallback onHoverAction = nullptr;
    };

    // Abstract base class for all shapes
    class GRAPHICS_API Shape {
    public:
        Shape(double x, double y, Uint32 color, const ShapeOptions& options = ShapeOptions{});
        virtual ~Shape() = default;

        // Pure virtual methods
        virtual void draw(SDL_Surface* surface) = 0;
        virtual bool contains(double x, double y) const = 0;
        virtual Shape* clone() const = 0;
        virtual std::string getType() const = 0;

        // Virtual drawShape method that calls draw() - can be overridden for specific behavior
        virtual void drawShape(SDL_Surface* surface);

        // Common properties and methods
        virtual void setPosition(double x, double y);
        virtual void move(double deltaX, double deltaY);
        
        // Getters
        double getX() const { return x_; }
        double getY() const { return y_; }
        Uint32 getColor() const { return color_; }
        bool isSelected() const { return isSelected_; }
        bool isVisible() const { return visible_; }
        bool isSelectable() const { return selectable_; }
        bool isDraggable() const { return draggable_; }
        bool isClickable() const { return clickable_; }
        bool isDragging() const { return isDragging_; }
        int getZOrder() const { return zOrder_; }
        
        // Setters
        void setColor(Uint32 color) { color_ = color; }
        void setColorHighlight(Uint32 color);
        void setSelected(bool selected) { isSelected_ = selected; }
        void setVisible(bool visible) { visible_ = visible; }
        void setSelectable(bool selectable) { selectable_ = selectable; }
        void setDraggable(bool draggable) { draggable_ = draggable; }
        void setClickable(bool clickable) { clickable_ = clickable; }
        void setZOrder(int zOrder) { zOrder_ = zOrder; }

        // Event action setters
        void setClickAction(ActionCallback action) { onClickAction_ = action; }
        void setDoubleClickAction(ActionCallback action) { onDoubleClickAction_ = action; }
        void setDragAction(ActionCallback action) { onDragAction_ = action; }
        void setHoverAction(ActionCallback action) { onHoverAction_ = action; }

        // Event handlers (called by EventHandler)
        virtual void onClick(const MouseEventData& eventData);
        virtual void onDoubleClick(const MouseEventData& eventData);
        virtual void onDragStart(const MouseEventData& eventData);
        virtual void onDrag(const MouseEventData& eventData);
        virtual void onDragEnd(const MouseEventData& eventData);
        virtual void onHover(const MouseEventData& eventData);
        virtual void onLeave(const MouseEventData& eventData);

    protected:
        double x_, y_;
        Uint32 color_;
        Uint32 colorHighlight_;
        bool isSelected_;
        bool visible_;
        bool selectable_;
        bool draggable_;
        bool clickable_;
        int zOrder_;
        bool isDragging_;

        // Action callbacks
        ActionCallback onClickAction_;
        ActionCallback onDoubleClickAction_;
        ActionCallback onDragAction_;
        ActionCallback onHoverAction_;
    };

    // Circle class
    class GRAPHICS_API Circle : public Shape {
    public:
        Circle(double x, double y, double radius, Uint32 color, const ShapeOptions& options = ShapeOptions{});
        
        void draw(SDL_Surface* surface) override;
        void drawShape(SDL_Surface* surface) override;
        bool contains(double x, double y) const override;
        Shape* clone() const override;
        std::string getType() const override { return "Circle"; }
        
        double getRadius() const { return radius_; }
        void setRadius(double radius) { radius_ = radius; }

    private:
        double radius_;
    };

    // Rectangle class
    class GRAPHICS_API Rectangle : public Shape {
    public:
        Rectangle(double x, double y, double width, double height, Uint32 color, const ShapeOptions& options = ShapeOptions{});
        
        void draw(SDL_Surface* surface) override;
        void drawShape(SDL_Surface* surface) override;
        bool contains(double x, double y) const override;
        Shape* clone() const override;
        std::string getType() const override { return "Rectangle"; }
        
        double getWidth() const { return width_; }
        double getHeight() const { return height_; }
        void setWidth(double width) { width_ = width; }
        void setHeight(double height) { height_ = height; }

    private:
        double width_, height_;
    };

    // Triangle class
    class GRAPHICS_API Triangle : public Shape {
    public:
        Triangle(double x1, double y1, double x2, double y2, double x3, double y3, Uint32 color, const ShapeOptions& options = ShapeOptions{});
        
        void draw(SDL_Surface* surface) override;
        void drawShape(SDL_Surface* surface) override;
        bool contains(double x, double y) const override;
        Shape* clone() const override;
        std::string getType() const override { return "Triangle"; }
        
        void setPosition(double x, double y) override;
        void move(double deltaX, double deltaY) override;

        // Triangle-specific getters
        void getVertices(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3) const;

    private:
        double x1_, y1_, x2_, y2_, x3_, y3_;
        void updateCentroid();
    };

    // Event handler class
    class GRAPHICS_API EventHandler {
    public:
        EventHandler(ShapeManager* shapeManager);
        
        void handleEvent(const SDL_Event& event);
        void update(); // Call this each frame to handle ongoing events

    private:
        ShapeManager* shapeManager_;
        Shape* draggedShape_;
        Shape* hoveredShape_;
        Uint32 lastClickTime_;
        double lastClickX_, lastClickY_;
        double dragStartX_, dragStartY_;
        
        static const Uint32 DOUBLE_CLICK_TIME = 300; // milliseconds
        
        void handleMouseButtonDown(const SDL_Event& event);
        void handleMouseButtonUp(const SDL_Event& event);
        void handleMouseMotion(const SDL_Event& event);
        Shape* getShapeAt(double x, double y);
    };

    // Shape manager class
    class GRAPHICS_API ShapeManager {
    public:
        ShapeManager();
        ~ShapeManager() = default;

        // Shape creation factory methods
        std::shared_ptr<Circle> createCircle(double x, double y, double radius, Uint32 color, const ShapeOptions& options = ShapeOptions{});
        std::shared_ptr<Rectangle> createRectangle(double x, double y, double width, double height, Uint32 color, const ShapeOptions& options = ShapeOptions{});
        std::shared_ptr<Triangle> createTriangle(double x1, double y1, double x2, double y2, double x3, double y3, Uint32 color, const ShapeOptions& options = ShapeOptions{});

        // Generic shape creation
        template<typename T, typename... Args>
        std::shared_ptr<T> createShape(Args&&... args) {
            auto shape = std::make_shared<T>(std::forward<Args>(args)...);
            addShape(shape);
            return shape;
        }

        // Shape management
        void addShape(std::shared_ptr<Shape> shape);
        void removeShape(std::shared_ptr<Shape> shape);
        void removeShapeAt(size_t index);
        void clear();

        // Filtering and querying
        std::vector<std::shared_ptr<Shape>> getVisibleShapes() const;
        std::vector<std::shared_ptr<Shape>> getSelectableShapes() const;
        std::vector<std::shared_ptr<Shape>> getShapesAt(double x, double y) const;
        std::shared_ptr<Shape> getTopShapeAt(double x, double y) const;
        std::vector<std::shared_ptr<Shape>> getSelectedShapes() const;

        // Selection management
        void selectShape(std::shared_ptr<Shape> shape);
        void deselectShape(std::shared_ptr<Shape> shape);
        void deselectAll();
        void selectAll();

        // Rendering
        void drawAll(SDL_Surface* surface);

        // Access
        const std::vector<std::shared_ptr<Shape>>& getShapes() const { return shapes_; }
        size_t getShapeCount() const { return shapes_.size(); }

        // Z-order management
        void bringToFront(std::shared_ptr<Shape> shape);
        void sendToBack(std::shared_ptr<Shape> shape);
        void moveUp(std::shared_ptr<Shape> shape);
        void moveDown(std::shared_ptr<Shape> shape);

    private:
        std::vector<std::shared_ptr<Shape>> shapes_;
        void sortByZOrder();
    };

    // Utility functions for ray casting (for compatibility with existing code)
    GRAPHICS_API void generateRays(Circle sun, struct Ray rays[RAY_COUNT]);
    GRAPHICS_API void drawRays(SDL_Surface* surface, Circle sun, struct Ray rays[RAY_COUNT], Uint32 color, Circle planets[PLANET_COUNT]);

} // namespace graphics