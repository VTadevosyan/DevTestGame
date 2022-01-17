
#include "board.hpp"
#include "config.hpp"
#include "game_controller.hpp"
#include "index.hpp"

#include <cassert>



index::index(size_t r, size_t c) noexcept
    : m_row(r)
    , m_col(c)
{
}

index::index(const index& i) noexcept
    : m_row(i.m_row)
    , m_col(i.m_col)
{
}


bool index::operator== (const index& i) const noexcept
{
	return m_row == i.row() && m_col == i.column();
}

bool index::operator!= (const index& i) const noexcept
{
	return !this->operator==(i);
}

bool index::operator< (const index& i) const noexcept
{
	if (m_row < i.row()) {
		return true;
	} else if (m_row == i.row()) {
		return m_col < i.column();
	}
	return false;
}

size_t index::row() const noexcept
{
    return m_row;
}

/// @brief Gets the column's index
size_t index::column() const noexcept
{
    return m_col;
}

/// @brief Checks the index validity
bool index::is_valid() const noexcept
{
    game_controller* c = game_controller::get();
    assert(c != nullptr);
    return m_row < c->get_rows() && m_col < c->get_cols();
}
