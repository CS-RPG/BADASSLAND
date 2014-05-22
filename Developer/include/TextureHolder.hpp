//TextureHolder.hpp
#ifndef _TextureHolder_hpp_included_
#define _TextureHolder_hpp_included_

//#include <_includes_badass.hpp>
#include <_includes_system.hpp>

extern std::string				gTexturesDirectory;

namespace Textures {

	enum ID {

		Elf_Green,
		Elf_Red,
		Elf_Yellow,
		HP_Bar

	};

}

#define myTextureMapType std::map<std::string, std::unique_ptr<sf::Texture>>

class TextureHolder {
public:

	void							load(std::string id);

	sf::Texture&					get(std::string id);

private:

	myTextureMapType				mTextureMap;

};


#endif
