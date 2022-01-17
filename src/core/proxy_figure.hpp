
#ifndef PROXY_FIGURE_HPP
#define PROXY_FIGURE_HPP

#include "figure.hpp"
#include "index.hpp"

#include <cassert>


//// @class proxy_figure
//// @brief Proxy figure Definition
class proxy_figure
{
public:
	//// @brief Constructs a new proxy figure
	//// @note Specified color should be a valid color
	//// @note Specified index should be a valid index
	explicit proxy_figure(const figure::color& c, const index& i) noexcept
		: m_color(c)
		, m_index(i)
	{
		assert(c != figure::color::UNDEFINED);
		assert(i.is_valid());
	}

	//// @brief Destructor
	virtual ~proxy_figure() = default;

public:
	//// @brief Gets the proxy figure's color
	inline figure::color get_color() const noexcept
	{
		return m_color;
	}

	//// @brief Gets the proxy figure's index
	inline index get_index() const noexcept
	{
		return m_index;
	}

private:
	figure::color m_color;
	index m_index;

};

#endif /// PROXY_FIGURE_HPP
