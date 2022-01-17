
#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <list>


class booster;
class figure;
class index;


//// @class listener
//// @brief Interface of notifer's listeners
class listener
{
public:
    //// @brief Destructor
    virtual ~listener() = default;

    //// @brief Handles figure destroy event
	virtual void on_figure_destroyed(figure*) { /* ... */ }

	//// @brief Handles objectives completed event
	virtual void on_objectives_completed() noexcept { /* ... */ }

	//// @brief Handles game level passed event
	virtual void on_level_passed() noexcept { /* ... */ }

	//// @brief Handles game level failed event
	virtual void on_level_failed() noexcept { /* ... */ }

	//// @brief Handles item swap event
	virtual void on_item_swapped(const index&) { /* ... */ }
	
	//// @brief Handles items destroying
	virtual void on_items_destroyed(const std::list<index>&) { /* ... */ }

	//// @brief Handles item swapping
	virtual void on_items_swapped(const index&, const index&) { /* ... */ }

	//// @brief Handles booster creation
	virtual void on_booster_created(booster*, const index&) { /* ... */ }

	//// @brief handles columns dropping
	virtual void on_columns_dropped(const std::list<size_t>&) { /* ... */ };
	
	//// @brief Handles new items dropping
	virtual void on_new_items_dropped(const std::list<std::pair<figure*, index>>&) { /* ... */ }

	//// @brief Notifies about shuffle.
	//// @note Should update gui when shuffle happened
	virtual void on_shuffle() { /* ... */ }

};


#endif // LISTENER_HPP
