
#include "exceptions.hpp"
#include "figure.hpp"
#include "game_controller.hpp"
#include "notifier.hpp"
#include "objective.hpp"
#include "objectives.hpp"

#include <cassert>



size_t objectives::min_objectives_count() noexcept
{
    return 1;
}

size_t objectives::max_objectives_count() noexcept
{
    return 3;
}

objectives::objectives(const config::ptr& c)
{
    init_objectives_from_config(c);
}

bool objectives::completed() const noexcept
{
    for (auto it : m_objectives) {
        if (!it->completed()) {
            return false;
        }
    }
    return true;
}

void objectives::get_objectives(std::vector<objective*>& o) const noexcept
{
	o = m_objectives;
}

void objectives::on_figure_destroyed(figure* f)
{
	game_controller::game_status status = game_controller::get()->get_game_status();
	if (status != game_controller::game_status::in_progress) {
		return;
	}
    assert(f != nullptr);
    const figure::color c = f->get_color();
    std::function<bool(objective*)> objective_finder = [&c] (objective* o) -> bool {
        assert(o != nullptr);
        return o->get_color() == c;
    };
    auto it = std::find_if(m_objectives.begin(), m_objectives.end(), objective_finder);
    if (it != m_objectives.end()) {
        (*it)->decrease_count();
    }
	if (completed()) {
		notifier::get()->on_objectives_completed();
	}
}


void objectives::init_objectives_from_config(const config::ptr& c)
{
    config::objectives_t os;
    c->get_objectives(os);
    for (const auto& it : os) {
        m_objectives.push_back(new objective(it.first, it.second));
    }
    if (!is_valid_objectives_count()) {
        throw objectives_count_error();
    }
}

bool objectives::is_valid_objectives_count() const noexcept
{
    const size_t count = m_objectives.size();
    return count >= min_objectives_count() && count <= max_objectives_count();
}

