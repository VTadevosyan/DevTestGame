
#include "canvas.hpp"
#include "definitions.hpp"
#include "texture_map.hpp"

#include "../core/game_controller.hpp"

#include <cassert>


namespace gui {

canvas::canvas(sf::RenderWindow* w, const size_t o)
	: m_window(w)
	, m_offset(o)
{
	assert(m_window != nullptr);
	init();
}

canvas::~canvas()
{
	clear();
}

void canvas::draw()
{
	for (auto it : m_tiles) {
		m_window->draw(*it);
	}
}

void canvas::init()
{
	m_dark_tile = texture_map::get_dark_tile();
	m_light_tile = texture_map::get_light_tile();
	create_tiles();
}

void canvas::create_tiles()
{
	assert(m_tiles.empty());
	const size_t rows = game_controller::get()->get_rows();
	const size_t cols = game_controller::get()->get_cols();
	size_t x = 0;
	size_t y = m_offset;
	sf::Texture* t = m_dark_tile;
	for (size_t r = 0; r < rows; ++r) {
		x = 0;
		t = r % 2 == 0 ? m_dark_tile : m_light_tile;
		for (size_t c = 0; c < cols; ++c) {
			t = t == m_dark_tile ? m_light_tile : m_dark_tile;
			sf::Sprite* s = new sf::Sprite(*t);
			s->setPosition(x, y);
			m_tiles.push_back(s);
			x += ITEM_SIZE;
		}
		y += ITEM_SIZE;
	}
}

void canvas::clear()
{
	for (auto it : m_tiles) {
		if (it != nullptr) {
			delete it;
		}
	}
	m_tiles.clear();
}

} //// gui namespace
