
#ifndef CORE_BOOSTERS_HPP
#define CORE_BOOSTERS_HPP

#include "board.hpp"
#include "board_item.hpp"
#include "index.hpp"

#include <set>


//// @class booster
//// @brief Interface for game boosters
//// Boosters can destroy horizontal or vertical or items around it.
//// @note Boosters have chain effect. It means that when booster is applied
//// it can activate another booster if it is located in booster's impact area
class booster : public board_item
{
public:
	//// @brief Boosters type enumeration
    enum class type {
        horizontal,
        vertical,
        radial
    };

public:
	//// @brief Gets the booster type
    virtual type get_type() const noexcept = 0;

	//// @brief Activates booster in given board with specified index
	//// Collects the booster's impact indexes even if booster creates chaining effect
    virtual void activate(const board::ptr&, const index&, std::set<index>&) = 0;

public:
	//// @brief Checks the board item type is figure or not
	bool is_figure() const noexcept override;

	//// @brief Checks the board item type is booster or not
	bool is_booster() const noexcept override;

};


//// @class horizontal_bomb
//// @brief type of booster which will destroy horizontal row when it was activated
//// @note The destroyed row will be that horizontal row where booster were placed
class horizontal_bomb : public booster
{
public:
	//// @brief Constructor
    explicit horizontal_bomb() noexcept;

public:
	//// @brief Gets the booster type
    booster::type get_type() const noexcept override;

	//// @brief Activates booster
    void activate(const board::ptr&, const index&, std::set<index>&) override;

};


//// @class vertical_bomb
//// @brief Type of booster which will destroy vertical row when it was activated
//// @note The destroyed row will be that vertical row where booster were placed
class vertical_bomb : public booster
{
public:
	//// @brief Constructor
    explicit vertical_bomb() noexcept;

public:
	//// @brief Gets the booster type
    booster::type get_type() const noexcept override;

	//// @brief Activates booster
    void activate(const board::ptr&, const index&, std::set<index>&) override;

};


//// @class radial_bomb
//// @brief Type of booster which will destroy items around it when it was activated
//// @note The depth of destroyed items is 1
class radial_bomb : public booster
{
public:
	//// @brief Constructor
    explicit radial_bomb() noexcept;

public:
	//// @brief Gets the booster type
    booster::type get_type() const noexcept override;

	//// @brief Activates booster
    void activate(const board::ptr&, const index&, std::set<index>&) override;

};

#endif // CORE_BOOSTERS_HPP
