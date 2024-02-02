#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>
#include <cmath>

#include <imgui.h>
#include <imgui-SFML.h>
#include "SFML/Graphics.hpp"


class ShapeInterface : public sf::Shape
{
public:
    ShapeInterface(sf::Vector2f speed) : sf::Shape(), speed(speed) {};

    //pure virtuals

    virtual bool contains(sf::Vector2f mousePosition) = 0;
    virtual void onClick() = 0;
    virtual void resetHighlight() = 0;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
    virtual std::string getName() = 0;
    virtual void setNewSize(float size) = 0;
    virtual float getNewSize() = 0;
    virtual void setFillColor(sf::Color) = 0;
    virtual sf::Color getFillColor() = 0;
    virtual void setPosition(sf::Vector2f) = 0;
    virtual sf::Vector2f getPosition() = 0;
    virtual sf::Vector2f getCenter() = 0;

    //setters and getters

    sf::Vector2f getSpeed() const
    {
        return speed;
    }

    void setSpeed(int newSpeed)
    {
        std::unordered_map<int, float> speedMap = { {-2, 0.25f}, {-1, 0.5f}, {0, 1.0f}, {1, 2.0f}, {2, 4.0f} };
        speed.x = static_cast<int>(speed.x * (speedMap[newSpeed]));
        speed.y = static_cast<int>(speed.y * (speedMap[newSpeed]));
    }

    void revSpeed(sf::Vector2f revSpeed)
    {
        speed = revSpeed;
    }


private:
    sf::Vector2f speed;

};


class CustomCircle : public sf::CircleShape, public ShapeInterface
{
public:
    CustomCircle(std::string name, float radius, sf::Color fillColor, sf::Vector2f position, sf::Vector2f speed)
        : sf::CircleShape(radius, 100), name(name), fillColor(fillColor), position(position), ShapeInterface(speed)
    {
        sf::CircleShape::setOrigin(getRadius(), getRadius());
        sf::CircleShape::setFillColor(fillColor);
        sf::CircleShape::setPosition(position);
    };

    //technicality junk

    std::size_t getPointCount() const override
    {
        return 100;
    }
    sf::Vector2f getPoint(std::size_t index) const override
    {
        float angle = static_cast<float>(index) * 2.f * 3.1415926535f / static_cast<float>(getPointCount());
        float x = getRadius() * std::cos(angle);
        float y = getRadius() * std::sin(angle);

        return sf::Vector2f(x, y);
    }

    //pure virtual overrides

    std::string getName() override
    {
        return name;
    }

    void setNewSize(float size) override 
    {
        setRadius(size);
    }

    float getNewSize() override
    {
        return getRadius();
    }

    void setFillColor(sf::Color color) override
    {
        sf::Color newColor = sf::Color(color.r, color.g, color.b, 150);
        sf::CircleShape::setFillColor(newColor);
        fillColor = newColor;
    }

    sf::Color getFillColor() override
    {
        return fillColor;
    }

    void setPosition(sf::Vector2f newPos) override
    {
        sf::CircleShape::setPosition(newPos);
    }

    sf::Vector2f getPosition() override
    {
        return sf::CircleShape::getPosition();
    }

    sf::Vector2f getCenter() override
    {
        return sf::CircleShape::getPosition();
    }

    //functional

    bool contains(sf::Vector2f mousePosition) override
    {
        sf::Vector2f globalOrigin = CustomCircle::getPosition();
        float distance = std::sqrt(std::pow(mousePosition.x - globalOrigin.x, 2) + std::pow(mousePosition.y - globalOrigin.y, 2));

        return distance <= getRadius();
    }

    void onClick() override
    {
        sf::Color newColor = sf::Color(fillColor.r, fillColor.g, fillColor.b);
        sf::CircleShape::setOutlineThickness(12.0f);
        sf::CircleShape::setOutlineColor(newColor);
    }

    void resetHighlight() override
    {
        sf::CircleShape::setOutlineThickness(0.f);
        sf::CircleShape::setOutlineColor(sf::Color::Transparent);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(static_cast<sf::CircleShape>(*this), states);
    }

    //print

    void printShape()
    {
        std::cout << name << " of radius " << getRadius() << ", Color (" << static_cast<int>(fillColor.r) << " / " << static_cast<int>(fillColor.g) << " / " 
            << static_cast<int>(fillColor.b) << ")" << std::endl;
    }


private:
    std::string name;
    float radius;
    sf::Color fillColor;
    sf::Vector2f position;

};


