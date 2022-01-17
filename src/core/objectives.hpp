
#ifndef OBJECTIVES_H
#define OBJECTIVES_H

#include "config.hpp"
#include "figure.hpp"
#include "listener.hpp"

#include <vector>


class objective;

//// @class objectives
//// @brief The manager of objectives
//// The Objectives keeps all registered objectives and observes them
//// If all objectives are completed notifies about it to game controller to finish the game
//// After successful matching all objectives will be updated
class objectives : public listener
{
public:
    using ptr = std::unique_ptr<objectives>;

public:
	//// @brief Gets the minimum objectives count - 1
    static size_t min_objectives_count() noexcept;

	//// @brief Gets the maximum objectives count - 3
    static size_t max_objectives_count() noexcept;

public:
	//// @brief Constructor
	//// Creates a new objective with given configuration
    objectives(const config::ptr&);

public:
	//// @brief Checks if all objectives are completed
    bool completed() const noexcept;

	//// @brief Gets the objectives data
	void get_objectives(std::vector<objective*>&) const noexcept;

public:
	//// @brief Callback handles figure destroy event
	//// Should update objectives after destroying figure(s)
    void on_figure_destroyed(figure*) override;


private:
    void init_objectives_from_config(const config::ptr&);
    bool is_valid_objectives_count() const noexcept;

private:
    std::vector<objective*> m_objectives;

};

#endif // OBJECTIVES_H
