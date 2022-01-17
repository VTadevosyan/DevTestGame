
#include "figure.hpp"
#include "notifier.hpp"


figure::figure(color c)
    : board_item()
    , m_color(c)
{
}

figure::~figure()
{
	notifier::get()->on_figure_destroyed(this);
}

figure::color figure::get_color() const noexcept
{
    return m_color;
}

bool figure::is_figure() const noexcept
{
	return true;
}

bool figure::is_booster() const noexcept
{
	return false;
}

std::string figure::color2str(const figure::color& c) noexcept
{
    switch (c) {
        case color::blue:
            return "Blue";
        case color::green:
            return "Green";
        case color::orange:
            return "Orange";
        case color::red:
            return "Red";
        case color::violet:
            return "Violet";
        case color::UNDEFINED:
        default:;
    }
    return std::string();
}

/// @brief Gets the figure's color from given color name
figure::color figure::str2color(const std::string& s) noexcept
{
    if (s == "Blue") {
        return color::blue;
    } else if (s == "Green") {
        return color::green;
    } else if (s == "Orange") {
        return color::orange;
    } else if (s == "Red") {
        return color::red;
    } else if (s == "Violet") {
        return color::violet;
    }
    return color::UNDEFINED;
}

bool figure::is_valid() const noexcept
{
    return m_color != color::UNDEFINED;
}

