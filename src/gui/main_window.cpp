
#include "canvas.hpp"
#include "definitions.hpp"
#include "main_window.hpp"
#include "objectives_pane.hpp"
#include "texture_map.hpp"

#include "../core/game_controller.hpp"
#include "../core/notifier.hpp"

#include <iostream>


namespace gui {

main_window::main_window()
{
	game_controller::create();
	game_controller* gc = game_controller::get();
	gc->start_game();
	texture_map::create();

	size_t w = gc->get_cols() * ITEM_SIZE;
	size_t h = gc->get_rows() * ITEM_SIZE + 2 * BOARD_OFFSET;

	m_window = new sf::RenderWindow(sf::VideoMode(w, h), "Match");
	m_window->setFramerateLimit(FPS);

	m_canvas = new canvas(m_window, BOARD_OFFSET + ITEM_SIZE);
	m_objectives_pane = new objectives_pane(m_window, 0, 0);


	notifier::get()->add_listener(this);

	create_items();
}

main_window::~main_window()
{
	if (m_window != nullptr) {
		delete m_window;
		m_window = nullptr;
	}
	if (m_canvas != nullptr) {
		delete m_canvas;
		m_canvas = nullptr;
	}
	if (m_objectives_pane != nullptr) {
		delete m_objectives_pane;
		m_objectives_pane = nullptr;
	}
	texture_map::destroy();
	notifier::get()->remove_listener(this);
	game_controller::destroy();
}

void main_window::draw()
{
	m_window->clear(sf::Color(150, 150, 150, 255));
	m_objectives_pane->draw();
	m_canvas->draw();
	for (auto it : m_items) {
		if (it != nullptr) {
			m_window->draw(*it);
		}
	}
	m_window->display();
}

void main_window::draw_with_delay()
{
	sf::Clock clk;
	while (clk.getElapsedTime().asSeconds() < 0.25) {
		draw();
	}
}

void main_window::create_items()
{
	game_controller* gc = game_controller::get();
	const size_t rows = gc->get_rows();
	const size_t cols = gc->get_cols();
	size_t x = 0;
	size_t y = BOARD_OFFSET;
	for (int r = 0; r < rows; ++r) {
		for (size_t c = 0; c < cols; ++c) {
			board_item* bi = gc->get_board_item(index(r, c));
			assert(bi != nullptr);
			sf::Texture* t = texture_map::find_texture(bi);
			assert(t != nullptr);
			sf::Sprite* s = new sf::Sprite(*t);
			s->setOrigin(-5, -5);
			s->setPosition(x, y);
			m_items.push_back(s);
			x += ITEM_SIZE;
		}
		x = 0;
	}
}



void main_window::on_items_destroyed(const std::list<index>& destroyed_items_indexes)
{
	if (destroyed_items_indexes.empty()) {
		return;
	}
	draw_with_delay();
	for (auto it : destroyed_items_indexes) {
		size_t raw_index = get_raw_index(it);
		if (m_items[raw_index] != nullptr) {
			delete m_items[raw_index];
			m_items[raw_index] = nullptr;
		}
	}
	draw_with_delay();
}

void main_window::on_items_swapped(const index& i1, const index& i2)
{
	draw_with_delay();
	const size_t raw_index_1 = get_raw_index(i1);
	const size_t raw_index_2 = get_raw_index(i2);
	sf::Vector2f i1_pos = m_items[raw_index_1]->getPosition();
	sf::Vector2f i2_pos = m_items[raw_index_2]->getPosition();
	m_items[raw_index_1]->setPosition(i2_pos);
	m_items[raw_index_2]->setPosition(i1_pos);
	sf::Sprite* tmp = m_items[raw_index_1];
	m_items[raw_index_1] = m_items[raw_index_2];
	m_items[raw_index_2] = tmp;
	draw_with_delay();
}

void main_window::on_booster_created(booster* b, const index& i)
{
	draw_with_delay();
	assert(b != nullptr);
	const size_t raw_index = get_raw_index(i);
	if (m_items[raw_index] != nullptr) {
		//// XXX WRONG INDEX ???
		delete m_items[raw_index];
		m_items[raw_index] = nullptr;
		board_item* item = game_controller::get()->get_board_item(i);
		if (item == nullptr) {
			return;
		}
		sf::Texture* t = texture_map::find_texture(item);
		assert(t != nullptr);
		sf::Sprite* s = new sf::Sprite(*t);
		s->setOrigin(-5, -5);
		s->setPosition(i.column() * ITEM_SIZE, BOARD_OFFSET + ITEM_SIZE + ITEM_SIZE * i.row());
		m_items[raw_index] = s;
		draw_with_delay();
		return;
	}
	assert(m_items[raw_index] == nullptr);
	sf::Texture* t = texture_map::find_texture(b->get_type());
	assert(t != nullptr);
	sf::Sprite* s = new sf::Sprite(*t);
	s->setOrigin(-5, -5);
	s->setPosition(i.column() * ITEM_SIZE, BOARD_OFFSET + ITEM_SIZE + ITEM_SIZE * i.row());
	m_items[raw_index] = s;
	draw_with_delay();
}

void main_window::on_columns_dropped(const std::list<size_t>& columns)
{
	if (columns.empty()) {
		return;
	}
	draw_with_delay();
	const size_t rows = game_controller::get()->get_rows();
	const size_t cols = game_controller::get()->get_cols();
	size_t max_drop_count = 1;
	std::map<size_t, std::vector<std::pair<int, int>>> animation_data;
	for (size_t col : columns) {
		for (size_t r = 0; r < rows; ++r) {
			index current(r, col);
			const size_t ri = get_raw_index(current);
			if (m_items[ri] == nullptr) {
				continue;
			}
			size_t empty_count = 0;
			for (size_t i = r; i < rows; ++i) {
				index ix(i, col);
				const size_t raw_idx = get_raw_index(ix);
				if (m_items[raw_idx] == nullptr) {
					++empty_count;
				}
			}
			if (empty_count) {
				if (empty_count > max_drop_count) {
					max_drop_count = empty_count;
				}
				animation_data[col].push_back(std::make_pair(ri, empty_count * ITEM_SIZE));
			}
		}
	}
	if (animation_data.empty()) {
		return;
	}
	const float duration = max_drop_count * ROW_DURATION;
	std::vector<std::pair<sf::Sprite*, size_t>> tmp_data;
	for (auto it : animation_data) {
		for (auto i : it.second) {
			if (m_items[i.first] != nullptr) {
				tmp_data.push_back(std::make_pair(m_items[i.first], i.second));
			}
		}
	}
	sf::Clock clk;
	while (clk.getElapsedTime().asSeconds() < duration) {
		for (auto& it : tmp_data) {
			m_window->clear(sf::Color(150, 150, 150, 255));
			m_objectives_pane->draw();
			m_canvas->draw();
			const size_t y_distance = ITEM_SIZE / STEPS_COUNT;
			if (it.second >= y_distance) {

				it.first->move(0, y_distance);
				it.second -= y_distance;
			}
			for (auto it : m_items) {
				if (it != nullptr && it->getPosition().y >= BOARD_OFFSET + ITEM_SIZE) {
					m_window->draw(*it);
				}
			}
		}
		m_window->display();
	}
	//// Avoid clock dependency
	for (auto data : animation_data) {
		for (auto it : data.second) {
			index ix = get_index_from_raw(it.first);
			size_t y = ix.row() * ITEM_SIZE + ITEM_SIZE + BOARD_OFFSET + it.second;
			if (m_items[it.first] != nullptr) {
				if (m_items[it.first]->getPosition().y != y) {
					m_items[it.first]->setPosition(m_items[it.first]->getPosition().x, y);
				}
			}
		}
	}
	m_window->display();
	update_item_indexes(columns);
}

void main_window::update_item_indexes(const std::list<size_t>& columns)
{
	const size_t rows = game_controller::get()->get_rows();
	for (size_t col : columns) {
		for (int i = rows - 1; i >= 0; --i) {
			index current(i, col);
			if (!current.is_valid()) {
				continue;
			}
			const size_t ri = get_raw_index(current);
			if (m_items[ri] == nullptr) {
				continue;
			}
			size_t tmp = i;
			if (tmp == rows - 1) {
				continue;
			}
			++tmp;
			while (tmp < rows) {
				index next(tmp, col);
				if (!next.is_valid()) {
					++tmp;
					continue;
				}
				const size_t next_ri = get_raw_index(next);
				if (m_items[next_ri] == nullptr) {
					std::swap(m_items[get_raw_index(current)], m_items[get_raw_index(next)]);
					current = next;
					++tmp;
				}
				else {
					break;
				}
			}
		}
	}
}

void main_window::on_new_items_dropped(const std::list<std::pair<figure*, index>>& new_items)
{
	if (new_items.empty()) {
		return;
	}
	std::map<size_t, std::vector<std::pair<sf::Sprite*, size_t>>> animation_data;
	game_controller* gc = game_controller::get();
	assert(gc != nullptr);
	for (auto it : new_items) {
		board_item* bi = gc->get_board_item(it.second);
		assert(bi != nullptr);
		sf::Texture* t = texture_map::find_texture(bi);
		assert(t != nullptr);
		sf::Sprite* s = new sf::Sprite(*t);
		s->setOrigin(-5, -5);
		s->setPosition(it.second.column() * ITEM_SIZE, BOARD_OFFSET);
		m_items[get_raw_index(it.second)] = s;
		animation_data[it.second.row()].push_back(std::make_pair(s, (it.second.row() + 1) * ITEM_SIZE));
	}
	std::vector<std::pair<sf::Sprite*, size_t>> tmp_data;
	for (auto iter = animation_data.rbegin(); iter != animation_data.rend(); ++iter) {
		for (auto i : iter->second) {
			tmp_data.push_back(i);
		}
		sf::Clock clk;
		while (clk.getElapsedTime().asSeconds() < ROW_DURATION) {
			for (auto& it : tmp_data) {
				m_window->clear(sf::Color(150, 150, 150, 255));
				m_objectives_pane->draw();
				m_canvas->draw();
				const size_t y_distance = ITEM_SIZE / STEPS_COUNT;
				if (it.second >= y_distance) {
					it.first->move(0, y_distance);
					it.second -= y_distance;
				}
				for (auto it : m_items) {
					if (it != nullptr && it->getPosition().y >= BOARD_OFFSET + ITEM_SIZE) {
						m_window->draw(*it);
					}
				}
			}
			m_window->display();
		}
	}
	//// Avoid clock dependency
	for (auto it : new_items) {
			index ix = it.second;
			const size_t ri = get_raw_index(ix);
			size_t y = ix.row() * ITEM_SIZE + ITEM_SIZE + BOARD_OFFSET;
			sf::Sprite* s = m_items[ri];
			if (s != nullptr) {
				if (s->getPosition().y != y) {
					s->setPosition(s->getPosition().x, y);
				}
			}
	}
	draw_with_delay();
}

void main_window::on_shuffle()
{
	destroy_items();
	draw_with_delay();
	create_items();
	draw_items_with_animation();
}

sf::Texture* main_window::get_item_texture(const index& i) const noexcept
{
	board_item* bi = game_controller::get()->get_board_item(i);
	return texture_map::find_texture(bi);
}


void main_window::draw_items_with_animation()
{
	const size_t rows = game_controller::get()->get_rows();
	const size_t cols = game_controller::get()->get_cols();
	float row_duration = STEPS_COUNT * DELAY;
	std::vector<sf::Sprite*> animation_data;
	for (int r = rows - 1; r >= 0; --r) {
		for (size_t c = 0; c < cols; ++c) {
			animation_data.push_back(m_items[r * cols + c]);
		}
		sf::Clock clock;
		size_t frames = 0;
		while (clock.getElapsedTime().asSeconds() < row_duration) {
			++frames;
			if (frames > STEPS_COUNT) {
				m_window->clear(sf::Color(150, 150, 150, 255));
				m_objectives_pane->draw();
				m_canvas->draw();
				for (auto it : animation_data) {
					m_window->draw(*it);
				}
				m_window->display();
				break;
			}
			m_window->clear(sf::Color(150, 150, 150, 255));
			m_objectives_pane->draw();
			m_canvas->draw();
			for (auto it : animation_data) {
				it->move(0, ITEM_SIZE / STEPS_COUNT);
				if (it->getPosition().y >= BOARD_OFFSET + ITEM_SIZE) {
					m_window->draw(*it);
				}
			}
			m_window->display();
		}
	}
	m_window->display();
}


void main_window::destroy_items()
{
	if (m_items.empty()) {
		return;
	}
	for (auto it : m_items) {
		if (it != nullptr) {
			delete it;
			it = nullptr;
		}
	}
	m_items.clear();
}

void main_window::mouse_pressed(const index& i)
{
	assert(i.is_valid());
	game_controller::get()->process_selection(i);
}

int main_window::exec_event_loop()
{
	m_window->clear(sf::Color(150, 150, 150, 255));
	m_objectives_pane->draw();
	m_canvas->draw();
	draw_items_with_animation();
	m_window->display();

	while (m_window->isOpen()) {
		sf::Event event;
		while (m_window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				m_window->close();
				break;
			}
			bool need_update_window = false;
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button != sf::Mouse::Left) {
					continue;
				}
				if (game_controller::get()->get_game_status() == game_controller::game_status::in_progress) {
					index i = find_index(event.mouseButton.x, event.mouseButton.y);
					if (i.is_valid()) {
						need_update_window = true;
						mouse_pressed(i);
					}
				}
			}
			if (need_update_window) {
				draw();
			}
		}
	}
	return 0;
}

index main_window::find_index(int x, int y) const
{
	if (y - BOARD_OFFSET - ITEM_SIZE < 0) {
		return invalid_index;
	} else if (x < 0) {
		return invalid_index;
	}
	y -= BOARD_OFFSET + ITEM_SIZE;
	const size_t r = y / ITEM_SIZE;
	const size_t c = x / ITEM_SIZE;
	return index(r, c);
}

size_t main_window::get_raw_index(const index& i) const noexcept
{
	assert(i.is_valid());
	return i.row() * game_controller::get()->get_cols() + i.column();
}

index main_window::get_index_from_raw(size_t raw_index) const noexcept
{
	assert(raw_index < m_items.size());
	game_controller* gc = game_controller::get();
	size_t r = raw_index / gc->get_cols();
	size_t c = raw_index % gc->get_cols();
	return index(r, c);
}

} //// gui namespace
