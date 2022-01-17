
#ifndef CORE_BOARD_HPP
#define CORE_BOARD_HPP

#include "figure.hpp"
#include "index.hpp"

#include <list>
#include <map>
#include <memory>
#include <vector>


class board_item;
class proxy_figure;


//// @class board
//// @brief Board keeps all items(figures and boosters) and allows to do some modifications
//// It keeps items using std::vector. You can get specified item passing the item's index.
//// The specified index will be converted to raw index and then return item if it exists.
class board
{
public:
    using ptr = std::unique_ptr<board>;

public:
	//// @brief Gets the minimum row/col size - 7
    static size_t min_size() noexcept;

	//// @brief Gets the minimum row/col size - 10
    static size_t max_size() noexcept;

public:
	//// @brief Constructor
    board(size_t, size_t);

	//// @brief Destructor
    ~board();

public:
	//// @brief Gets the board rows count
    size_t rows() const noexcept;

	//// @brief Gets the board columns count
    size_t columns() const noexcept;

	//// @brief Gets the board item with specified index
	//// @return board_item if index is valid or nullptr
	board_item* get_item(const index&) const noexcept;

	//// @brief Sets the item to specified index
	//// @note tem can be a figure or a booster
    void add_item(board_item*, const index&);

	//// @brief Removes given item from board
	//// @note after destroying item their index should free. e.g nullptr
	//// @note if specified board item it will replace existing item then destroy it
    void destroy_item(const index&);

	//// @brief Determines whether if two specified indexes can be swapped or not
	//// @return true if neighbour indexes can be swapped and false otherwise
	//// @note Swap can be completed only if after swapping indexes finds any pattern to match
    bool can_swap(const index&, const index&) const noexcept;

	//// @brief Swaps given two indexes in board
	//// @note Given indexes can be swapped
    void swap(const index&, const index&);

	//// @brief Drop items down in given column while the bottom item(s) is free
	void drop_column(const size_t);

	//// @brief Drop items down in given column while the bottom item(s) is free
	//// @note Collects dropped indexes to match
    void drop_column(const size_t, std::list<index>&);

	//// @brief Gets the empty tiles on board
	void get_empty_tiles(std::list<index>&) const;

	//// @brief Adds the specified proxy figure into proxy items map
	void add_proxy_figure(const proxy_figure*) noexcept;

	//// @brief Removes the specified proxy figure from proxy items map
	void remove_proxy_figure(const proxy_figure*) noexcept;

	//// @brief Checks in proxy items map if specified index is proxy item's index or not
	bool is_proxy_item(const index&) const noexcept;

	//// @brief Gets the proxy items color with given index
	//// @return proxy item's color if it is exists, otherwise - figure::color::UNDEFINED
	figure::color get_proxy_color(const index&) const noexcept;

	//// @brief Shuffles the board;
	void shuffle();


	//// XXX
    void draw();

private:
    size_t get_raw_index(const index&) const noexcept;
	index get_index_from_raw(size_t) const noexcept;
    bool is_neighbours(const index&, const index&) const noexcept;

private:
    std::vector<board_item*> m_data;
	std::map<index, figure::color> m_proxy_items;
    size_t m_rows;
    size_t m_cols;

};

#endif // CORE_BOARD_HPP
