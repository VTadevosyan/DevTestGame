
#include "definitions.hpp"
#include "objectives_pane.hpp"
#include "texture_map.hpp"

#include "../core/game_controller.hpp"
#include "../core/notifier.hpp"
#include "../core/objective.hpp"

#include <cassert>


namespace gui {

objectives_pane::objectives_pane(sf::RenderWindow* w, const size_t x, const size_t y)
	: m_window(w)
	, m_x_pos(x)
	, m_y_pos(y)
{
	assert(m_window != nullptr);
	init();
}

objectives_pane::~objectives_pane()
{
	if (m_font != nullptr) {
		delete m_font;
		m_font = nullptr;
	}
	if (m_moves_label != nullptr) {
		delete m_moves_label;
		m_moves_label = nullptr;
	}
	if (m_game_status_label != nullptr) {
		delete m_game_status_label;
		m_game_status_label = nullptr;
	}
	for (auto it : m_bg_items) {
		delete it;
	}
	m_bg_items.clear();
	for (auto it : m_objective_figures) {
		delete it;
	}
	m_objective_figures.clear();
	for (auto it : m_objective_figures_count) {
		delete it;
	}
	m_objective_figures_count.clear();
	notifier::get()->remove_listener(this);
}

void objectives_pane::draw()
{
	//// Update moves count and objectives
	update();
	//// first draw background
	///m_window->pushGLStates();
	for (auto it : m_bg_items) {
		m_window->draw(*it);
	}
	m_window->draw(*m_moves_label);
	for (auto it : m_objective_figures) {
		m_window->draw(*it);
	}
	for (auto it : m_objective_figures_count) {
		m_window->draw(*it);
	}
	if (!m_game_status_label->getString().isEmpty()) {
		m_window->draw(*m_game_status_label);
	}
	///m_window->popGLStates();
}

void objectives_pane::on_level_passed() noexcept
{
	show_game_status_label();
}

void objectives_pane::on_level_failed() noexcept
{
	show_game_status_label();
}

void objectives_pane::update()
{
	game_controller* gc = game_controller::get();
	m_moves_label->setString(std::to_string(gc->get_moves_count()));
	std::vector<objective*> objectives;
	gc->get_objectives(objectives);
	for (size_t i = 0; i < objectives.size(); ++i) {
		m_objective_figures_count[i]->setString(std::to_string(objectives[i]->get_count()));
	}
}

void objectives_pane::init()
{
	init_font();
	init_textures();
	init_bg_items();
	init_moves_count();
	init_objectives();
	init_game_status_label();
	notifier::get()->add_listener(this);
}

void objectives_pane::init_font()
{
	m_font = new sf::Font;
	m_font->loadFromFile("../../../resources/arial.ttf");
}

void objectives_pane::init_textures()
{
	m_dark_tile = texture_map::get_dark_tile();
	m_light_tile = texture_map::get_light_tile();
}

void objectives_pane::init_moves_count()
{
	m_moves_label = create_text(std::to_string(game_controller::get()->get_moves_count()));
	m_moves_label->setPosition(0, 0);
}

void objectives_pane::init_bg_items()
{
	//// moves count
	m_bg_items.push_back(new sf::Sprite(*m_dark_tile));
	m_bg_items.back()->setPosition(0, m_y_pos);

	//// objectives
	game_controller* gc = game_controller::get();
	std::vector<objective*> objectives;
	gc->get_objectives(objectives);

	//// Background tiles
	const size_t count = objectives.size();
	for (size_t i = 0; i < count * 2; ++i) {
		sf::Sprite* sp = new sf::Sprite(*m_light_tile);
		sp->setPosition(ITEM_SIZE + (i * ITEM_SIZE), 0);
		m_bg_items.push_back(sp);
	}
}

void objectives_pane::init_objectives()
{
	//// objectives
	game_controller* gc = game_controller::get();
	std::vector<objective*> objectives;
	gc->get_objectives(objectives);
	const size_t count = objectives.size();
	//// Figures
	for (size_t i = 0; i < count; ++i) {
		sf::Texture* t = texture_map::find_texture(objectives[i]->get_color());
		assert(t != nullptr);
		sf::Sprite* sp = new sf::Sprite(*t);
		sp->setPosition(ITEM_SIZE + (i * 2 * ITEM_SIZE), 0);
		sp->setOrigin(0, -5);
		m_objective_figures.push_back(sp);
	}
	//// Figures count
	for (size_t i = 0; i < count; ++i) {
		sf::Text* count = create_text(std::to_string(objectives[i]->get_count()));
		count->setPosition((i * 2 * ITEM_SIZE) + ITEM_SIZE + ITEM_SIZE, 0);
		m_objective_figures_count.push_back(count);
	}
}

void objectives_pane::init_game_status_label()
{
	m_game_status_label = create_text("");
	m_game_status_label->setPosition(10, 10 + ITEM_SIZE);
}

void objectives_pane::show_game_status_label()
{
	game_controller::game_status status = game_controller::get()->get_game_status();
	std::string display_text;
	sf::Color color;
	switch (status) {
		case game_controller::game_status::failed: {
			display_text = "GAME FAILED !!!";
			color = sf::Color::Red;
		} break;
		case game_controller::game_status::passed: {
			display_text = "GAME COMPLETED !!!";
			color = sf::Color::Green;
		} break;
		default:;
	}
	m_game_status_label->setString(display_text);
	m_game_status_label->setFillColor(color);
	m_window->draw(*m_game_status_label);
}


sf::Text* objectives_pane::create_text(const std::string& text, const sf::Color& color)
{
	sf::Text* t = new sf::Text(text, *m_font, 50);
	t->setCharacterSize(40);
	t->setStyle(sf::Text::Bold);
	t->setFillColor(color);
	t->setOrigin(-10, -10);
	return t;
}

} //// gui namespace
