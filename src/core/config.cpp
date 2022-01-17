
#include "board.hpp"
#include "config.hpp"
#include "exceptions.hpp"
#include "objectives.hpp"

#include <boost/property_tree/json_parser.hpp>

#include <iostream>


const size_t config::MIN_FIGURE_COLORS_COUNT = 3;
const size_t config::MAX_FIGURE_COLORS_COUNT = 5;

void config::load(const std::string& f)
{
    try {
        init(f);
    } catch(base_exception const& e) {
        throw e;
    } catch(...) {
        throw std::runtime_error("Unknown Error");
    }
}

bool config::is_valid() const noexcept
{
    bool b = is_valid_board_size();
    b &= is_valid_moves_count();
    b &= is_valid_figures_count();
    b &= is_valid_objectives();
    return b;
}

size_t config::get_rows() const noexcept
{
    return m_board_size.rows;
}

size_t config::get_cols() const noexcept
{
    return m_board_size.cols;
}

size_t config::get_moves_count() const noexcept
{
    return m_moves_count;
}

size_t config::get_figures_count() const noexcept
{
    return m_figure_colors_count;
}

void config::get_objectives(objectives_t& o) const noexcept
{
    o = m_objectives;
}

void config::init(const std::string& f)
{
    boost::property_tree::ptree ptree;
    try {
        boost::property_tree::read_json(f, ptree);
    }  catch (boost::property_tree::json_parser::json_parser_error const& e) {
        throw json_error(static_cast<const char*>(e.what()));
    }
    load_board_size(ptree);
	load_figures_colors_count(ptree);
    load_objectives(ptree);
    load_moves_count(ptree);
}

void config::load_board_size(const boost::property_tree::ptree& ptree)
{
    static const std::string s_rows = "Board.Rows";
    static const std::string s_cols = "Board.Cols";
    const std::string& r = ptree.get_child(s_rows).data();
    m_board_size.rows = str2uint(r);
    const std::string& c = ptree.get_child(s_cols).data();
    m_board_size.cols = str2uint(c);
    if (!is_valid_board_size()) {
        throw board_size_error();
    }
}

void config::load_objectives(const boost::property_tree::ptree& ptree)
{
    static const std::string s_objectives = "Objectives";
    boost::property_tree::ptree objs = ptree.get_child(s_objectives);
    for (const auto& it : objs) {
        figure::color f = figure::str2color(it.first.data());
        if (!is_valid_color(f) || (size_t)f >= m_figure_colors_count) {
            throw figure_color_error();
        }
        const size_t c = str2uint(it.second.data());
        if (!is_valid_objective_figures_count(c)) {
            throw positive_integer_error();
        }
        m_objectives.push_back(std::make_pair(f, c));
    }
    if (!is_valid_objectives_count()) {
        throw objectives_count_error();
    }
}

size_t config::str2uint(const std::string& s) const
{
    long i = -1;
    try {
        i = std::stoul(s);
    } catch(std::exception const&) {
    }
    if (i < 0) {
        throw positive_integer_error();
    }
    return static_cast<size_t>(i);
}

void config::load_moves_count(const boost::property_tree::ptree& ptree)
{
    static const std::string s_moves = "Moves";
    try {
        m_moves_count = str2uint(ptree.get_child(s_moves).data());
    } catch(...) {
        throw moves_count_error();
    }
    if (!is_valid_moves_count()) {
        throw moves_count_error();
    }
}

void config::load_figures_colors_count(const boost::property_tree::ptree& ptree)
{
    static const std::string s_colors = "Colors";
    try {
        m_figure_colors_count = str2uint(ptree.get_child(s_colors).data());
    } catch(...) {
        throw figures_count_error();
    }
    if (!is_valid_figures_count()) {
        throw figures_count_error();
    }
}

bool config::is_valid_board_size() const noexcept
{
    if (m_board_size.rows < board::min_size() || m_board_size.rows > board::max_size()) {
        return false;
    } else if (m_board_size.cols < board::min_size() || m_board_size.cols > board::max_size()) {
        return false;
    }
    return true;
}

bool config::is_valid_moves_count() const noexcept
{
    return m_moves_count > 0;
}

bool config::is_valid_figures_count() const noexcept
{
    return m_figure_colors_count >= MIN_FIGURE_COLORS_COUNT && m_figure_colors_count <= MAX_FIGURE_COLORS_COUNT;
}

bool config::is_valid_objectives() const noexcept
{
    if (!is_valid_objectives_count()) {
        return false;
    }
    for (const auto& it : m_objectives) {
        if (!is_valid_color(it.first)) {
            return false;
        } else if (!is_valid_objective_figures_count(it.second)) {
            return false;
        }
    }
    return true;
}

bool config::is_valid_objectives_count() const noexcept
{
    const size_t count = m_objectives.size();
    return count >= objectives::min_objectives_count() && count <= objectives::max_objectives_count();
}

bool config::is_valid_color(figure::color c) const noexcept
{
    return c != figure::color::UNDEFINED;
}

bool config::is_valid_objective_figures_count(size_t c) const noexcept
{
    return c > 0;
}

