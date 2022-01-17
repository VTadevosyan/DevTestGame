
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "../core/index.hpp"
#include "../core/listener.hpp"

#include <SFML/Graphics.hpp>


namespace gui {

class canvas;
class objectives_pane;

//// @class main_window
//// @brief Draws game's window (Objectives, moves count, board and items)
class main_window : public listener
{
public:
	//// @brief Constructor
	main_window();

	//// @brief Constructor
	~main_window();

public:
	//// @brief creates and executes an event loop
	int exec_event_loop();

public:
	//// @brief Swap items on board and draw with delay
	void on_items_swapped(const index&, const index&) override;

	//// @brief Create booster on board and draw with delay
	void on_booster_created(booster*, const index&) override;

	//// @brief Drop items on given columns with animation
	void on_columns_dropped(const std::list<size_t>&) override;

	//// @brief Drop new items from top with animation
	void on_new_items_dropped(const std::list<std::pair<figure*, index>>&) override;

	//// @brief Destroy given items and show board with empty tiles to see the destroyed objects
	void on_items_destroyed(const std::list<index>&) override;

	//// @brief Load items from game controller and draw newly generated board with animation
	void on_shuffle() override;

private:
	void draw_items_with_animation();
	void draw();
	void draw_with_delay();

	void destroy_items();
	void create_items();

	index find_index(int, int) const;
	void mouse_pressed(const index&);

	size_t get_raw_index(const index&) const noexcept;
	index get_index_from_raw(size_t) const noexcept;
	sf::Texture* get_item_texture(const index&) const noexcept;

	void update_item_indexes(const std::list<size_t>&);

private:
	sf::RenderWindow* m_window;
	canvas* m_canvas;
	objectives_pane* m_objectives_pane;
	std::vector<sf::Sprite*> m_items;

};

} //// gui namespace

#endif
