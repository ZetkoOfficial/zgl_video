#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include <SFML/OpenGL.hpp>

typedef long long ll;
using namespace std;

void get_pixels(int width, int height) {  
    uint8_t* data = new uint8_t[width*height*3];
    glReadPixels(0,0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    int p = 0;
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            // here we process the image data
            p += 3;
        }
    }

    delete[] data;
}