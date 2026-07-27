#pragma once
#include "application/window.hpp"

#include <string_view>

// This abstract class represents a Window
class DrawingSurface {
public:
    virtual void init() = 0;
    virtual void destroy() = 0;
    virtual void swap_buffers() = 0;
    virtual void poll_events() = 0;
    virtual bool should_close() = 0;
    virtual void make_current() = 0;

    virtual void create_surface(const WindowOptions& options) = 0;

    virtual ~DrawingSurface() = default;
};
