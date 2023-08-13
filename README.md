# ZetkoOfficial OpenGL screenshot/video recording library (zgl_video)
This library allows the user to take screenshots and record videos of their SFML's OpenGL context's pixel buffer.
It uses [libpng](http://www.libpng.org/pub/png/libpng.html) for creating .png screenshots and [FFMPEG](https://www.ffmpeg.org/) for creating .mp4 videos, which makes these two libraries dependencies for this library.

## Compiling and example
When compiling a program with this library included, compile flags `-lGL -lpng -lpthread` are required. An installation of `ffmpeg` is also required and its location can be passed into the `video_context::init_recording` method(the default is `/usr/bin/ffmpeg`). The library also uses parts of `boost::process`, which is therefore also required. 

An example of the usage of this libaray can be seen in [example.cpp](./example.cpp), which uses the library [SFML](https://www.sfml-dev.org/) and can be compiled with `make test`.

## Usage
To take screenshots a `video_context` object should be created once, with the correct dimension parameters. Then a screenshot can be simply made at any time using `video_context::screenshot`.
```cpp
    ...
    #include "zgl_video.hpp"
    using namespace zgl_video;

    int main() {
        video_context context (window_width, window_height);
        
        ... // creating window, drawing, etc...

        context.screenshot("screenshot.png") // making a screenshot 
    }
``` 
To record a video instead, the method `video_context::init_recording` should be called on the context after creating it. Then frames can be added to the video by calling `video_context::add_frame` and the recording can be stopped by calling `video_context::stop_recording`.
```cpp
    ...
    #include "zgl_video.hpp"
    using namespace zgl_video;

    int main() {
        video_context context (window_width, window_height);
        context.init_recording("output.mp4");
        
        ... // creating window, etc...

        draw loop {
            ... // here a frame is drawn in OPENGL ...

            context.add_frame();
        }
    }
``` 


