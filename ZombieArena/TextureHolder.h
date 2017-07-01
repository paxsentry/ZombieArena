#ifndef ZOMBIE_ARENA_TEXTURE_HOLDER_H
#define ZOMBIE_ARENA_TEXTURE_HOLDER_H

#include "SFML\Graphics.hpp"
#include <map>

class TextureHolder
{
public:
	TextureHolder();
	static sf::Texture& getTexture(std::string const& fileName);

private:
	std::map<std::string, sf::Texture> m_texures;
	static TextureHolder* m_s_instance;
};

#endif // !ZOMBIE_ARENA_TEXTURE_HOLDER_H