
#ifndef CORE_CONFIG_HPP
#define CORE_CONFIG_HPP

#include "figure.hpp"
#include "objective.hpp"

#include <string>
#include <vector>

#include <iostream>

#include <boost/property_tree/ptree.hpp>


//// @class config
//// @brief Loads the game configuration from given JSON file
//// Given JSON file should have the following fields
////	* Board (7 - 10)
////		* Rows - <INTEGER>
////		* Cols - <INTEGER>
////	* Moves
////	* Objectives (1 - 3)
////		* <COLORS> - <INTEGER>
////	* Colors (3 - 5)
class config
{
public:
    static const size_t MIN_FIGURE_COLORS_COUNT;
    static const size_t MAX_FIGURE_COLORS_COUNT;

private:
	//// @struct board_size
	//// @brief Board size(row/col) holder
    struct board_size
    {
        size_t rows = 0;
        size_t cols = 0;
    };


public:
    using objectives_t = std::vector<std::pair<figure::color, size_t>>;
    using ptr = std::unique_ptr<config>;

public:
	//// @brief Constructor
    config() = default;

	//// @brief Destructor
    virtual ~config() = default;

public:
	//// @brief Loads the game configuration based on given JSON file
	//// @param[in] f The file name
	//// @throw see exceptions.hpp
    void load(const std::string& f);

	//// @brief Determines whether if current game configurations is valid or not
    bool is_valid() const noexcept;

	//// @brief Gets the board rows count
    size_t get_rows() const noexcept;

	//// @brief Gets the board cols count
    size_t get_cols() const noexcept;

	//// @brief Gets the game moves count
    size_t get_moves_count() const noexcept;

    //// @brief Gets the game figure colors count
    size_t get_figures_count() const noexcept;

    //// @brief Gets the objectives
    void get_objectives(objectives_t&) const noexcept;

private:
    void init(const std::string&);

    void load_board_size(const boost::property_tree::ptree&);
    void load_objectives(const boost::property_tree::ptree&);
    void load_moves_count(const boost::property_tree::ptree&);
    void load_figures_colors_count(const boost::property_tree::ptree&);

    bool is_valid_board_size() const noexcept;
    bool is_valid_moves_count() const noexcept;
    bool is_valid_figures_count() const noexcept;
    bool is_valid_objectives() const noexcept;
    bool is_valid_objectives_count() const noexcept;
    bool is_valid_color(figure::color) const noexcept;
    bool is_valid_objective_figures_count(size_t) const noexcept;

    size_t str2uint(const std::string&) const;


private:
    board_size m_board_size;
    size_t m_moves_count = 0;
    size_t m_figure_colors_count = 0;
    objectives_t m_objectives;

};

#endif /// CORE_CONFIG_HPP
