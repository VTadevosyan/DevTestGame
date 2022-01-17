
#include "match_data.hpp"

#include <cassert>


pattern::type match_data::get_pattern_type() const noexcept
{
    assert(m_pattern_type != pattern::type::UNDEFINED);
    return m_pattern_type;
}

void match_data::get_indexes(std::list<index>& indexes) const noexcept
{
    indexes = m_indexes;
}

void match_data::reset() noexcept
{
    m_indexes.clear();
    m_pattern_type = pattern::type::UNDEFINED;
}

void match_data::set_pattern_type(const pattern::type& t) noexcept
{
    assert(t != pattern::type::UNDEFINED);
    m_pattern_type = t;
}

void match_data::set_indexes(const std::list<index>& i) noexcept
{
    m_indexes = i;
}
