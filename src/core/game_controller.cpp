
#include "board.hpp"
#include "boosters.hpp"
#include "config.hpp"
#include "exceptions.hpp"
#include "game_controller.hpp"
#include "matcher.hpp"
#include "match_data.hpp"
#include "notifier.hpp"
#include "patterns.hpp"

#include <algorithm>
#include <list>
#include <cassert>
#include <ctime>
#include <iostream>



game_controller* game_controller::s_game_controller = nullptr;


void game_controller::create()
{
	s_game_controller = new game_controller;
}

void game_controller::destroy()
{
	assert(s_game_controller != nullptr);
    delete s_game_controller;
	s_game_controller = nullptr;
}

game_controller* game_controller::get()
{
	assert(s_game_controller != nullptr);
	return s_game_controller;
}

game_controller::game_controller()
{
    init();
    notifier::create();
    notifier::get()->add_listener(m_objectives.get());
	notifier::get()->add_listener(this);
}

game_controller::~game_controller()
{
	notifier::get()->remove_listener(m_objectives.get());
	notifier::get()->remove_listener(this);
    assert(m_config);
    m_config.reset();
    assert(m_matcher);
    m_matcher.reset();
    assert(m_board);
    m_board.reset();
    assert(m_objectives);
    m_objectives.reset();
    notifier::destroy();
}

void game_controller::init()
{
    load_config();
    init_patterns();
    m_board = board::ptr(new board(m_config->get_rows(), m_config->get_cols()));
    m_objectives = objectives::ptr(new objectives(m_config));

    m_moves_count = m_config->get_moves_count();
    m_figure_colors_count = m_config->get_figures_count();
    for (size_t i = 0; i < m_figure_colors_count; ++i) {
        m_color_types.push_back(static_cast<figure::color>(i));
    }
    init_booster_types();
}

void game_controller::init_booster_types()
{
    m_booster_types = {
        std::make_pair(pattern::type::horizontal, booster::type::vertical),
        std::make_pair(pattern::type::vertical, booster::type::horizontal),
        std::make_pair(pattern::type::radial, booster::type::radial),
        std::make_pair(pattern::type::t, booster::type::radial)
    };
}

void game_controller::init_patterns()
{
    m_matcher = matcher::ptr(new matcher);
    m_matcher->add_pattern(new t_pattern);
    m_matcher->add_pattern(new radial_pattern);
    m_matcher->add_pattern(new horizontal_pattern);
    m_matcher->add_pattern(new vertical_pattern);
}

void game_controller::start_game()
{
    fill_board();
	update_game_status(game_status::in_progress);
}

void game_controller::load_config()
{
    m_config = config::ptr(new config);
    m_config->load("../../../resources/CONFIG.JSON");
    assert(m_config->is_valid());
}

void game_controller::on_objectives_completed() noexcept
{
	update_game_status(game_status::passed);
}

game_controller::game_status game_controller::get_game_status() const noexcept
{
	return m_game_status;
}

void game_controller::update_game_status(const game_status s) noexcept
{
	if (s == game_status::not_started) {
		m_game_status = s;
	}
	if (m_game_status == game_status::not_started && s == game_status::in_progress) {
		m_game_status = s;
		return;
	}
	if (m_game_status == game_status::in_progress) {
		m_game_status = s;
		notifier* n = notifier::get();
		assert(n != nullptr);
		switch (m_game_status) {
			case game_status::failed: {
				n->on_level_failed();
			} break;
			case game_status::passed: {
				n->on_level_passed();
			} break;
			default:;
		}
	}
}

size_t game_controller::get_rows() const noexcept
{
    assert(m_board);
    return m_board->rows();
}

size_t game_controller::get_cols() const noexcept
{
    assert(m_board.get() != nullptr);
    return m_board->columns();
}

size_t game_controller::get_moves_count() const noexcept
{
	return m_moves_count;
}

board_item* game_controller::get_board_item(const index& i) const noexcept
{
	return m_board->get_item(i);
}

void game_controller::get_objectives(std::vector<objective*>& o) const noexcept
{
	assert(m_objectives != nullptr);
	m_objectives->get_objectives(o);
}

void game_controller::swap(const index& i1, const index& i2)
{
    assert(i1.is_valid());
    assert(i2.is_valid());
    assert(can_swap(i1, i2));
    m_board->swap(i1, i2);
	notifier::get()->on_items_swapped(i1, i2);
}

bool game_controller::can_swap(const index& i1, const index& i2) const noexcept
{
    if (!i1.is_valid() || !i2.is_valid()) {
        return false;
    }
    assert(m_board);
    return m_board->can_swap(i1, i2);
}

