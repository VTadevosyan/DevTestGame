
#include "objective.hpp"

#include <cassert>


objective::objective(figure::color f, size_t c)
    : m_color(f)
    , m_count(c)
{
    assert(m_count);
}

figure::color objective::get_color() const noexcept
{
    return m_color;
}

size_t objective::get_count() const noexcept
{
    return m_count;
}

bool objective::is_valid() const noexcept
{
    return m_color != figure::color::UNDEFINED;
}

bool objective::completed() const noexcept
{
    assert(is_valid());
    return !m_count;
}

void objective::decrease_count(size_t c) noexcept
{
    if (!c) {
        return;
    }
    m_count -= c > m_count ? m_count : c;
}
