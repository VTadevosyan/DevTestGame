
#include "board.hpp"
#include "exceptions.hpp"
#include "config.hpp"

#include <iostream>


/// base_exception
base_exception::base_exception(const char* msg)
    : m_err_msg(msg)
{
}

base_exception::base_exception(const std::string& msg)
    : m_err_msg(msg.c_str())
{
}

/*base_exception::base_exception(const base_exception& e)
    : m_err_msg(e.m_err_msg)
{
}*/

const char* base_exception::what() const noexcept
{
    return m_err_msg;
}


/// positive_integer_error
positive_integer_error::positive_integer_error()
    : base_exception("The specified value must be a positive integer")
{
}


/// json_error
json_error::json_error(const char* msg)
    : base_exception(msg)
{
}


/// board_size_error
board_size_error::board_size_error()
    : base_exception("The value must be between " + std::to_string(board::min_size()) + " and " + std::to_string(board::max_size()))
{
}


/// moves_count_error
moves_count_error::moves_count_error()
    : base_exception("The moves count must be positive integer")
{
}


/// figures_count_error
figures_count_error::figures_count_error()
    : base_exception("The figures count must be between " + std::to_string(config::MIN_FIGURE_COLORS_COUNT) + " and " + std::to_string(config::MAX_FIGURE_COLORS_COUNT))
{
}


/// figures_color_error
figure_color_error::figure_color_error()
    : base_exception("Invalid figure color specified")
{
}


/// objectives_count_error
objectives_count_error::objectives_count_error()
    : base_exception("The objectives count must be between 1 and 3")
{
}
