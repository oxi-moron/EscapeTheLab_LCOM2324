#ifndef G6_MAP_H
#define G6_MAP_H

#include <lcom/lcf.h>

/**
 * @brief Gets the element of a given grid position (wall, door or empty cell).
 * @param x X coordinate of the grid position.
 * @param y Y coordinate of the grid position.
 * @param pos Address where the grid position will be stored.
 * @return 0 if successful, non-zero otherwise.
 */
int (map_get_grid_pos) (uint32_t x, uint32_t y, uint8_t* pos);

/**
 * @brief Sets the element of a given grid position.
 * @param x X coordinate of the grid position.
 * @param y Y coordinate of the grid position.
 * @param pos Position to be set.
 * @return 0 if successful, non-zero otherwise.
 */
int (map_set_grid_pos) (uint32_t x, uint32_t y, uint8_t pos);

/**
 * @brief Gets the map grid dimensions.
 * @param width Address where the map width will be stored.
 * @param height Address where the map height will be stored.
 * @return 0 if successful, non-zero otherwise.
 */
int (map_get_grid_dimensions) (uint32_t* width, uint32_t* height);

/**
 * @brief Loads a new map as the current map.
 * @param map_no Number of the map to be loaded.
 * @return 0 if successful, non-zero otherwise.
 */
int (map_load) (uint8_t map_no);

#endif //G6_MAP_H
