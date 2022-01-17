
#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

#include <list>


class booster;
class figure;
class index;
class listener;


//// @class notifier
//// @brief Notifies about some event(s) to their listeners
class notifier
{
public:
	//// @brief Creates a notiier
    static void create();

	//// @brief Destroys a notiier
    static void destroy();

	//// @brief Gets a notifier
    static notifier* get();

private:
    static notifier* s_notifier;

public:
	//// @brief Destructor
	~notifier() = default;

	//// @brief Deleted copy constructor
	notifier(const notifier&) = delete;

	//// @brief Deleted move constructor
	notifier(notifier&&) = delete;

	//// @brief Deleted operator assignement
	notifier& operator= (const notifier&) = delete;
	
	//// @brief Deleted move operator assignement
	notifier&& operator= (notifier&&) = delete;
	
private:
	//// @brief Constructor
	notifier() = default;

public:
	//// @brief Notifies about destroying figure
    void on_figure_destroyed(figure*) noexcept;

	//// @brief Notifies about all game objectives were completed
	void on_objectives_completed() noexcept;

	//// @brief Notifies about completing a game
	void on_level_passed() noexcept;

	//// @brief Notifies about failing a game
	void on_level_failed() noexcept;

	//// @brief Notifies about destroyed items
	void on_items_destroyed(const std::list<index>&);

	//// @brief Notifies about swapped items
	void on_items_swapped(const index&, const index&);

	//// @brief Notifies about booster creation
	void on_booster_created(booster*, const index&);

	//// @brief Notifies about dropped columns
	void on_columns_dropped(const std::list<size_t>&);

	//// @brief Notifies about newly generated items
	void on_new_items_dropped(const std::list<std::pair<figure*, index>>&);

	//// @brief Notifies about shuffling board items
	void on_shuffle();

public:
	//// @brief Adds a new listener object
    void add_listener(listener*);

	//// @brief Removes a given listener from registered listeners
	void remove_listener(listener*);

	//// @brief Enables callbacks
	void enable() noexcept;
	
	//// @brief Disables callbacks
	void disable() noexcept;

private:
    std::list<listener*> m_listeners;
	//// XXX TODO - CHECK IF EVERY CALLBACK NEED TO CHECK THIS FLAG BEFORE CALLING THEIR LISTENER
	bool m_enabled = true;

};

#endif // NOTIFIER_HPP
