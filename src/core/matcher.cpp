
#include "matcher.hpp"
#include "match_data.hpp"
#include "patterns.hpp"
#include "proxy_figure.hpp"


#include <cassert>
#include <iostream>



bool matcher::match(const board::ptr& b, const index& i, match_data& m)
{
    m.reset();
    for (auto it : m_patterns) {
        if (it->match(b, i, m)) {
            return true;
        }
    }
    return false;
}

bool matcher::proxy_match(const board::ptr& b, const index& pi1, const index& pi2)
{
	std::unique_ptr<proxy_figure> pf1;
	std::unique_ptr<proxy_figure> pf2;
	board_item* item = b->get_item(pi1);
	if (item != nullptr && item->is_figure()) {
		pf1.reset(new proxy_figure(static_cast<figure*>(item)->get_color(), pi2));
		b->add_proxy_figure(pf1.get());
	}
	item = b->get_item(pi2);
	if (item != nullptr && item->is_figure()) {
		pf2.reset(new proxy_figure(static_cast<figure*>(item)->get_color(), pi1));
		b->add_proxy_figure(pf2.get());
	}

	bool matched = false;
	for (auto it : m_patterns) {
		match_data md;
		if (it->match(b, pi1, md)) {
			matched = true;
			break;
		}
		if (it->match(b, pi2, md)) {
			matched = true;
			break;
		}
	}

	b->remove_proxy_figure(pf1.get());
	b->remove_proxy_figure(pf2.get());
	return matched;
}

void matcher::add_pattern(pattern* p)
{
    assert(p != nullptr);
    m_patterns.push_back(p);
}
