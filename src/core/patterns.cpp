
#include "match_data.hpp"
#include "patterns.hpp"

#include <cassert>
#include <iostream>


/// pattern
bool pattern::is_figure(const board::ptr& b, const index& i) const noexcept
{
	assert(b != nullptr);
	if (!i.is_valid()) {
		return false;
	}
	board_item* item = b->get_item(i);
	return item != nullptr ? item->is_figure() : false;
}

figure::color pattern::get_color(const board::ptr& b, const index& i) const
{
    if (!i.is_valid()) {
        return figure::color::UNDEFINED;
    }
	if (b->is_proxy_item(i)) {
		return b->get_proxy_color(i);
	}
	if (!is_figure(b, i)) {
		return figure::color::UNDEFINED;
	}
	return static_cast<figure*>(b->get_item(i))->get_color();
}


/// vertical pattern
bool horizontal_pattern::match(const board::ptr& b, const index& i, match_data& md)
{
    assert(i.is_valid());
	if (!is_figure(b, i)) {
		return false;
	}
    const figure::color& c = get_color(b, i);
	return match(b, i, c, md);
}

bool horizontal_pattern::match(const board::ptr& b, const proxy_figure* f)
{
	assert(b);
	assert(f != nullptr);
	match_data md;
	return match(b, f->get_index(), f->get_color(), md);
}

pattern::type horizontal_pattern::get_type() const noexcept
{
    return pattern::type::horizontal;
}

bool horizontal_pattern::match(const board::ptr& b, const index& i, const figure::color& c, match_data& md)
{
	assert(b);
	assert(i.is_valid());
	assert(c != figure::color::UNDEFINED);
	size_t it = i.column();
	std::list<index> mi;
	mi.push_back(i);
	while (get_color(b, index(i.row(), --it)) == c) {
		mi.push_back(index(i.row(), it));
	}
	it = i.column();
	while (get_color(b, index(i.row(), ++it)) == c) {
		mi.push_back(index(i.row(), it));
	}
	const size_t count = mi.size();
	const bool matched = count > 2;
	if (matched) {
		md.set_indexes(mi);
		md.set_pattern_type(pattern::type::horizontal);
	}
	return matched;
}


/// horizontal pattern
bool vertical_pattern::match(const board::ptr& b, const index& i, match_data& md)
{
    assert(i.is_valid());
	if (!is_figure(b, i)) {
		return false;
	}
    const figure::color& c = get_color(b, i);
	return match(b, i, c, md);
}

bool vertical_pattern::match(const board::ptr& b, const proxy_figure* f)
{
	assert(b);
	assert(f != nullptr);
	match_data md;
	return match(b, f->get_index(), f->get_color(), md);
}


pattern::type vertical_pattern::get_type() const noexcept
{
    return pattern::type::vertical;
}

bool vertical_pattern::match(const board::ptr& b, const index& i, const figure::color& c, match_data& md)
{
	assert(b);
	assert(i.is_valid());
	assert(c != figure::color::UNDEFINED);
	size_t it = i.row();
	std::list<index> mi;
	mi.push_back(i);
	while (get_color(b, index(--it, i.column())) == c) {
		mi.push_back(index(it, i.column()));
	}
	it = i.row();
	while (get_color(b, index(++it, i.column())) == c) {
		mi.push_back(index(it, i.column()));
	}
	const size_t count = mi.size();
	const bool matched = count > 2;
	if (matched) {
		md.set_indexes(mi);
		md.set_pattern_type(pattern::type::vertical);
	}
	return matched;
}


/// radial pattern
bool radial_pattern::match(const board::ptr& b, const index& i, match_data& md)
{
    assert(i.is_valid());
	if (!is_figure(b, i)) {
		return false;
	}
    const figure::color& c = get_color(b, i);
	return match(b, i, c, md);
}

bool radial_pattern::match(const board::ptr& b, const proxy_figure* f)
{
	assert(b);
	assert(f);
	match_data md;
	return match(b, f->get_index(), f->get_color(), md);
}

bool radial_pattern::match(const board::ptr& b, const index& i, const figure::color& c, match_data& md)
{
	assert(b);
	assert(i.is_valid());
	assert(c != figure::color::UNDEFINED);
	std::list<index> mi;
	index neighbour = index(i.row(), i.column() - 1);
	if (_match(b, c, i, neighbour, mi)) {
		md.set_indexes(mi);
		md.set_pattern_type(pattern::type::radial);
		return true;
	}
	mi.clear();
	neighbour = index(i.row(), i.column() + 1);
	if (_match(b, c, i, neighbour, mi)) {
		md.set_indexes(mi);
		md.set_pattern_type(pattern::type::radial);
		return true;
	}
	return false;
}

