
#ifndef MATCHER_HPP
#define MATCHER_HPP

#include "board.hpp"
#include "index.hpp"
#include "patterns.hpp"
#include "proxy_figure.hpp"

#include <list>
#include <memory>
#include <vector>


class match_data;


//// @class matcher
//// @brief This is pattern matcher
//// Matcher will start find matched patterns by calling match() function (or proxy_match)
//// The matched result will be hold in match_data structure
class matcher
{
public:
    using ptr = std::unique_ptr<matcher>;

public:
    struct data
    {
        pattern::type type;
        std::list<index> indexes;
    };

public:
	//// @brief Constructor
    matcher() = default;

	//// @brief Destructor
    ~matcher() = default;

public:
	//// @brief Matches pattern combinations with priority
	//// Collects the matched indexes, pattern type information
    bool match(const board::ptr&, const index&, match_data&);

	//// @brief Matches pattern combinations with (optimized) priority
	//// @note proxy_match() creates proxy figures on board from specified indexes
	//// @note This function won't collect match data
	bool proxy_match(const board::ptr&, const index&, const index&);

	//// @brief Add a new pattern
	//// @note Pattern should be added by priority
	//// E.g. First added pattern will be matched first then others
    void add_pattern(pattern*);

private:
    std::vector<pattern*> m_patterns;
    std::list<index> m_matched_indexes;
};

#endif // MATCHER_HPP
