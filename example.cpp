#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>

#include <SFML/Graphics.hpp>
#include "gl_video.cpp"

typedef long long ll;
using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "Example View");
    video_context vc (500,500);
    sf::CircleShape circle(150); circle.setPosition(100, 200);

    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close(); 
            if (event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Enter) {
                    auto start = chrono::system_clock::now();
                    vc.screenshot("screenshot.png");
                    auto end = chrono::system_clock::now();

                    cout << chrono::duration_cast<chrono::milliseconds>(end-start).count() << "ms per screenshot" << endl; 
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(circle);
        window.display();
    }

    return 0;
}

