
#ifndef TEXTURE_MAP_HPP
#define TEXTURE_MAP_HPP

#include "../core/boosters.hpp"
#include "../core/figure.hpp"

#include <SFML/Graphics.hpp>

#include <map>
#include <string>


namespace gui {

//// @class texture_map
//// @brief
class texture_map
{
public:
	//// @brief Creates a texture map data
	static void create();

	//// @brief Destroys a texture map data
	static void destroy();

public:
	//// @brief Find texture from given board item
	static sf::Texture* find_texture(board_item*) noexcept;

	//// @brief Find texture from given figure's color
	static sf::Texture* find_texture(const figure::color&) noexcept;

	//// @brief Find texture from given booster's type
	static sf::Texture* find_texture(const booster::type&) noexcept;

	//// @brief Gets the light tile's texture
	static sf::Texture* get_light_tile() noexcept;

	//// @brief Gets the dark tile's texture
	static sf::Texture* get_dark_tile() noexcept;

public:
	static std::string get_str(const sf::Texture* t)
	{
		for (auto it : s_figures) {
			if (it.second == t) {
				switch (it.first) {
				case figure::color::blue:  return "B";
				case figure::color::green: return "G";
				case figure::color::orange: return "O";
				case figure::color::red: return "R";
				case figure::color::violet: return "V";
				}
			}
		}
		for (auto it : s_boosters) {
			if (it.second == t) {
				switch (it.first) {
				case booster::type::horizontal: return "-";
				case booster::type::vertical: return "|";
				case booster::type::radial: return "*";
				}
			}
		}
		return std::string();
	}

private:
	static std::map<figure::color, sf::Texture*> s_figures;
	static std::map<booster::type, sf::Texture*> s_boosters;
	static sf::Texture* s_tile_light;
	static sf::Texture* s_tile_dark;
	static sf::IntRect s_rect;
};

} //// gui  namespace

#endif
