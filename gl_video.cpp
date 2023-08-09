#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <png.h>

#include <SFML/OpenGL.hpp>

typedef long long ll;
using namespace std;

// helper struct for managing pixels
struct pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

/* 
    This function is a simple implementation of creating a png file using libpng.
*/
void write_png(string filename, int width, int height, pixel* buffer){
    
    // initialize required resources
    FILE *fp = fopen(filename.c_str(), "wb");
    if(!fp) throw runtime_error("Error opening file for writing.");

    png_struct* png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr) {
        fclose(fp);
        throw logic_error("Error creating png write struct.");
    }

    png_info* info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL); fclose(fp);
        throw logic_error("Error creating png info struct.");
    }

    // set and write png info 
    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);

    // create the row pointers required by png_write_image
    png_byte** row_pointers = new png_byte*[height];
    for(int j = 0; j < height; j++){
        row_pointers[j] = new png_byte[3*width];
        for(int i = 0; i < width; i++){
            row_pointers[j][3*i+0] = buffer[(height-j-1)*width+i].r;
            row_pointers[j][3*i+1] = buffer[(height-j-1)*width+i].g;
            row_pointers[j][3*i+2] = buffer[(height-j-1)*width+i].b;
        }
    }
    png_write_image(png_ptr, row_pointers);
    
    // cleanup resources 
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    for(int j = 0; j < height; j++) delete[] row_pointers[j];
    delete[] row_pointers;
    fclose(fp);
}

struct video_context {
    pixel* buffer; int width, height;
    
    video_context(int width, int height){
        this->buffer = new pixel[width*height];
        this->width = width; this->height = height;
    }
    ~video_context(){ delete[] this->buffer; }
    
    // Populates the current local buffer, with the pixel buffer of the current opengl context.
    void populate_buffer() { glReadPixels(0,0, width, height, GL_RGB, GL_UNSIGNED_BYTE, this->buffer); }
    
    // Creates a screenshot of the pixel buffer of the current opengl context in bmp format.
    void screenshot(string filename) {
        populate_buffer();
        write_png(filename, width, height, buffer);
    }

    // Adds the pixel buffer of the current opengl context as a frame to the rendering video.
    void add_frame() {
        populate_buffer();
        // TODO: here we handle the addition of the current buffer to the video
    }
};