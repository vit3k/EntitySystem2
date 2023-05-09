#pragma once
#include <vector>
#include <string>
#include <memory>
#include "EntityW/Component.h"
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>

class Frame
{
public:
	int width;
	int height;
	int x;
	int y;
	Frame(int width, int height, int x, int y) : width(width), height(height), x(x), y(y){};
};

class SpriteSheet
{
	int frameWidth;
	int frameHeight;
	std::string path;
public:
	std::shared_ptr<sf::Texture> texture;
	
	SpriteSheet(std::string path, int frameWidth, int frameHeight) : path(path), frameWidth(frameWidth), frameHeight(frameHeight) {};
	static std::shared_ptr<SpriteSheet> create(std::string path, int frameWidth, int frameHeight)
	{
		auto spriteSheet = std::make_shared<SpriteSheet>(path, frameWidth, frameHeight);
		spriteSheet->load();
		return spriteSheet;
	}
	std::vector<Frame> frames;

	void load()
	{
		texture = std::make_shared<sf::Texture>();
		texture->loadFromFile(path);
		auto size = texture->getSize();
		auto framesX = size.x / frameWidth;
		auto framesY = size.y / frameHeight;
		for (int y = 0; y < framesY; y++)
		{
			for (int x = 0; x < framesX; x++)
			{
				frames.emplace_back(frameWidth, frameHeight, x * frameWidth, y * frameHeight);
			}
		}
	}
};

class Animation
{
public:
	std::string name;
	int fps;
	std::vector<int> frames;
	std::shared_ptr<SpriteSheet> spriteSheet;

	Animation(std::string name, int fps, std::shared_ptr<SpriteSheet> spriteSheet, std::vector<int> frames) 
		: name(name), fps(fps), spriteSheet(spriteSheet), frames(frames) {};

};


class AnimatedSpriteComponent : EntityW::Component<AnimatedSpriteComponent>
{
    std::string animation;
public:
    std::shared_ptr<sf::Sprite> sprite;
    std::map<std::string, std::shared_ptr<Animation>> animations;

	AnimatedSpriteComponent(std::map<std::string, std::shared_ptr<Animation>> animations): animations(animations) {
		sprite = std::make_shared<sf::Sprite>();
	};

    void setAnimation(std::string animation)
    {
        this->animation = animation;
        //this->sprite = animations[animation]->
    }
};

class SpriteComponent : public EntityW::Component<SpriteComponent>
{
public:
    Vector2 scale;
    std::shared_ptr<sf::Sprite> sprite;
    SpriteComponent(std::string path): path(path) {
        scale.x = 1.0f;
        scale.y = 1.0f;
    };
	void load()
	{
		texture = std::make_shared<sf::Texture>();
		texture->loadFromFile(path);
        texture->setSmooth(true);
		sprite = std::make_shared<sf::Sprite>();
		sprite->setTexture(*texture);

	}
    static std::shared_ptr<SpriteComponent> create(std::string path)
	{
		auto spriteComponent = std::shared_ptr<SpriteComponent>(new SpriteComponent(path));
		spriteComponent->load();
		return spriteComponent;
	}
    ~SpriteComponent()
    {
        printf("Deleting sprite component\n");
    }
private:
    std::shared_ptr<sf::Texture> texture;
    std::string path;
};

