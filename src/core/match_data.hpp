
#ifndef MATCH_DATA_HPP
#define MATCH_DATA_HPP

#include "index.hpp"
#include "patterns.hpp"

#include <list>


//// @class match_data
//// @brief Data holder class for keeping matching information
//// E.g The matched pattern type and matched indexes
class match_data
{
public:
	//// @brief Constructor
    match_data() = default;

	//// @brief Destructor
    virtual ~match_data() = default;

public:
	//// @brief Gets the matched pattern type
    pattern::type get_pattern_type() const noexcept;

	//// @brief Gets the matched indexes
    void get_indexes(std::list<index>&) const noexcept;

	//// @brief Gets the matched pattern type
    void set_pattern_type(const pattern::type&) noexcept;

	//// @brief Sets the matched indexes
    void set_indexes(const std::list<index>&) noexcept;

	//// @brief Resest the match data
	void reset() noexcept;

private:
    std::list<index> m_indexes;
    pattern::type m_pattern_type;

};


#endif // MATCH_DATA_HPP