pattern::type radial_pattern::get_type() const noexcept
{
    return pattern::type::radial;
}

bool radial_pattern::_match(const board::ptr& b, const figure::color& c, const index& i, const index& n, std::list<index>& mi)
{
    if (get_color(b, n) == c) {
        if (match_top(b, c, i, n, mi)) {
            return true;
        } else if (match_bottom(b, c, i, n, mi)) {
            return true;
        }
    }
    return false;
}

bool radial_pattern::match_top(const board::ptr& b, const figure::color& c, const index& i, const index& n, std::list<index>& mi)
{
    index i1(i.row() - 1, i.column());
    index i2(n.row() - 1, n.column());
    const bool matched = get_color(b, i1) == c && get_color(b, i2) == c;
    if (matched) {
        mi = { i, n, i1, i2 };
    }
    return matched;
}

bool radial_pattern::match_bottom(const board::ptr& b, const figure::color& c, const index& i, const index& n, std::list<index>& mi)
{
    index i1(i.row() + 1, i.column());
    index i2(n.row() + 1, n.column());
    const bool matched = get_color(b, i1) == c && get_color(b, i2) == c;
    if (matched) {
        mi = { i, n, i1, i2 };
    }
    return matched;
}


/// T patern
bool t_pattern::match(const board::ptr& b, const index& i, match_data& md)
{
    assert(i.is_valid());
	if (!is_figure(b, i)) {
		return false;
	}
    const figure::color c = get_color(b, i);
	assert(c != figure::color::UNDEFINED);
	std::list<index> tmp;
    if (match_t_base_horizontal(b, c, i, tmp)) {
        if (match_t_tails_vertical(b, c, i, tmp)) {
            md.set_indexes(tmp);
            md.set_pattern_type(pattern::type::t);
            return true;
        }
    }
    tmp.clear();
    if (match_t_base_vertical(b, c, i, tmp)) {
        if (match_t_tails_horizontal(b, c, i, tmp)) {
            md.set_indexes(tmp);
            md.set_pattern_type(pattern::type::t);
            return true;
        }
    }
    return false;
}

bool t_pattern::match(const board::ptr&, const proxy_figure*)
{
	return false;
}

pattern::type t_pattern::get_type() const noexcept
{
    return pattern::type::t;
}

bool t_pattern::match_t_base_vertical(const board::ptr& b, const figure::color& c, const index& i, std::list<index>& mi)
{
    index ti1(i.row() - 1, i.column());
    index ti2(i.row() - 2, i.column());
    if (get_color(b, ti1) == c && get_color(b, ti2) == c) {
        mi.push_back(ti1);
        mi.push_back(ti2);
        return true;
    }
    index bi1(i.row() + 1, i.column());
    index bi2(i.row() + 2, i.column());
    if (get_color(b, bi1) == c && get_color(b, bi2) == c) {
        mi.push_back(bi1);
        mi.push_back(bi2);
        return true;
    }
    return false;
}

bool t_pattern::match_t_base_horizontal(const board::ptr& b, const figure::color& c, const index& i, std::list<index>& mi)
{
    index ti1(i.row(), i.column() - 1);
    index ti2(i.row(), i.column() - 2);
    if (get_color(b, ti1) == c && get_color(b, ti2) == c) {
        mi.push_back(ti1);
        mi.push_back(ti2);
        return true;
    }
    index bi1(i.row(), i.column() + 1);
    index bi2(i.row(), i.column() + 2);
    if (get_color(b, bi1) == c && get_color(b, bi2) == c) {
        mi.push_back(bi1);
        mi.push_back(bi2);
        return true;
    }
    return false;
}

bool t_pattern::match_t_tails_vertical(const board::ptr& b, const figure::color& c, const index& i, std::list<index>& mi)
{
    index i1(i.row() - 1, i.column());
    index i2(i.row() + 1, i.column());
    const bool matched = get_color(b, i1) == c && get_color(b, i2) == c;
    if (matched) {
        mi.push_back(i1);
        mi.push_back(i2);
    }
    return matched;
}

bool t_pattern::match_t_tails_horizontal(const board::ptr& b, const figure::color& c, const index& i, std::list<index>& mi)
{
    index i1(i.row(), i.column() - 1);
    index i2(i.row(), i.column() + 1);
    const bool matched = get_color(b, i1) == c && get_color(b, i2) == c;
    if (matched) {
        mi.push_back(i1);
        mi.push_back(i2);
    }
    return matched;
}
