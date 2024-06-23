//
// gif.hpp
// by Charlie Tangora
// Public domain.
// Email me : ctangora -at- gmail -dot- com
//
// This file offers a simple, very limited way to create animated GIFs directly
// in code.
//
// Those looking for particular cleverness are likely to be disappointed; it's
// pretty much a straight-ahead implementation of the GIF format with optional
// Floyd-Steinberg dithering. (It does at least use delta encoding - only the
// changed portions of each frame are saved.)
//
// So resulting files are often quite large. The hope is that it will be handy
// nonetheless as a quick and easily-integrated way for programs to spit out
// animations.
//
// Only RGBA8 is currently supported as an input format. (The alpha is ignored.)
//
// If capturing a buffer with a bottom-left origin (such as OpenGL), define
// GIF_FLIP_VERT to automatically flip the buffer data when writing the image
// (the buffer itself is unchanged.
//
// USAGE:
// Create a GifWriter struct. Pass it to GifBegin() to initialize and write the
// header. Pass subsequent frames to GifWriteFrame(). Finally, call GifEnd() to
// close the file handle and free memory.
//

#ifndef gif_hpp
#define gif_hpp

#include <stdbool.h> // for bool macros
#include <stdint.h>  // for integer typedefs
#include <stdio.h>   // for FILE*
#include <string>
#include <vector>

// Define these macros to hook into a custom memory allocator.
// TEMP_MALLOC and TEMP_FREE will only be called in stack fashion - frees in the
// reverse order of mallocs and any temp memory allocated by a function will be
// freed before it exits. MALLOC and FREE are used only by GifBegin and GifEnd
// respectively (to allocate a buffer the size of the image, which is used to
// find changed pixels for delta-encoding.)

#ifndef GIF_TEMP_MALLOC
#include <stdlib.h>
#define GIF_TEMP_MALLOC malloc
#endif

#ifndef GIF_TEMP_FREE
#include <stdlib.h>
#define GIF_TEMP_FREE free
#endif

#ifndef GIF_MALLOC
#include <stdlib.h>
#define GIF_MALLOC malloc
#endif

#ifndef GIF_FREE
#include <stdlib.h>
#define GIF_FREE free
#endif

