
#ifndef FIGURE_HPP
#define FIGURE_HPP

#include "board_item.hpp"

#include <string>
#include <vector>


//// @class figure
//// @brief Figure is a type of board item
//// It should has a color which can't be changed after creation
class figure : public board_item
{
public:
	//// @enum color
	//// @brief Figures colors enumeration
    enum class color
    {
        UNDEFINED = -1,
        blue = 0,
        green,
        orange,
        red,
        violet
    };

    using colors_t = std::vector<color>;

public:
	//// @brief Gets the figure color's name
    static std::string color2str(const figure::color&) noexcept;

	//// @brief Gets the figure's color from given color name
    static color str2color(const std::string&) noexcept;

public:
	//// @brief Constructor
    explicit figure(const color);

	//// @brief Destructor
    virtual ~figure();

public:
	//// @brief Gets the figure's color
    color get_color() const noexcept;

	//// @brief Checks the figure's validity: color is specified or not
    bool is_valid() const noexcept;

public:
	//// @brief Checks the board item type is figure or not
	bool is_figure() const noexcept override;

	//// @brief Checks the board item type is booster or not
	bool is_booster() const noexcept override;

private:
    color m_color;

};

#endif /// FIGURE_HPP