void game_controller::process_selection(const index& i)
{
	//// specified index should be valid
	if (!i.is_valid()) {
		return;
	}
	//// if selection is empty select item
	if (!m_selected_index.is_valid()) {
		m_selected_index = i;
		return;
	}
	//// booster should be activated after clicking on it twice
	if (!can_swap(m_selected_index, i)) {
		if (m_selected_index == i) {
			board_item* item = m_board->get_item(m_selected_index);
			if (item->is_booster()) {
				activate_booster(dynamic_cast<booster*>(item), m_selected_index);
				drop_new_items();
				decrease_moves_count();
			}
		}
		m_selected_index = invalid_index;
		return;
	}
	//// swap two items
	swap(m_selected_index, i);
	board_item* item1 = m_board->get_item(m_selected_index);
	board_item* item2 = m_board->get_item(i);
	assert(item1 != nullptr);
	assert(item2 != nullptr);
	//// swapping two boosters doesn't allowed
	if (item1->is_booster() && item2->is_booster()) {
		swap(m_selected_index, i);
		m_selected_index = invalid_index;
		return;
	}
	//// activate booster if one of swapping items is booster
	if (item1->is_booster()) {
		activate_booster(dynamic_cast<booster*>(item1), m_selected_index);
		match_data md;
		const bool matched = match(m_selected_index, md);
		if (matched) {
			process_matching(i, md);
		}
		drop_new_items();
		decrease_moves_count();
		m_selected_index = invalid_index;
		return;
	}
	if (item2->is_booster()) {
		activate_booster(dynamic_cast<booster*>(item2), i);
		match_data md;
		const bool matched = match(m_selected_index, md);
		if (matched) {
			process_matching(i, md);
		}
		drop_new_items();
		decrease_moves_count();
		m_selected_index = invalid_index;
		return;
	}
	//// match items
	match_data md;
	std::list<index> mi;
	bool matched_first = match(m_selected_index, md);
	if (matched_first) {
		process_matching(m_selected_index, md);
	}
	md.reset();
	bool matched_second = match(i, md);
	if (matched_second) {
		process_matching(i, md);
	}
	//// items should be swapped back if nothing happpens after swap
	if (!matched_first && !matched_second) {
		swap(m_selected_index, i);
		m_selected_index = invalid_index;
		return;
	}
	//// process
	drop_new_items();
	decrease_moves_count();
	m_selected_index = invalid_index;
}

bool game_controller::match(const index& i, match_data& md)
{
	assert(m_board);
	assert(m_matcher);
	if (!i.is_valid()) {
		return false;
	}
	return m_matcher->match(m_board, i, md);
}

bool game_controller::proxy_match(const index& i1, const index& i2) const
{
	assert(m_board);
	assert(m_matcher);
	return m_matcher->proxy_match(m_board, i1, i2);
}

void game_controller::activate_booster(booster* b, const index& i)
{
	assert(b != nullptr);
	std::set<index> impact_areas;
	b->activate(m_board, i, impact_areas);
	std::list<index> destroyed_items(std::begin(impact_areas), std::end(impact_areas));
	notifier::get()->on_items_destroyed(destroyed_items);
	std::set<int> impact_cols;
	for (auto it : impact_areas) {
		impact_cols.insert(it.column());
	}
	drop_columns(impact_cols);
}

void game_controller::process_matching(const index& i, const match_data& md)
{
	std::list<index> mi;
	md.get_indexes(mi);
	for (auto it : mi) {
		m_board->destroy_item(it);
	}
	//// Notifies about destroyed items
	notifier::get()->on_items_destroyed(mi);

	if (mi.size() > 3) {
		booster* b = create_booster(m_booster_types[md.get_pattern_type()]);
		m_board->add_item(b, i);
		notifier::get()->on_booster_created(b, i);
	}

	std::set<size_t> unique_dropped_indexes;
	std::list<index> dropped_indexes;
	for (auto it : mi) {
		m_board->drop_column(it.column(), dropped_indexes);
		unique_dropped_indexes.insert(it.column());
	}
	std::list<size_t> di(std::begin(unique_dropped_indexes), std::end(unique_dropped_indexes));
	notifier::get()->on_columns_dropped(di);
	

	for (auto it : dropped_indexes) {
		board_item* item = m_board->get_item(it);
		if (!item) {
			continue;
		}
		match_data md;
		std::list<index> mi;
		bool matched = m_matcher->match(m_board, it, md);
		if (matched) {
			md.get_indexes(mi);
			std::set<int> drop_indexes;
			for (auto it : mi) {
				m_board->destroy_item(it);
				drop_indexes.insert(it.column());
			}
			notifier::get()->on_items_destroyed(mi);
			if (mi.size() > 3) {
				booster* b = create_booster(m_booster_types[md.get_pattern_type()]);
				m_board->add_item(b, it);
				notifier::get()->on_booster_created(b, i);
			}
			drop_columns(drop_indexes);
		}
	}
}

