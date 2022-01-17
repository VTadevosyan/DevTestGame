
#include "boosters.hpp"

#include <cassert>
#include <list>
#include <iostream>


bool booster::is_figure() const noexcept
{
	return false;
}

bool booster::is_booster() const noexcept
{
	return true;
}

/// horizontal bomb (booster)
horizontal_bomb::horizontal_bomb() noexcept
{
}

void horizontal_bomb::activate(const board::ptr& b, const index& i, std::set<index>& impact_areas)
{
    assert(b);
	assert(i.is_valid());
	std::list<index> impact_indexes;
	impact_areas.insert(i);
	b->destroy_item(i);
	for (size_t c = 0; c < b->columns(); ++c) {
		index ix(i.row(), c);
		if (!ix.is_valid() || i == ix) {
			continue;
		}
		impact_areas.insert(ix);
		board_item* item = b->get_item(ix);
		if (item != nullptr && item->is_booster()) {
			assert(dynamic_cast<booster*>(item));
			static_cast<booster*>(item)->activate(b, ix, impact_areas);
			continue;
		}
		b->destroy_item(ix);
	}
}

booster::type horizontal_bomb::get_type() const noexcept
{
    return booster::type::horizontal;
}


/// vertical bomb (booster)
vertical_bomb::vertical_bomb() noexcept
{
}

void vertical_bomb::activate(const board::ptr& b, const index& i, std::set<index>& impact_areas)
{
    assert(b);
	assert(i.is_valid());
	impact_areas.insert(i);
	b->destroy_item(i);
	for (size_t r = 0; r < b->rows(); ++r) {
		index ix(r, i.column());
		if (!ix.is_valid() || i == ix) {
			continue;
		}
		impact_areas.insert(ix);
		board_item* item = b->get_item(ix);
		if (item != nullptr && item->is_booster()) {
			assert(dynamic_cast<booster*>(item));
			static_cast<booster*>(item)->activate(b, ix, impact_areas);
			continue;
		}
		b->destroy_item(ix);
	}
}

booster::type vertical_bomb::get_type() const noexcept
{
    return booster::type::vertical;
}


/// radial bomb (booster)
radial_bomb::radial_bomb() noexcept
{
}

void radial_bomb::activate(const board::ptr& b, const index& i, std::set<index>& impact_areas)
{
    assert(b);
    std::list<index> impact_indexes = {
        index(i.row() - 1, i.column()),
        index(i.row() + 1, i.column()),
        index(i.row(), i.column() - 1),
        index(i.row(), i.column() + 1),
        index(i.row() - 1, i.column() - 1),
        index(i.row() + 1, i.column() + 1),
        index(i.row() + 1, i.column() - 1),
        index(i.row() - 1, i.column() + 1),
    };
	impact_areas.insert(i);
	b->destroy_item(i);
    for (auto it : impact_indexes) {
        if (it.is_valid()) {
			impact_areas.insert(it);
			board_item* item = b->get_item(it);
			if (item != nullptr && item->is_booster()) {
				assert(dynamic_cast<booster*>(item));
				static_cast<booster*>(item)->activate(b, it, impact_areas);
				continue;
			}
            b->destroy_item(it);
        }
    }
}

booster::type radial_bomb::get_type() const noexcept
{
    return booster::type::radial;
}

