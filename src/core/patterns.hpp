
#ifndef PATTERNS_HPP
#define PATTERNS_HPP

#include "board.hpp"
#include "figure.hpp"
#include "index.hpp"
#include "proxy_figure.hpp"

#include <list>
#include <memory>


class match_data;


//// @class pattern
//// @brief Interface of patterns
class pattern
{
public:
    enum class type {
        UNDEFINED = -1,
        horizontal = 0,
        vertical,
        radial,
        t
    };

public:
	//// @brief Constructor
    pattern() = default;

	//// @brief Destructor
    virtual ~pattern() = default;

public:
	//// @brief Match is a predefined combination of same color figures.
	//// @return true if found concrete combination - false otherwise
	//// @note match() will return false if matched colors count is less or equal 2
    virtual bool match(const board::ptr&, const index&, match_data&) = 0;

	//// @brief Match is a predefined combination of same color figures.
	////		This function uses proxy figure for matching pattern.
	////		It means that current index and color will be used from proxy figure and not get from board.
	//// @return true if found a concrete combination - false otherwise
	//// @note match() will return false if matched colors count is less or equal 2
	//// @note For just checking available moves count no need to return match data if don't need to hint something!
	virtual bool match(const board::ptr&, const proxy_figure*) = 0;

	//// @brief Gets the pattern type
    virtual type get_type() const noexcept = 0;

protected:
	//// @brief Gets the figure's color from given index
    figure::color get_color(const board::ptr&, const index&) const;

	//// @brief Checks if item with given index is figure or not(not empty or booster)
	bool is_figure(const board::ptr&, const index&) const noexcept;
};


//// @class horizontal_pattern
class horizontal_pattern : public pattern
{
public:
	//// @brief Horizontal combination of same color figures.
	//// @return true if found horizontal combination
    bool match(const board::ptr&, const index&, match_data&) override;

	//// @brief Horizontal combination of same color figures.
	//// @return true if found horizontal combination
	//// @note Starting figure is a proxy figure
	bool match(const board::ptr&, const proxy_figure*) override;

	//// @brief Gets the pattern type
    pattern::type get_type() const noexcept override;

private:
	bool match(const board::ptr&, const index&, const figure::color&, match_data&);

};


//// @class vertical_pattern
class vertical_pattern : public pattern
{
public:
	//// @brief Vertical combination of same color figures.
	//// @return true if found vertical combination
    bool match(const board::ptr&, const index&, match_data&) override;
	
	//// @brief Vertical combination of same color figures.
	//// @return true if found vertical combination
	//// @note Starting figure is a proxy figure
	bool match(const board::ptr&, const proxy_figure*) override;

	//// @brief Gets the pattern type
    pattern::type get_type() const noexcept override;

private:
	bool match(const board::ptr&, const index&, const figure::color&, match_data&);

};


//// @class vertical_pattern
class radial_pattern : public pattern
{
public:
	//// @brief Radial combination of same color figures.
	//// @return true if found radial(squared) combination
    bool match(const board::ptr&, const index&, match_data&) override;

	//// @brief Radial combination of same color figures.
	//// @return true if found radial(squared) combination
	//// @note Starting figure is a proxy figure
	bool match(const board::ptr&, const proxy_figure*) override;

    /// @brief Gets the pattern type
    pattern::type get_type() const noexcept override;

private:
	bool match(const board::ptr&, const index&, const figure::color&, match_data&);
    bool _match(const board::ptr&, const figure::color&, const index&, const index&, std::list<index>&);
    bool match_top(const board::ptr&, const figure::color&, const index&, const index&, std::list<index>&);
    bool match_bottom(const board::ptr&, const figure::color&, const index&, const index&, std::list<index>&);

};


//// @class t_pattern
class t_pattern : public pattern
{
public:
	//// @brief Vertical combination of same color figures.
	//// @return true if found vertical combination
    bool match(const board::ptr&, const index&, match_data&) override;

	//// Don't need to match this pattern in proxy matching
	//// This is because it contains vertical and horizontal patterns
	bool match(const board::ptr&, const proxy_figure*) override;

	//// @brief Gets the pattern type
    pattern::type get_type() const noexcept override;

private:
    bool match_t_base_vertical(const board::ptr&, const figure::color&, const index&, std::list<index>&);
    bool match_t_base_horizontal(const board::ptr&, const figure::color&, const index&, std::list<index>&);
    bool match_t_tails_vertical(const board::ptr&, const figure::color&, const index&, std::list<index>&);
    bool match_t_tails_horizontal(const board::ptr&, const figure::color&, const index&, std::list<index>&);

};

#endif // PATTERNS_HPP