class CustomRectangle : public sf::RectangleShape, public ShapeInterface
{
public:
    CustomRectangle(std::string name, sf::Vector2f size, sf::Color& fillColor, sf::Vector2f position, sf::Vector2f speed)
        : sf::RectangleShape(size), name(name), fillColor(fillColor), position(position), ShapeInterface(speed)
    {
        sf::RectangleShape::setFillColor(fillColor);
        sf::RectangleShape::setPosition(position);
        aspectRatio = (getSize().y / getSize().x);
    }

    //technicality junk

    std::size_t getPointCount() const override
    {
        return 4;
    }
    sf::Vector2f getPoint(std::size_t index) const override
    {
        switch (index)
        {
        case 0: return sf::Vector2f(0.f, 0.f);
        case 1: return sf::Vector2f(getSize().x, 0.f);
        case 2: return sf::Vector2f(getSize().x, getSize().y);
        case 3: return sf::Vector2f(0.f, getSize().y);
        default: return sf::Vector2f();
        }
    }

    //pure virtual overrides

    std::string getName() override
    {
        return name;
    }

    void setNewSize(float size) override
    {
        sf::Vector2f newSize = { size, size * aspectRatio };
        setSize(newSize);
    }

    float getNewSize() override
    {
        return getSize().x;
    }

    void setFillColor(sf::Color color) override
    {
        sf::Color newColor = sf::Color(color.r, color.g, color.b, 150);
        sf::RectangleShape::setFillColor(newColor);
        fillColor = newColor;
    }

    sf::Color getFillColor() override
    {
        return fillColor;
    }

    void setPosition(sf::Vector2f newPos) override
    {
        sf::RectangleShape::setPosition(newPos);
    }

    sf::Vector2f getPosition() override
    {
        return sf::RectangleShape::getPosition();
    }

    sf::Vector2f getCenter() override
    {
        sf::Vector2f rectCenter = sf::Vector2f{ getPosition().x + (getSize().x / 2), getPosition().y + (getSize().y / 2) };
        return rectCenter;
    }

    //functional

    bool contains(sf::Vector2f mousePosition) override
    {
        if (mousePosition.x >= sf::RectangleShape::getPosition().x && mousePosition.x <= (sf::RectangleShape::getPosition().x + getSize().x))
        {
            if (mousePosition.y >= sf::RectangleShape::getPosition().y && mousePosition.y <= (sf::RectangleShape::getPosition().y + getSize().y))
            {
                return true;
            }
        }
        return false;
    }

    void onClick() override
    {
        sf::Color newColor = sf::Color(fillColor.r, fillColor.g, fillColor.b);
        sf::RectangleShape::setOutlineThickness(12.0f);
        sf::RectangleShape::setOutlineColor(newColor);
    }

    void resetHighlight() override
    {
        sf::RectangleShape::setOutlineThickness(0.f);
        sf::RectangleShape::setOutlineColor(sf::Color::Transparent);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(static_cast<sf::RectangleShape>(*this), states);
    }

    //print

    void printShape()
    {
        std::cout << name << " of size " << getSize().x << " by " << getSize().y << ", Color (" << static_cast<int>(fillColor.r) << " / " << static_cast<int>(fillColor.g) << " / "
            << static_cast<int>(fillColor.b) << ")" << std::endl;
    }


private:
    std::string name;
    sf::Vector2f size;
    sf::Color fillColor;
    sf::Vector2f position;
    float aspectRatio;

};


