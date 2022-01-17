
#ifndef GUI_DEFINITIONS_HPP
#define GUI_DEFINITIONS_HPP

//// Defines item's width and height
constexpr int ITEM_SIZE = 80;

//// Frames Per Second
constexpr int FPS = 12;

//// Frame delay (depends from FPS)
constexpr float DELAY = (float)1 / FPS;

//// Dropping steps count is 4. E.g dropping items down
constexpr int STEPS_COUNT = 4;

//// The default time when item will be go down by one row
constexpr float ROW_DURATION = STEPS_COUNT * DELAY;

//// The board offset
//// The board Y position is 180
//// For newly created items the vertical position will be BOARD_OFFSET
//// The created items should go down from initial position
constexpr int BOARD_OFFSET = 100;


#endif //// GUI_DEFINITIONS_HPP
