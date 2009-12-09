/*
 *  Copyright (c) 1997 - 2002 by Volker Meyer & Hansj�rg Malthaner
 *
 * This file is part of the Simutrans project under the artistic licence.
 *
 *  node structure:
 *	0   Name
 *	1   Copyright
 *	2   Bildliste Hintergrund
 *	3   Bildliste Vordergrund
 *  4   cursor(image 0) and icon (image 1)
 *	5   Bildliste Hintergrund - snow
 *	6   Bildliste Vordergrund - snow
 */

#ifndef __TUNNEL_BESCH_H
#define __TUNNEL_BESCH_H

#include "../simimg.h"
#include "../simtypes.h"
#include "obj_besch_std_name.h"
#include "skin_besch.h"
#include "bildliste2d_besch.h"
#include "weg_besch.h"

class tunnel_besch_t : public obj_besch_std_name_t {
	friend class tunnel_writer_t;
	friend class tunnel_reader_t;
	friend class tunnelbauer_t;	// to convert the old tunnels to new ones

private:
	static int hang_indices[16];

	uint32 topspeed;	// speed in km/h
	uint32 preis;	// 1/100 credits
	uint32 scaled_price; // The price after scaling. @author: jamespetts
	uint32 maintenance;	// monthly cost for bits_per_month=18
	uint32 scaled_maintenance;
	uint8 wegtyp;	// waytype for tunnel
	uint32 max_weight; // maximum weight for vehicles. @author: jamespetts

	// allowed era
	uint16 intro_date;
	uint16 obsolete_date;

	/* number of seasons (0 = none, 1 = no snow/snow)
	*/
	sint8 number_seasons;

	/*Way constraints for, e.g., loading gauges, types of electrification, etc.
	* @author: jamespetts*/
	uint8 way_constraints_permissive;
	uint8 way_constraints_prohibitive;

	/* has underground way image ? (0 = no, 1 = yes)
	*/
	uint8 has_way;

	werkzeug_t *builder;

public:
	const bild_besch_t *get_hintergrund(hang_t::typ hang, uint8 season) const
	{
		const bild_besch_t *bild = NULL;
		if(season && number_seasons == 1) {
			bild = static_cast<const bildliste_besch_t *>(get_child(5))->get_bild(hang_indices[hang]);
		}
		if(bild == NULL) {
			bild = static_cast<const bildliste_besch_t *>(get_child(2))->get_bild(hang_indices[hang]);
		}
		return bild;
	}

	image_id get_hintergrund_nr(hang_t::typ hang, uint8 season) const
	{
		const bild_besch_t *besch = get_hintergrund(hang, season);
		return besch != NULL ? besch->get_nummer() : IMG_LEER;
	}

	const bild_besch_t *get_vordergrund(hang_t::typ hang, uint8 season) const
	{
		const bild_besch_t *bild = NULL;
		if(season && number_seasons == 1) {
			bild = static_cast<const bildliste_besch_t *>(get_child(6))->get_bild(hang_indices[hang]);
		}
		if(bild == NULL) {
			bild = static_cast<const bildliste_besch_t *>(get_child(3))->get_bild(hang_indices[hang]);
		}
		return bild;
	}

	image_id get_vordergrund_nr(hang_t::typ hang, uint8 season) const
	{
		const bild_besch_t *besch = get_vordergrund(hang, season);
		return besch != NULL ? besch->get_nummer() :IMG_LEER;
	}

	const skin_besch_t *get_cursor() const { return static_cast<const skin_besch_t *>(get_child(4)); }


	// get costs etc.
	waytype_t get_waytype() const { return static_cast<waytype_t>(wegtyp); }

	sint32 get_preis() const { return scaled_price; }

	sint32 get_base_price() const { return preis; }

	sint32 get_wartung() const { return scaled_maintenance; }

	sint32 get_base_maintenance() const { return maintenance; }

	void set_scale(float scale_factor) 
	{ 
		scaled_price = preis * scale_factor > 0 ? (uint32) (preis * scale_factor) : 1; 
		scaled_maintenance = maintenance * scale_factor > 0 ? (uint32) (maintenance * scale_factor) : 1;
	}

	uint32  get_topspeed() const { return topspeed; }

	uint32  get_max_weight() const { return max_weight; }

	uint16 get_intro_year_month() const { return intro_date; }

	uint16 get_retire_year_month() const { return obsolete_date; }
	
	/* Way constraints: determines whether vehicles
	 * can travel on this way. This method decodes
	 * the byte into bool values. See here for
	 * information on bitwise operations: 
	 * http://www.cprogramming.com/tutorial/bitwise_operators.html
	 * @author: jamespetts
	 * */
	const bool permissive_way_constraint_set(uint8 i)
	{
		return ((way_constraints_permissive & 1)<<i != 0);
	}

	const bool prohibitive_way_constraint_set(uint8 i)
	{
		return ((way_constraints_prohibitive & 1)<<i != 0);
	}

	uint8 get_way_constraints_permissive() const { return way_constraints_permissive; }
	uint8 get_way_constraints_prohibitive() const { return way_constraints_prohibitive; }

	const weg_besch_t *get_weg_besch() const
	{
		if(has_way) {
			return static_cast<const weg_besch_t *>(get_child(5 + number_seasons * 2));
		}
		return NULL;
	}

	// default tool for building
	werkzeug_t *get_builder() const {
		return builder;
	}
	void set_builder( werkzeug_t *w )  {
		builder = w;
	}
};

#endif
