
#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include "board.hpp"
#include "boosters.hpp"
#include "config.hpp"
#include "figure.hpp"
#include "index.hpp"
#include "listener.hpp"
#include "matcher.hpp"
#include "objectives.hpp"
#include "patterns.hpp"

#include <map>
#include <memory>


//// @class game_controller
//// @brief Singleton class to control the game: turns, status and etc...
//// Game configuration should be loaded from given config JSON file
//// The default config file: CONFIG.JSON
class game_controller : public listener
{
public:
    //// @enum gmae_status
    //// @brief Game status enumeration
    enum class game_status
    {
        not_started = 0,
        in_progress,
        passed,
        failed
    };

public:
	//// @brief Creates the game_controller
	static void create();

	//// @brief Destroys the game_controller example
	static void destroy();

    //// @brief Gets the game_controller example
    static game_controller* get();

private:
	static game_controller* s_game_controller;

public:
    //// @brief Constructor
    game_controller();

    //// @brief Destructor
    ~game_controller();

public:
    //// @brief Starts the game
    void start_game();

	//// @brief Gets the game status
	game_status get_game_status() const noexcept;

	//// @brief Gets the moves count
	size_t get_moves_count() const noexcept;

    //// @brief Gets the board rows count
    size_t get_rows() const noexcept;

    //// @brief Gets the board columns count
    size_t get_cols() const noexcept;

	//// @brief Gets the board item with given index
	board_item* get_board_item(const index& i) const noexcept;

	//// @brief Gets the objectives data
	void get_objectives(std::vector<objective*>&) const noexcept;

	//// @brief Process selection when user selects an item from board
	//// It should try to swap items or just keeps the selected item
	//// After swapping items controlle will try to find matchings, create or activate boosters
	//// Some objects will be destroyed after successfull match
	//// After successfull matching moves count will be decreased
	void process_selection(const index&);

	//// @brief Updates the current status
	//// @note Game status will 'passed' if all objectives will be completed
	////       Game status will 'failed' if moves count will be 0 but objectives won't be completed
	void update_game_status(const game_status) noexcept;

    //// @brief Determines whether if two specified indexes can be swapped or not
    //// @return true if indexes can be swapped and false otherwise
    //// @note You can swap indexes if they are neighbours (horizontal or vertical)
    bool can_swap(const index&, const index&) const noexcept;

    //// @brief Swaps given two indexes in board
    void swap(const index&, const index&);
	
	//// @brief Checks if moves available in board or not
	//// E.g Can swapped items match to patterns or not
	//// If there are no available moves need to shuffle
	bool moves_available();

	//// @brief Shuffle board items
	void shuffle();

public:
	void on_objectives_completed() noexcept override;

private:
    void init();
    void init_patterns();
    void load_config();
    void fill_board();
	void drop_columns(const std::set<int>&);
	void drop_new_items();
    void init_booster_types();
    booster* create_booster(const booster::type&) noexcept;

	bool match(const index&, match_data&);
	bool proxy_match(const index&, const index&) const;

	void activate_booster(booster*, const index&);

	void process_matching(const index&, const match_data&);

	void find_matchings_and_destroy();

	/// @brief Decreases moves count
	/// Notifies about failing level if moves count is 0 and objectives aren't completed
	void decrease_moves_count();



private:
    config::ptr m_config = nullptr;
    objectives::ptr m_objectives = nullptr;
    matcher::ptr m_matcher = nullptr;
    board::ptr m_board = nullptr;
    game_status m_game_status = game_status::not_started;
    size_t m_moves_count = 0;
    size_t m_figure_colors_count = 0;
	index m_selected_index = invalid_index;
    figure::colors_t m_color_types;
    std::map<pattern::type, booster::type> m_booster_types;

};


#endif /// GAME_CONTROLLER_HPP
