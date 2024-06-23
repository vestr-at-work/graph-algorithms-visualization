#ifndef GIFRenderer_hpp
#define GIFRenderer_hpp

#include "gif.hpp"
#include "Frame.hpp"
#include "GIFFrame.hpp"
#include "Renderer.hpp"
#include <cstddef>

#include <string> // For std::string

// Class for rendering GIF images, derived from the Renderer base class
class GIFRenderer : public Renderer {
    std::size_t m_frame_width;
    std::size_t m_frame_height;
    gif::GifWriter m_writer; // GIF writer for creating the GIF file
    GIFFrame m_next_frame; // The next frame to be drawn in the GIF

public:
    /**
     * Constructor for GIFRenderer.
     *
     * @param file_path The path to the file where the GIF will be saved.
     * @param frame_delay The delay between frames in hundredths of a second.
     * @param width The width of each frame in the GIF.
     * @param height The height of each frame in the GIF.
     */
    GIFRenderer(const std::string& file_path, int frame_delay,
        std::size_t width, std::size_t height);

    /**
     * Begins drawing a new frame in the GIF.
     *
     * @return A reference to the frame being drawn.
     */
    virtual Frame& beginDrawing() override;

    /**
     * Ends drawing the current frame in the GIF.
     */
    virtual void endDrawing() override;

    /**
     * Finalizes the GIF rendering process.
     * This method may contain any necessary cleanup operations before finishing the GIF file.
     */
    virtual void finalize() override {};
};


#endif