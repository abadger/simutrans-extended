/*
 * Copyright (c) 2010 Bernd Gabriel
 *
 * This file is part of the Simutrans project under the artistic licence.
 * (see licence.txt)
 */

#include <string.h>
#include "gui_component_table.h"
#include "gui_label.h"
#include "gui_button.h"
#include "../../simgraph.h"
#include "../../simwin.h"

// BG, 22.03.2010
coordinate_t gui_component_table_t::add_column(gui_table_column_t *column) {
	coordinate_t x = gui_table_t::add_column(column);
	coordinate_t h = get_size().get_y();
	for (coordinate_t y = 0; y < h; y++) {
		init_cell(x, y);
	}
	return x;
}

// BG, 22.03.2010
coordinate_t gui_component_table_t::add_row(gui_table_row_t *row) {
	coordinate_t y = gui_table_t::add_row(row);
	coordinate_t w = get_size().get_x();
	for (coordinate_t x = 0; x < w; x++) {
		init_cell(x, y);
	}
	return y;
}


// BG, 18.03.2010
void gui_component_table_t::change_size(const coordinates_t &old_size, const coordinates_t &new_size) {

	// remove no longer needed cells
	if (gui_cells.get_count())
	{
		for (coordinate_t x = new_size.get_x(); x < old_size.get_x(); x++) {
			for (coordinate_t y = 0; y < old_size.get_y(); y++) {
				remove_cell(x, y);
			}
		}
		for (coordinate_t y = new_size.get_y(); y < old_size.get_y(); y++) {
			for (coordinate_t x = 0; x < min(old_size.get_x(), new_size.get_x()); x++) {
				remove_cell(x, y);
			}
		}
	}

	// change size of arrays
	gui_table_t::change_size(old_size, new_size);
	gui_cells.resize(new_size.get_x());
	for (coordinate_t x = 0; x < new_size.get_x(); x++) {
		gui_cells[x].resize(new_size.get_y());
	}

	// initialize new cells
	for (coordinate_t x = old_size.get_x(); x < new_size.get_x(); x++) {
		for (coordinate_t y = 0; y < new_size.get_y(); y++) {
			init_cell(x, y);
		}
	}
	for (coordinate_t y = old_size.get_y(); y < new_size.get_y(); y++) {
		for (coordinate_t x = 0; x < min(old_size.get_x(), new_size.get_x()); x++) {
			init_cell(x, y);
		}
	}
}

// BG, 18.03.2010
void gui_component_table_t::init_cell(coordinate_t x, coordinate_t y) {
}

// BG, 18.03.2010
void gui_component_table_t::paint_cell(const koord &offset, coordinate_t x, coordinate_t y) {
	gui_komponente_t *component = get_cell_component(x, y);
	if (component)
	{
		component->zeichnen(offset);
	}
}

// BG, 18.03.2010
void gui_component_table_t::remove_cell(coordinate_t x, coordinate_t y) {
	if (owns_cell_components)
	{
		gui_komponente_t *cell = get_cell_component(x, y);
		if (cell) {
			set_cell_component(x, y, NULL);
			delete cell;
		}
	}
}

// BG, 27.03.2010
void gui_component_table_t::remove_column(coordinate_t x) 
{
	// remove cells
	for (coordinate_t y = get_size().get_y(); y > 0; ) {
		remove_cell(x, --y);
	}

	// change size of arrays
	gui_table_t::remove_column(x);
	if ((uint32) x < gui_cells.get_count()) {
		gui_cells.remove_at(x);
	}
}

// BG, 27.03.2010
void gui_component_table_t::remove_row(coordinate_t y)
{
	// remove cells and change size of arrays
	for (coordinate_t x = get_size().get_x(); x > 0; ) {
		remove_cell(--x, y);
		if ((uint32) x < gui_cells.get_count()) {
			gui_cells[x].remove(y);
		}
	}
	gui_table_t::remove_row(y);
}