namespace gif {
namespace impl {

// Constants
const int kGifTransIndex = 0;

// Struct definitions
typedef struct {
    int bitDepth;

    uint8_t r[256];
    uint8_t g[256];
    uint8_t b[256];

    // k-d tree over RGB space, organized in heap fashion
    // i.e. left child of node i is node i*2, right child is node i*2+1
    // nodes 256-511 are implicitly the leaves, containing a color
    uint8_t treeSplitElt[256];
    uint8_t treeSplit[256];
} GifPalette;

// Struct to hold the status of bit-writing
typedef struct {
    uint8_t bitIndex; // how many bits in the partial byte written so far
    uint8_t byte;     // current partial byte

    uint32_t chunkIndex;
    uint8_t chunk[256]; // bytes are written in here until we have 256 of them,
                        // then written to the file
} GifBitStatus;

// The LZW dictionary is a 256-ary tree constructed as the file is encoded,
// this is one node
typedef struct {
    uint16_t m_next[256];
} GifLzwNode;

// Structure to handle GIF writing
typedef struct {
    FILE* f;
    uint8_t* oldImage;
    bool firstFrame;
} GifWriter;

// Function declarations

// max, min, and abs functions
int GifIMax(int l, int r);
int GifIMin(int l, int r);
int GifIAbs(int i);

// Walks the k-d tree to pick the palette entry for a desired color.
// Takes as in/out parameters the current best color and its error -
// only changes them if it finds a better color in its subtree.
void GifGetClosestPaletteColor(GifPalette* pPal, int r, int g, int b,
                               int* bestInd, int* bestDiff, int treeRoot);

// Swaps pixels at two specified indices in the image array
void GifSwapPixels(uint8_t* image, int pixA, int pixB);

// Just the partition operation from quicksort
int GifPartition(uint8_t* image, const int left, const int right, const int elt,
                 int pivotIndex);

// Perform an incomplete sort, finding all elements above and below the desired
// median
void GifPartitionByMedian(uint8_t* image, int left, int right, int com,
                          int neededCenter);

// Builds a palette by creating a balanced k-d tree of all pixels in the image
void GifSplitPalette(uint8_t* image, int numPixels, int firstElt, int lastElt,
                     int splitElt, int splitDist, int treeNode,
                     bool buildForDither, GifPalette* pal);

// Finds all pixels that have changed from the previous image and
// moves them to the front of the buffer.
int GifPickChangedPixels(const uint8_t* lastFrame, uint8_t* frame,
                         int numPixels);

// Creates a palette by placing all the image pixels in a k-d tree and then
// averaging the blocks at the bottom. This is known as the "modified median
// split" technique.
void GifMakePalette(const uint8_t* lastFrame, const uint8_t* nextFrame,
                    uint32_t width, uint32_t height, int bitDepth,
                    bool buildForDither, GifPalette* pPal);

// Implements Floyd-Steinberg dithering, writes palette value to alpha
void GifDitherImage(const uint8_t* lastFrame, const uint8_t* nextFrame,
                    uint8_t* outFrame, uint32_t width, uint32_t height,
                    GifPalette* pPal);

// Picks palette colors for the image using simple thresholding, no dithering
void GifThresholdImage(const uint8_t* lastFrame, const uint8_t* nextFrame,
                       uint8_t* outFrame, uint32_t width, uint32_t height,
                       GifPalette* pPal);

// insert a single bit
void GifWriteBit(GifBitStatus* stat, uint32_t bit);

// write all bytes so far to the file
void GifWriteChunk(FILE* f, GifBitStatus* stat);

// write a specific code using the specified bit length
void GifWriteCode(FILE* f, GifBitStatus* stat, uint32_t code, uint32_t length);

// write a 256-color (8-bit) image palette to the file
void GifWritePalette(const GifPalette* pPal, FILE* f);

// write the image header, LZW-compress and write out the image
void GifWriteLzwImage(FILE* f, uint8_t* image, uint32_t left, uint32_t top,
                      uint32_t width, uint32_t height, uint32_t delay,
                      GifPalette* pPal);

// Creates a gif file.
// The input GIFWriter is assumed to be uninitialized.
// The delay value is the time between frames in hundredths of a second.
bool GifBegin(GifWriter* writer, const char* filename, uint32_t width,
              uint32_t height, uint32_t delay, int32_t bitDepth = 8,
              bool dither = false);

// Writes out a new frame to a GIF in progress.
bool GifWriteFrame(GifWriter* writer, const uint8_t* image, uint32_t width,
                   uint32_t height, uint32_t delay, int bitDepth = 8,
                   bool dither = false);

// Writes the EOF code, closes the file handle, and frees temp memory used by a GIF.
bool GifEnd(GifWriter* writer);

} // namespace impl

// Struct representing a pixel with RGBA components
struct Pixel {
    uint8_t r, g, b, a; // red green blue alpha
    Pixel() : r{0}, g{0}, b{0}, a{0} {}
    Pixel(uint8_t r, uint8_t g, uint8_t b) : r{r}, g{g}, b{b}, a{0} {}
};

// Class representing a frame of the GIF
class Frame {
    std::size_t width;
    std::size_t height;
    std::vector<Pixel> data;

public:
    // Constructor initializing the frame
    Frame(std::size_t width, std::size_t height, Pixel default_color = Pixel{})
        : width{width}, height{height}, data{width * height, default_color} {}

    // Function to get the raw data of the frame
    const uint8_t* raw_data() const {
        return reinterpret_cast<const uint8_t*>(data.data());
    }

    // Operator to access pixel by coordinate
    Pixel& operator[](std::pair<std::size_t, std::size_t> coord) {
        auto [i, j] = coord;
        return data[i * width + j];
    }

    // Constant operator to access pixel by coordinate
    const Pixel& operator[](std::pair<std::size_t, std::size_t> coord) const {
        auto [i, j] = coord;
        return data[i * width + j];
    }
};

// Wrapper class for writing GIFs
class GifWriter final {
    impl::GifWriter g{};
    int width, height, delay;

public:
    // Constructor for initializing a GIF writer
    GifWriter(const std::string& file_name, int width, int height, int delay)
        : width{width}, height{height}, delay{delay} {
        impl::GifBegin(&g, file_name.c_str(), width, height, delay);
    }

    // Function to write a frame to the GIF
    void write_frame(const Frame& frame) {
        impl::GifWriteFrame(&g, frame.raw_data(), width, height, delay);
    }
    // Destructor for ending the GIF writer
    ~GifWriter() { impl::GifEnd(&g); }
};

} // namespace gif
#endif
