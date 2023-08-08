#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include <SFML/OpenGL.hpp>

typedef long long ll;
using namespace std;

struct video_context {
    uint8_t* buffer; int width, height;
    
    video_context(int width, int height){
        this->buffer = new uint8_t[width*height*3];
        this->width = width; this->height = height;
    }
    ~video_context(){ delete[] this->buffer; }
    
    // Populates the current local buffer, with the pixel buffer of the current opengl context.
    void populate_buffer() { glReadPixels(0,0, width, height, GL_RGB, GL_UNSIGNED_BYTE, this->buffer); }
    
    // Creates a screenshot of the pixel buffer of the current opengl context in bmp format.
    void screenshot(string filename) {
        populate_buffer();

        int p = 0;
        for(int j = 0; j < height; j++){
            for(int i = 0; i < width; i++){
                /*  TODO: The pixel (i,height-j-1) should be set here to color
                    RGB(this->buffer[p],this->buffer[p+1],this->buffer[p+2]) */ 
                p += 3;
            }
        }
    }

    // Adds the pixel buffer of the current opengl context as a frame to the rendering video.
    void add_frame() {
        populate_buffer();
        // TODO: here we handle the addition of the current buffer to the video
    }
};