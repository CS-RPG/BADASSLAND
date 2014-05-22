//TextureHolder.cpp
#include <TextureHolder.hpp>

void TextureHolder::load(std::string id) {

	std::unique_ptr<sf::Texture> texture(new sf::Texture());
	texture->loadFromFile("./textures/" + id + ".png");

	mTextureMap.insert(std::make_pair(id, std::move(texture)));

}

sf::Texture& TextureHolder::get(std::string id) {

	auto found = mTextureMap.find(id);
	return *found->second;

}
