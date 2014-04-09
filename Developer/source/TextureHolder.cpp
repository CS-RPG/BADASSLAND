//TextureHolder.cpp
#include <TextureHolder.hpp>

void TextureHolder::load(Textures::ID id, std::string fileName) {

	std::unique_ptr<sf::Texture> texture(new sf::Texture());
	texture->loadFromFile(fileName);

	mTextureMap.insert(std::make_pair(id, std::move(texture)));

}

sf::Texture& TextureHolder::get(Textures::ID id) {

	auto found = mTextureMap.find(id);
	return *found->second;

}