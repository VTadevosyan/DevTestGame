
#include "definitions.hpp"
#include "texture_map.hpp"


namespace gui {


std::map<figure::color, sf::Texture*> texture_map::s_figures = {};

std::map<booster::type, sf::Texture*> texture_map::s_boosters = {};

sf::IntRect texture_map::s_rect = sf::IntRect(0, 0, ITEM_SIZE, ITEM_SIZE);

sf::Texture* texture_map::s_tile_light = nullptr;
sf::Texture* texture_map::s_tile_dark = nullptr;

static const std::string resources_path = "../../../resources/";


void texture_map::create()
{
	//// Figures
	sf::Texture* blue_texture = new sf::Texture;
	blue_texture->loadFromFile(resources_path + "blue.png", s_rect);
	sf::Texture* green_texture = new sf::Texture;
	green_texture->loadFromFile(resources_path + "green.png", s_rect);
	sf::Texture* orange_texture = new sf::Texture;
	orange_texture->loadFromFile(resources_path + "orange.png", s_rect);
	sf::Texture* red_texture = new sf::Texture;
	red_texture->loadFromFile(resources_path + "red.png", s_rect);
	sf::Texture* violet_texture = new sf::Texture;
	violet_texture->loadFromFile(resources_path + "violet.png", s_rect);
	s_figures = {
		std::make_pair(figure::color::blue, blue_texture),
		std::make_pair(figure::color::green, green_texture),
		std::make_pair(figure::color::orange, orange_texture),
		std::make_pair(figure::color::red, red_texture),
		std::make_pair(figure::color::violet, violet_texture)
	};

	//// Boosters
	sf::Texture* bomb_texture = new sf::Texture;
	bomb_texture->loadFromFile(resources_path + "bomb.png", s_rect);
	sf::Texture* h_bomb_texture = new sf::Texture;
	h_bomb_texture->loadFromFile(resources_path + "h_bomb.png", s_rect);
	sf::Texture* v_bomb_texture = new sf::Texture;
	v_bomb_texture->loadFromFile(resources_path + "v_bomb.png", s_rect);
	s_boosters = {
		std::make_pair(booster::type::horizontal, h_bomb_texture),
		std::make_pair(booster::type::vertical, v_bomb_texture),
		std::make_pair(booster::type::radial, bomb_texture)
	};

	//// Tiles
	s_tile_dark = new sf::Texture;
	s_tile_dark->loadFromFile(resources_path + "tile_1.png", sf::IntRect(0, 0, ITEM_SIZE, ITEM_SIZE));
	s_tile_light = new sf::Texture;
	s_tile_light->loadFromFile(resources_path + "tile_2.png", sf::IntRect(0, 0, ITEM_SIZE, ITEM_SIZE));

}

void texture_map::destroy()
{
	//// Figures
	for (auto it : s_figures) {
		delete it.second;
	}
	s_figures.clear();
	//// Boosters
	for (auto it : s_boosters) {
		delete it.second;
	}
	s_boosters.clear();
}

sf::Texture* texture_map::find_texture(board_item* item) noexcept
{
	if (item == nullptr) {
		return nullptr;
	}
	if (figure* f = dynamic_cast<figure*>(item)) {
		return find_texture(f->get_color());
	}
	else if (booster* b = dynamic_cast<booster*>(item)) {
		return find_texture(b->get_type());
	}
	return nullptr;
}

sf::Texture* texture_map::find_texture(const figure::color& c) noexcept
{
	auto it = s_figures.find(c);
	return it != s_figures.end() ? it->second : nullptr;
}

sf::Texture* texture_map::find_texture(const booster::type& t) noexcept
{
	auto it = s_boosters.find(t);
	return it != s_boosters.end() ? it->second : nullptr;
}

sf::Texture* texture_map::get_light_tile() noexcept
{
	return s_tile_light;
}

sf::Texture* texture_map::get_dark_tile() noexcept
{
	return s_tile_dark;
}

}
