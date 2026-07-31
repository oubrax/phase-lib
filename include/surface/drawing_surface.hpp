#pragma once

/// Every backend providing a drawing surface must implement this abstract class.
class DrawingSurface {
public:
    int width = 0, height = 0;

    virtual void destroy() = 0;
    virtual void swap_buffers() = 0;
    virtual void poll_events() = 0;
    virtual bool should_close() = 0;
    virtual void make_current() = 0;

    virtual ~DrawingSurface() = default;
};
