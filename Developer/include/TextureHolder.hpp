//TextureHolder.hpp
#ifndef _TextureHolder_hpp_included_
#define _TextureHolder_hpp_included_

//#include <_includes_badass.hpp>
#include <_includes_system.hpp>

namespace Textures {

	enum ID {

		Elf_Green,
		Elf_Red,
		Elf_Yellow,
		HP_Bar

	};

}

#define myTextureMapType std::map<Textures::ID, std::unique_ptr<sf::Texture>>

class TextureHolder {
public:

	void							load(Textures::ID id, std::string fileName);

	sf::Texture&					get(Textures::ID id);

private:

	myTextureMapType				mTextureMap;

};


#endif