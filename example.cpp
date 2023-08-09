#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>

#include <SFML/Graphics.hpp>
#include "zgl_video.hpp"

typedef long long ll;
using namespace std;

int main() {

    int width = 1000, height = 800;

    sf::RenderWindow window(sf::VideoMode(width, height), "Example View");
    sf::CircleShape circle(150); circle.setPosition(100, 200);
    window.setFramerateLimit(30); 

    // creating the video_context and inititalizing the recording
    zgl_video::video_context vc (width,height); vc.init_recording("output.mp4");

    int i=0, j=0;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close(); 
            if (event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Enter) vc.screenshot("screenshot.png");  // taking a screenshot and saving it to screenshot.png
                if(event.key.code == sf::Keyboard::A) i--;
                if(event.key.code == sf::Keyboard::D) i++;
                if(event.key.code == sf::Keyboard::W) j--;
                if(event.key.code == sf::Keyboard::S) j++;
            }
        }
        circle.setPosition(100+10*i, 200+10*j);

        window.clear(sf::Color::Black);
        window.draw(circle);
        window.display();

        vc.add_frame(); // adding a frame to the recording
    }

    return 0;
}

