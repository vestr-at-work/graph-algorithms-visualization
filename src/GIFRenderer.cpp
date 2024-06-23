
#include "GIFRenderer.hpp"

GIFRenderer::GIFRenderer(const std::string& file_path, int frame_delay, 
    std::size_t width, std::size_t height) 
    : m_frame_width(width), m_frame_height(height),
    m_writer(file_path, width, height, frame_delay),
    m_next_frame(width, height) {}

Frame& GIFRenderer::beginDrawing() {
    return m_next_frame;
}

void GIFRenderer::endDrawing() {
    m_writer.write_frame(m_next_frame.getFrame());
    m_next_frame = GIFFrame(m_frame_width, m_frame_height);
}