void game_controller::drop_columns(const std::set<int>& column_indexes)
{
	assert(m_board);
	if (column_indexes.empty()) {
		return;
	}
	for (int i : column_indexes) {
		m_board->drop_column(i);
	}
	std::list<size_t> di(std::begin(column_indexes), std::end(column_indexes));
	notifier::get()->on_columns_dropped(di);
}

void game_controller::find_matchings_and_destroy()
{
	assert(m_board);
	notifier* n = notifier::get();
	assert(n != nullptr);
	const size_t rows = m_board->rows();
	const size_t cols = m_board->columns();
	for (size_t r = 0; r <rows; ++r) {
		for (size_t c = 0; c < cols; ++c) {
			index current_index = index(r, c);
			match_data md;
			board_item* item = m_board->get_item(current_index);
			if (!item) {
				continue;
			}
			bool matched = m_matcher->match(m_board, current_index, md);
			if (matched) {
				std::list<index> mi;
				md.get_indexes(mi);
				std::set<int> drop_cols;
				for (auto it : mi) {
					m_board->destroy_item(it);
					drop_cols.insert(it.column());
				}
				n->on_items_destroyed(mi);
				if (mi.size() > 3) {
					booster* b = create_booster(m_booster_types[md.get_pattern_type()]);
					m_board->add_item(b, current_index);
					n->on_booster_created(b, current_index);
				}
				for (auto it : drop_cols) {
					std::list<index> tmp_i;
					m_board->drop_column(it, tmp_i);
				}
				std::list<size_t> di(std::begin(drop_cols), std::end(drop_cols));
				n->on_columns_dropped(di);
			}
		}
	}
}

void game_controller::drop_new_items()
{
	std::list<index> empty_tiles;
	m_board->get_empty_tiles(empty_tiles);
	while (!empty_tiles.empty()) {
		std::list<std::pair<figure*, index>> new_items_data;
		for (auto it : empty_tiles) {
			figure* f = new figure(figure::color(rand() % m_figure_colors_count));
			m_board->add_item(f, it);
			new_items_data.push_back(std::make_pair(f, it));
		}
		notifier::get()->on_new_items_dropped(new_items_data);
		find_matchings_and_destroy();
		empty_tiles.clear();
		m_board->get_empty_tiles(empty_tiles);
	}
	//// Shuffle if there are no any moves
	if (!moves_available()) {
		shuffle();
	}
}

void game_controller::fill_board()
{
    srand(time(0));
	const size_t rows = m_board->rows();
	const size_t cols = m_board->columns();
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
			board_item* item = m_board->get_item(index(i, j));
			if (item != nullptr) {
				continue;
			}
			bool matched = true;
			while (matched) {
				figure* f = new figure(figure::color(rand() % m_figure_colors_count));
				m_board->add_item(f, index(i, j));
				match_data md;
				matched = m_matcher->match(m_board, index(i, j), md);
				if (matched) {
					m_board->destroy_item(index(i, j));
					figure* f = new figure(figure::color(rand() % m_figure_colors_count));
					m_board->add_item(f, index(i, j));
				}
			}
        }
    }
	if (!moves_available()) {
		shuffle();
	}
}

void game_controller::shuffle()
{
	//// XXX TODO INFINITE LOOP ???
	//// NEED TO CHECK IF CAN'T FIND ANY MOVES EVEN SHUFFLING BOARD
	while (true) {
		m_board->shuffle();
		notifier::get()->on_shuffle();
		find_matchings_and_destroy();
		drop_new_items();
		if (moves_available()) {
			break;
		}
	}
}

booster* game_controller::create_booster(const booster::type& bt) noexcept
{
    switch (bt) {
        case booster::type::horizontal:
            return new horizontal_bomb;
        case booster::type::vertical:
            return new vertical_bomb;
        case booster::type::radial:
            return new radial_bomb;
    }
    return nullptr;
}

void game_controller::decrease_moves_count()
{
	assert(m_moves_count);
	--m_moves_count;
	if (m_moves_count == 0) {
		update_game_status(game_status::failed);
	}
}

bool game_controller::moves_available()
{
	assert(m_board);
	const size_t rows = m_board->rows();
	const size_t cols = m_board->columns();
	//// first check boosters
	for (size_t r = 0; r < rows; ++r) {
		for (size_t c = 0; c < cols; ++c) {
			board_item* item = m_board->get_item(index(r, c));
			if (item != nullptr && item->is_booster()) {
				return true;
			}
		}
	}
	//// swap items with right and bottom neighbours and try match
	match_data md;
	for (size_t r = 0; r < rows; ++r) {
		for (size_t c = 0; c < cols; ++c) {
			index current(r, c);
			index right(r, c + 1);
			if (right.is_valid() && proxy_match(current, right)) {
				return true;
			}
			index bottom(r + 1, c);
			if (bottom.is_valid() && proxy_match(current, bottom)) {
				return true;
			}
		}
	}
	return false;
}
