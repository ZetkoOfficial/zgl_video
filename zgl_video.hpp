#ifndef ZETKOOFFICIAL_GL_VIDEO
#define ZETKOOFFICIAL_GL_VIDEO

#include <string>
#include <stdexcept>
#include <png.h>

#include <SFML/OpenGL.hpp>
#include <boost/process.hpp>

namespace zgl_video {

// helper struct for managing pixels
struct pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

/* 
    This function is a simple implementation of creating a png file using libpng.
*/
void write_png(std::string filename, int width, int height, pixel* buffer){
    
    // initialize required resources
    FILE *fp = fopen(filename.c_str(), "wb");
    if(!fp) throw std::runtime_error("Error opening file for writing.");

    png_struct* png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr) {
        fclose(fp);
        throw std::runtime_error("Error creating png write struct.");
    }

    png_info* info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL); fclose(fp);
        throw std::runtime_error("Error creating png info struct.");
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
            row_pointers[j][3*i+0] = buffer[j*width+i].r;
            row_pointers[j][3*i+1] = buffer[j*width+i].g;
            row_pointers[j][3*i+2] = buffer[j*width+i].b;
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
    private: 

    pixel *buffer, *buffer2; int width, height;
    boost::process::opstream ffmpeg_in; boost::process::child ffmpeg_child;

    // Populates the current local buffer, with the pixel buffer of the current opengl context.
    void populate_buffer() { 
        glReadPixels(0,0, width, height, GL_RGB, GL_UNSIGNED_BYTE, this->buffer2);

        // reorder y-axis correctly
        for(int j = 0; j < height; j++){
            for(int i = 0; i < width; i++) this->buffer[(height-j-1)*width+i] = this->buffer2[j*width+i];
        }
    }

    public:
    bool is_recording = false;
    
    video_context(int width, int height){
        this->buffer  = new pixel[width*height];
        this->buffer2 = new pixel[width*height];
        this->width = width; this->height = height;
    }
    ~video_context() { 
        // clean up pixel buffers
        delete[] this->buffer;
        delete[] this->buffer2;

        // stop recording if not stopped
        if(this->is_recording) stop_recording();
    }
    
    // Creates a screenshot of the pixel buffer of the current opengl context in bmp format.
    void screenshot(std::string filename) {
        populate_buffer();
        write_png(filename, width, height, buffer);
    }

    // initializes ffmpeg for recording video
    void init_recording(std::string filename, std::string ffmpeg_location="/usr/bin/ffmpeg", bool display_output = true) {
        this->is_recording = true;
        std::string size_str = std::to_string(this->width) + "x" + std::to_string(this->height);

        if(display_output){
            ffmpeg_child = boost::process::child(
                ffmpeg_location, 
                "-y", 
                "-f", "rawvideo", 
                "-pixel_format", "rgb24",
                "-video_size", size_str,
                "-i", "-",
                "-c:v", "libx264", filename,

                boost::process::std_in < this->ffmpeg_in
            );
        } else{
            ffmpeg_child = boost::process::child(
                ffmpeg_location, 
                "-y", 
                "-f", "rawvideo", 
                "-pixel_format", "rgb24",
                "-video_size", size_str,
                "-i", "-",
                "-c:v", "libx264", filename,

                boost::process::std_in < this->ffmpeg_in,
                boost::process::std_out > boost::process::null
            );
        }
        
    }

    // stops ffmpeg and finalizes the recording
    void stop_recording() {
        this->is_recording = false;

        ffmpeg_in.close();
        kill(ffmpeg_child.id(), SIGTERM);
        ffmpeg_child.wait();
    }

    // Adds the pixel buffer of the current opengl context as a frame to the rendering video.
    void add_frame() {
        if(!is_recording) throw std::runtime_error("Recording is not initialized.");

        populate_buffer();
        this->ffmpeg_in.write((char*) this->buffer, sizeof(struct pixel) * width * height);
    }
};

}
#endif