
#ifndef OBJECTIVE_HPP
#define OBJECTIVE_HPP

#include "figure.hpp"

#include <vector>


class objective;

using objectives_t = std::vector<objective*>;


//// @class objective
//// @brief Objective is used to count the specified figures
//// Objective will be considered as completed if user collects specified objects with given count
//// Game couldn't be passed if there are any objective which is not completed
class objective
{
public:
	//// @brief Constructor
    objective(figure::color, size_t);

	//// @brief Destructor
    virtual ~objective() = default;

public:
	//// @brief Checks if current objective is completed or not
    bool completed() const noexcept;

	//// @brief Gets the figure
    figure::color get_color() const noexcept;

	//// @brief Gets the count of figures which need to collect to pass the objective
    size_t get_count() const noexcept;

	//// @brief Decreases the count of current objective
    void decrease_count(size_t = 1) noexcept;

	//// @brief Checks if current objective is valid or not
    bool is_valid() const noexcept;

private:
    figure::color m_color = figure::color::UNDEFINED;
    size_t m_count = 0;
};

#endif /// OBJECTIVE_HPP