int main()
{
    //window setup
    const int wWidth = 1280;
    const int wHeight = 720;

    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "peepee poopoo");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);
    sf::Clock deltaClock;


    //data and file read setup
    ShapeInterface* clickedShape = nullptr;
    std::ifstream inputFile("proj1_config.txt");
    std::string line;
    std::vector<std::unique_ptr<ShapeInterface>> shapes;
    int alphaVal = 150;

    ImGui::GetStyle().ScaleAllSizes(1.0f);

    if (!inputFile.is_open())
    {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }


    //read in config
    while (std::getline(inputFile, line))
    {
        std::istringstream iss(line);
        std::string shapeType;
        iss >> shapeType;

        if (shapeType == "Circle")
        {
            std::string name;
            float posX, posY;
            float speedX, speedY;
            int fillR, fillG, fillB;
            float radius;

            if (iss >> name >> posX >> posY >> speedX >> speedY >> fillR >> fillG >> fillB >> radius)
            {
                sf::Color fillColor(fillR, fillG, fillB, alphaVal);
                sf::Vector2f posVec2 = sf::Vector2f{ posX, posY };
                sf::Vector2f speedVec2 = sf::Vector2f{ speedX, speedY };
                auto customCircle = std::make_unique<CustomCircle>(name, radius, fillColor, posVec2, speedVec2);
                shapes.emplace_back(std::move(customCircle));
            }
            else
            {
                std::cerr << "Error: can't read shape type: C";
            }
        }
        else if (shapeType == "Rectangle")
        {
            std::string name;
            float posX, posY;
            float speedX, speedY;
            int fillR, fillG, fillB;
            float sizeW, sizeH;

            if (iss >> name >> posX >> posY >> speedX >> speedY >> fillR >> fillG >> fillB >> sizeW >> sizeH)
            {
                sf::Color fillColor(fillR, fillG, fillB, alphaVal);
                sf::Vector2f size = sf::Vector2f(sizeW, sizeH);
                sf::Vector2f posVec2 = sf::Vector2f{ posX, posY };
                sf::Vector2f speedVec2 = sf::Vector2f{ speedX, speedY };
                auto customRectangle = std::make_unique<CustomRectangle>(name, size, fillColor, posVec2, speedVec2);
                shapes.emplace_back(std::move(customRectangle));
            }
            else
            {
                std::cerr << "Error: can't read shape type: R";
            }
        }
        else
        {
            std::cerr << "Error: unknown shape type.";
        }
    }


    //font
    sf::Font myFont;
    if (!myFont.loadFromFile("bin/fonts/tech.ttf"))
    {
        std::cerr << "Could not load font!\n";
        exit(-1);
    }
    ImGui::SFML::UpdateFontTexture();


    //variables for the window loop
    sf::Text text("Sample Text", myFont, 24);
    text.setFillColor(sf::Color::Black);
    text.setPosition(0, wHeight - (float)text.getCharacterSize());

    sf::Vector2f resetPos = sf::Vector2f{ 30.0f, 30.0f };
    std::string sizeName = "Radius";
    float sliderSize = 0.0f;
    bool drawShapes = true;
    bool drawText = true;
    float newColor[3];
    int newSpeed = 0;
    float c[3];


    //window loop
    try
    {
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {

                ImGui::SFML::ProcessEvent(event);
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }


                //shape select & ImGui menu initialization on shape select
                bool imguiCapturingMouse = ImGui::GetIO().WantCaptureMouse;
                if (!imguiCapturingMouse)
                {
                    if (event.type == sf::Event::MouseButtonPressed)
                    {
                        if (event.mouseButton.button == sf::Mouse::Left)
                        {
                            sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

                            if (clickedShape != nullptr)
                            {
                                clickedShape->resetHighlight();
                                clickedShape = nullptr;
                            }

                            for (const auto& shape : shapes)
                            {
                                if (shape->contains(mousePosition))
                                {
                                    clickedShape = shape.get();
                                    clickedShape->onClick();
                                    sf::Color temp = clickedShape->getFillColor();
                                    c[0] = (temp.r / 255);
                                    c[1] = (temp.g / 255);
                                    c[2] = (temp.b / 255);
                                    sliderSize = clickedShape->getNewSize();

                                    if (auto circle = dynamic_cast<sf::CircleShape*>(clickedShape))
                                    {
                                        sizeName = "Radius";
                                    }
                                    else if (auto rect = dynamic_cast<sf::RectangleShape*>(clickedShape))
                                    {
                                        sizeName = "Width";
                                    }
                                }
                            }
                        }
                    }
                }
            }


            //shape bounce physics
            for (auto& shape : shapes)
            {
                if (auto circle = dynamic_cast<sf::CircleShape*>(shape.get()))
                {
                    if (circle->getGlobalBounds().left <= 0 || circle->getGlobalBounds().left + (2 * circle->getRadius()) >= wWidth)
                    {
                        float curSpeed = shape->getSpeed().x;
                        sf::Vector2f revSpeed = sf::Vector2f{ curSpeed * -1.0f, shape->getSpeed().y};
                        shape->revSpeed(revSpeed);
                    }
                    else if (circle->getGlobalBounds().top <= 0 || circle->getGlobalBounds().top + (2 * circle->getRadius()) >= wHeight)
                    {
                        float curSpeed = shape->getSpeed().y;
                        sf::Vector2f revSpeed = sf::Vector2f{ shape->getSpeed().x, curSpeed * -1.0f};
                        shape->revSpeed(revSpeed);
                    }
                }
                else if (auto rect = dynamic_cast<sf::RectangleShape*>(shape.get()))
                {
                    if (rect->getGlobalBounds().left <= 0 || (rect->getGlobalBounds().left + rect->getSize().x) >= wWidth)
                    {
                        float curSpeed = shape->getSpeed().x;
                        sf::Vector2f revSpeed = sf::Vector2f{ curSpeed * -1.0f, shape->getSpeed().y };
                        shape->revSpeed(revSpeed);
                    }
                    else if (rect->getGlobalBounds().top <= 0 || (rect->getGlobalBounds().top + rect->getSize().y) >= wHeight)
                    {
                        float curSpeed = shape->getSpeed().y;
                        sf::Vector2f revSpeed = sf::Vector2f{ shape->getSpeed().x, curSpeed * -1.0f };
                        shape->revSpeed(revSpeed);
                    }
                }
            }

            ImGui::SFML::Update(window, deltaClock.restart());


            //ImGui menu
            ImGui::Begin("Window title");
            ImGui::Text("Window text!");
            ImGui::Checkbox("Draw Shapes", &drawShapes);
            ImGui::SameLine();
            ImGui::Checkbox("Draw Text", &drawText);
            
            if (clickedShape != nullptr)
            {
                if (ImGui::SliderFloat(sizeName.c_str(), &sliderSize, 1.0f, 300.0f))
                {
                    std::cout << "Changing size... old size = " << clickedShape->getNewSize() << std::endl;
                    clickedShape->setNewSize(sliderSize);
                    std::cout << "New size = " << clickedShape->getNewSize() << std::endl;
                }

                if (ImGui::SliderInt("Speed", &newSpeed, -2, 2))
                {
                    std::cout << "Changing speed... speed = " << clickedShape->getSpeed().x << ", " << clickedShape->getSpeed().y << std::endl;
                    clickedShape->setSpeed(newSpeed);
                    std::cout << "New speed = " << clickedShape->getSpeed().x << ", " << clickedShape->getSpeed().y << std::endl;
                }

                if (ImGui::ColorEdit3("Color", c))
                {
                    std::cout << "Changing color... old color = " << clickedShape->getFillColor().r << ", " << clickedShape->getFillColor().g << ", " << clickedShape->getFillColor().b << std::endl;
                    sf::Color newColor(c[0] * 255, c[1] * 255, c[2] * 255);
                    clickedShape->setFillColor(newColor);
                    std::cout << "New color = " << clickedShape->getFillColor().r << ", " << clickedShape->getFillColor().g << ", " << clickedShape->getFillColor().b << std::endl;
                }

                if (ImGui::Button("Reset Shape"))
                {
                    clickedShape->setPosition(resetPos);
                }
            }
            else
            {
                ImGui::SliderFloat(sizeName.c_str(), &sliderSize, 1.0f, 300.0f);
                ImGui::SliderInt("Speed", &newSpeed, -2, 2);
                ImGui::ColorEdit3("Color", c);
                ImGui::Button("Reset Shape");
            }
            ImGui::End();


            //update shape locations based on speed in each frame
            std::vector<sf::Vector2f> newPositions;
            for (auto& shape : shapes)
            {
                sf::Vector2f newPosition = shape->getPosition() + shape->getSpeed();
                newPositions.push_back(newPosition);
            }
            for (std::size_t i = 0; i < shapes.size(); ++i)
            {
                shapes[i]->setPosition(newPositions[i]);
            }


            //clear window for the draw
            window.clear(sf::Color(255, 255, 255));


            //draw loop
            for (std::size_t i = 0; i < shapes.size(); ++i)
            {
                if (drawShapes)
                {
                    shapes[i]->draw(window, sf::RenderStates::Default);
                }
                if (drawText)
                {
                    std::string labelString = shapes[i]->getName();
                    sf::Text text(labelString, myFont, 24);
                    text.setPosition(shapes[i]->getCenter() - sf::Vector2f(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f));
                    window.draw(text);
                }
            }

            ImGui::SFML::Render(window);
            window.display();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }


    //clean up
    clickedShape = nullptr;
    ImGui::SFML::Shutdown();
    return 0;
};
