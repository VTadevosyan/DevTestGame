
#ifndef GUI_CANVAS_HPP
#define GUI_CANVAS_HPP

#include <SFML/Graphics.hpp>

#include <vector>


class index;

namespace gui {

//// @class canvas
// // @brief Canvas is used for drawing board's background tiles on given position in given window
class canvas
{
public:
	//// @brief Constructor
	//// @param[in] r Render window where will be drawn board and items
	//// @param[in] o The canvas vertical start position (offset from the top)
	canvas(sf::RenderWindow* r, const size_t o);

	//// @brief Destructor
	~canvas();

	//// @brief Draw canvas
	void draw();

private:
	void init();
	void clear();

	void create_tiles();

private:
	sf::RenderWindow* m_window = nullptr;
	std::vector<sf::Sprite*> m_tiles;
	sf::Texture* m_dark_tile = nullptr;
	sf::Texture* m_light_tile = nullptr;
	const size_t m_offset;
};

} //// gui namespace

#endif // GUI_CANVAS_HPP

