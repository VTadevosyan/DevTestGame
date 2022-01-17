
#ifndef CORE_BOARD_ITEM_HPP
#define CORE_BOARD_ITEM_HPP

//// @class board_item
//// @brief The base class for board items: e.g figures and boosters
class board_item
{
public:
	//// @brief Constructor
    board_item() = default;

	//// @brief Destructor
    virtual ~board_item() = default;

public:
	//// @brief Checks the board item type is figure or not
	virtual bool is_figure() const noexcept = 0;

	//// @brief Checks the board item type is booster or not
	virtual bool is_booster() const noexcept = 0;

};

#endif // CORE_BOARD_ITEM_HPP
