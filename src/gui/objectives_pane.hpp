
#ifndef GUI_OBJECTIVES_PANE_HPP
#define GUI_OBJECTIVES_PANE_HPP

#include "../core/listener.hpp"

#include <SFML/Graphics.hpp>

#include <vector>


namespace gui {


//// @class objectives_pane
//// @brief Objectives Pane shows current objectives data and moves count on given window
class objectives_pane : public listener
{
public:
	//// @brief Constructor
	objectives_pane(sf::RenderWindow*, const size_t, const size_t);

	//// @brief Destructor
	~objectives_pane();

public:
	//// @brief Updates and draws an objectives pane
	void draw();

public:
	//// @brief Handles game level passed event
	void on_level_passed() noexcept override;

	//// @brief Handles game level failed event
	void on_level_failed() noexcept override;

private:
	void init();
	void init_font();
	void init_textures();
	void init_bg_items();
	void init_moves_count();
	void init_objectives();
	void init_game_status_label();

	void show_game_status_label();

	void update();

	sf::Text* create_text(const std::string&, const sf::Color& = sf::Color::White);

private:
	sf::RenderWindow* m_window = nullptr;
	sf::Font* m_font = nullptr;
	sf::Text* m_moves_label = nullptr;
	sf::Text* m_game_status_label = nullptr;
	sf::Texture* m_dark_tile = nullptr;
	sf::Texture* m_light_tile = nullptr;
	std::vector<sf::Sprite*> m_bg_items;
	std::vector<sf::Sprite*> m_objective_figures;
	std::vector<sf::Text*> m_objective_figures_count;
	const size_t m_x_pos;
	const size_t m_y_pos;
};

} //// gui namespace


#endif //// GUI_OBJECTIVES_PANE_HPP

