
#include "board.hpp"
#include "boosters.hpp"
#include "board_item.hpp"
#include "figure.hpp"
#include "notifier.hpp"
#include "proxy_figure.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>



size_t board::min_size() noexcept
{
    return 7;
}


size_t board::max_size() noexcept
{
    return 10;
}

board::board(size_t r, size_t c)
    : m_rows(r)
    , m_cols(c)
{
    m_data = std::vector<board_item*>(m_rows * m_cols, nullptr);
}

board::~board()
{
    for (auto it : m_data) {
        if (it != nullptr) {
            delete it;
            it = nullptr;
        }
    }
}

size_t board::rows() const noexcept
{
    return m_rows;
}

size_t board::columns() const noexcept
{
    return m_cols;
}

size_t board::get_raw_index(const index& i) const noexcept
{
    assert(i.is_valid());
    return i.row() * m_cols + i.column();
}

index board::get_index_from_raw(size_t raw_index) const noexcept
{
	assert(raw_index < m_data.size());
	size_t r = raw_index / m_cols;
	size_t c = raw_index % m_cols;
	return index(r, c);
}

void board::add_item(board_item* item, const index& i)
{
    assert(item != nullptr);
    const size_t ri = get_raw_index(i);
    assert(ri < m_data.size());
    if (m_data[ri] != nullptr) {
        delete m_data[ri];
    }
    m_data[ri] = item;
}

void board::destroy_item(const index& i)
{
    if (!i.is_valid()) {
        return;
    }
    const size_t ri = get_raw_index(i);
    if (m_data[ri] != nullptr) {
        delete m_data[ri];
        m_data[ri] = nullptr;
    }
}

void board::drop_column(const size_t col_index)
{
	std::list<index> di;
	drop_column(col_index, di);
}

void board::drop_column(const size_t col_index, std::list<index>& dropped_indexes)
{
	std::list<std::pair<index, index>> swap_indexes;
	for (int i = m_rows - 1; i >= 0; --i) {
		index current(i, col_index);
		if (!current.is_valid()) {
			continue;
		}
		const size_t ri = get_raw_index(current);
		if (m_data[ri] == nullptr) {
			continue;
		}
		size_t tmp = i;
		if (tmp == m_rows - 1) {
			continue;
		}
		++tmp;
		while (tmp < m_rows) {
			index next(tmp, col_index);
			if (!next.is_valid()) {
				++tmp;
				continue;
			}
			const size_t next_ri = get_raw_index(next);

			if (m_data[next_ri] == nullptr) {
				swap_indexes.push_back(std::make_pair(current, next));
				swap(current, next);
				dropped_indexes.push_back(next);
				current = next;
				++tmp;
			}
			else {
				break;
			}
		}
	}
}

void board::swap(const index& i1, const index& i2)
{
	assert(can_swap(i1, i2));
    const size_t ri1 = get_raw_index(i1);
    const size_t ri2 = get_raw_index(i2);
    board_item* tmp = m_data[ri1];
    m_data[ri1] = m_data[ri2];
    m_data[ri2] = tmp;
}

bool board::can_swap(const index& i1, const index& i2) const noexcept
{
	if (i1 == i2) {
		return false;
	}
    return is_neighbours(i1, i2);
}

bool board::is_neighbours(const index& i1, const index& i2) const noexcept
{
    if (!i1.is_valid() || !i2.is_valid()) {
        return false;
    }
    if (i1.row() == i2.row()) {
        const size_t col1 = i1.column();
        const size_t col2 = i2.column();
        return col1 + 1 == col2 || col1 - 1 == col2;
    } else if (i1.column() == i2.column()) {
        const size_t row1 = i1.row();
        const size_t row2 = i2.row();
        return row1 + 1 == row2 || row1 - 1 == row2;
    }
    return false;
}

board_item* board::get_item(const index& i) const noexcept
{
    if (!i.is_valid()) {
        return nullptr;
    }
    const size_t raw_index = get_raw_index(i);
    assert(raw_index < m_data.size());
    return m_data[raw_index];
}

void board::get_empty_tiles(std::list<index>& l) const
{
	for (size_t i = 0; i < m_data.size(); ++i) {
		if (m_data[i] == nullptr) {
			l.push_back(std::move(get_index_from_raw(i)));
		}
	}
}


void board::add_proxy_figure(const proxy_figure* f) noexcept
{
	assert(f != nullptr);
	m_proxy_items.insert(std::make_pair(f->get_index(), f->get_color()));
}

void board::remove_proxy_figure(const proxy_figure* f) noexcept
{
	assert(f != nullptr);
	m_proxy_items.erase(f->get_index());
}

bool board::is_proxy_item(const index& i) const noexcept
{
	if (m_proxy_items.empty()) {
		return false;
	}
	if (!i.is_valid()) {
		return false;
	}
	return m_proxy_items.find(i) != m_proxy_items.end();
}

figure::color board::get_proxy_color(const index& i) const noexcept
{
	if (!is_proxy_item(i)) {
		return figure::color::UNDEFINED;
	}
	try {
		return m_proxy_items.at(i);
	} catch (const std::out_of_range&) {
	}
	return figure::color::UNDEFINED;
}

void board::shuffle()
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(m_data.begin(), m_data.end(), g);
}


/// XXX
namespace  {

char get_c(board_item* i) {
    figure* f = dynamic_cast<figure*>(i);
    if (f) {
        switch (f->get_color()) {
            case figure::color::blue: return 'B';
            case figure::color::green: return 'G';
            case figure::color::orange: return 'O';
            case figure::color::red: return 'R';
            case figure::color::violet: return 'V';
            case figure::color::UNDEFINED:
            default:;
        }
    }
    booster* b = dynamic_cast<booster*>(i);
    if (b != nullptr) {
        switch (b->get_type()) {
            case booster::type::horizontal:
                return '-';
            case booster::type::vertical:
                return '|';
            case booster::type::radial:
                return '*';
        }
    }
    return 0;
}
}

void board::draw()
{
    std::cout << __FUNCSIG__ << std::endl;
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            if (m_data[i * m_cols + j] == nullptr) {
                std::cout << "  ";
                continue;
            }
            std::cout << get_c(m_data[i * m_cols + j]) << " ";
        } std::cout << std::endl;
    }
}
