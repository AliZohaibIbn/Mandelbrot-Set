#include <SFML/Graphics.hpp>
#include <cmath>
#include <complex>

const int Width = 800;
const int Height = 600;
const int maxIterations = 100;

int calculateMandelbrot(const std::complex<double>& c) {

    std::complex<double> z = 0;
    int iterations = 0;

    while (std::abs(z) <= 2.0 && iterations < maxIterations) {
        z = z * z + c;
        ++iterations;
    }

    return iterations;

}

std::complex<double> screenToComplex(int x, int y, double zoom, double offsetX, double offsetY) {
    double real = (x - Width / 2.0) * zoom + offsetX;
    double imag = (y - Height / 2.0) * zoom + offsetY;
    return std::complex<double>(real, imag);
}

int main() {
    
    sf::RenderWindow window(sf::VideoMode(Width, Height), "Mandelbrot Set");
    window.setFramerateLimit(60);

    // Mandelbrot parameters
    double zoom = 0.005;
    double offsetX = -0.5;
    double offsetY = 0.0;

    sf::Image mandelbrotImage;
    mandelbrotImage.create(Width, Height, sf::Color::Black);

    sf::Texture texture;
    sf::Sprite sprite;

    bool needsRedraw = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                // zoom controls: 
                if (event.key.code == sf::Keyboard::Up) zoom *= 0.9;
                if (event.key.code == sf::Keyboard::Down) zoom /= 0.9;

                // panning controls
                if (event.key.code == sf::Keyboard::W) offsetY -= zoom * 50;
                if (event.key.code == sf::Keyboard::S) offsetY += zoom * 50;
                if (event.key.code == sf::Keyboard::A) offsetX -= zoom * 50;
                if (event.key.code == sf::Keyboard::D) offsetX += zoom * 50;

                needsRedraw = true;
            }

        }

        if (needsRedraw) {
            for (int y = 0; y < Height; ++y) {
                for (int x = 0; x < Width; ++x) {
                    std::complex<double> point = screenToComplex(x, y, zoom, offsetX, offsetY);
                    int iterations = calculateMandelbrot(point);

                    // map iter to colour
                    sf::Color color = (iterations == maxIterations) ? sf::Color::Black : sf::Color(255 - iterations * 255 / maxIterations, 255 - iterations * 255 / maxIterations / 2, iterations * 255 / maxIterations);

                    mandelbrotImage.setPixel(x, y, color);
                }
            }

            texture.loadFromImage(mandelbrotImage);
            sprite.setTexture(texture);
            needsRedraw = false;
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}