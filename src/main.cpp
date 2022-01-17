
#include "core/board.hpp"
#include "core/exceptions.hpp"
#include "core/config.hpp"
#include "core/game_controller.hpp"

#include "gui/main_window.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <cassert>
#include <iostream>



int main(int, char*[])
{

    try {
     
		gui::main_window mw;
		return mw.exec_event_loop();

    } catch(const base_exception& e) {
        std::cout << "Couldn't launch game ..." << std::endl;
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "Couldn't launch game ..." << std::endl;
    }

    return 0;
}
