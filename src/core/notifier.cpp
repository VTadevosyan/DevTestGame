
#include "boosters.hpp"
#include "game_controller.hpp"
#include "listener.hpp"
#include "notifier.hpp"

#include <cassert>


notifier* notifier::s_notifier = nullptr;

void notifier::create()
{
	assert(!s_notifier);
    s_notifier = new notifier;
}

void notifier::destroy()
{
    assert(s_notifier);
    delete s_notifier;
    s_notifier = nullptr;
}

notifier* notifier::get()
{
    assert(s_notifier != nullptr);
    return s_notifier;
}

void notifier::on_figure_destroyed(figure* f) noexcept
{
	if (!m_enabled) {
		return;
	}
    for (auto it : m_listeners) {
        it->on_figure_destroyed(f);
    }
}

void notifier::on_objectives_completed() noexcept
{
	if (!m_enabled) {
		return;
	}
	for (auto it : m_listeners) {
		it->on_objectives_completed();
	}
}

void notifier::on_level_passed() noexcept
{
	if (!m_enabled) {
		return;
	}
	for (auto it : m_listeners) {
		it->on_level_passed();
	}
}

void notifier::on_level_failed() noexcept
{
	if (!m_enabled) {
		return;
	}
	for (auto it : m_listeners) {
		it->on_level_failed();
	}
}

void notifier::on_items_destroyed(const std::list<index>& items)
{
	if (!m_enabled) {
		return;
	}
	for (auto it : m_listeners) {
		it->on_items_destroyed(items);
	}
}

void notifier::on_items_swapped(const index& i1, const index& i2)
{
	if (!m_enabled) {
		return;
	}
	for (auto it : m_listeners) {
		it->on_items_swapped(i1, i2);
	}
}

void notifier::on_booster_created(booster* b, const index& i)
{
	if (!m_enabled) {
		return;
	}
	for (auto it : m_listeners) {
		it->on_booster_created(b, i);
	}
}

void notifier::on_columns_dropped(const std::list<size_t>& cols)
{
	if (!m_enabled) {
		return;
	}
	for (auto it : m_listeners) {
		it->on_columns_dropped(cols);
	}
}

void notifier::on_new_items_dropped(const std::list<std::pair<figure*, index>>& new_items)
{
	if (!m_enabled) {
		return;
	}
	for (auto it : m_listeners) {
		it->on_new_items_dropped(new_items);
	}
}

void notifier::on_shuffle()
{
	if (!m_enabled) {
		return;
	}
	for (auto it : m_listeners) {
		it->on_shuffle();
	}
}

void notifier::add_listener(listener* l)
{
    assert(l != nullptr);
    m_listeners.push_back(l);
}

void notifier::remove_listener(listener* l)
{
	assert(l != nullptr);
	auto it = std::find(m_listeners.begin(), m_listeners.end(), l);
	if (it != m_listeners.end()) {
		m_listeners.erase(it);
	}
	std::cout << "listeners: " << m_listeners.size() << std::endl;
}

void notifier::enable() noexcept
{
	m_enabled = true;
}

void notifier::disable() noexcept
{
	m_enabled = false;
}
