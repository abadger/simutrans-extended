/*
 * Copyright (c) 1997 - 2001 Hansj�rg Malthaner
 *
 * This file is part of the Simutrans project under the artistic licence.
 * (see licence.txt)
 */

#ifndef obj_gebaeude_h
#define obj_gebaeude_h

#include "../ifc/sync_steppable.h"
#include "../simobj.h"
#include "../simcolor.h"
#include "../tpl/minivec_tpl.h"

class building_tile_desc_t;
class fabrik_t;
class stadt_t;
class goods_desc_t;
class planquadrat_t;

/**
 * Asynchron oder synchron animierte Gebaeude f�r Simutrans.
 * @author Hj. Malthaner
 */
class gebaeude_t : public obj_t, sync_steppable
{
private:
	const building_tile_desc_t *tile;

	

	/**
	 * Time control for animation progress.
	 * @author Hj. Malthaner
	 */
	uint16 anim_time;

	/**
	 * Current anim frame
	 * @author Hj. Malthaner
	 */
	uint8 count;

	/**
	 * Is this a sync animated object?
	 * @author Hj. Malthaner
	 */
	uint8 sync:1;

	/**
	 * Boolean flag if a construction site or buildings image
	 * shall be displayed.
	 * @author Hj. Malthaner
	 */
	uint8 zeige_baugrube:1;

	/**
	 * if true, this ptr union contains a factory pointer
	 * ? Surely, this cannot be right with an 8 bit integer? Out of date comment?
	 * @author Hj. Malthaner
	 */
	uint8 is_factory:1;

	uint8 season:3;
	uint8 background_animated:1;

	uint8 remove_ground:1;  // true if ground image can go

	uint8 anim_frame;

	/**
	 * Zeitpunkt an dem das Gebaeude Gebaut wurde
	 * "Time at that was built the building" (Babelfish)
	 * @author Hj. Malthaner
	 */
	sint64 purchase_time;
	
	/**
	* either point to a factory or a city
	* @author Hj. Malthaner
	*/
	union {
		fabrik_t  *fab;
		stadt_t *stadt;
	} ptr;

	/**
	 * Initializes all variables with safe, usable values
	 * @author Hj. Malthaner
	 */
	void init();

	/** 
	 * Stores the dynamic population of this building:
	 * either as visitor demand (commercial) or population
	 * (residential). This is the fully adjusted figure, 
	 * representing passengers to be generated or demanded 
	 * per game month.
	 */
	union people_t
	{
		uint16 population;
		uint16 visitor_demand;
	} people;

	union adjusted_people_t
	{
		uint16 population;
		uint16 visitor_demand;
	} adjusted_people;

	/** 
	 * Stores the dynamic number of jobs in this building
	 * at present. This is the fully adjusted figure, 
	 * representing commuters to be demanded per game month.
	 */
	uint16 jobs;
	uint16 adjusted_jobs;

	/** 
	 * Stores the dynamic level of mail demand in this building
	 * at present. This is the fully adjusted figure, 
	 * representing packets of mail to be generated or demanded 
	 * per game month.
	 */
	uint16 mail_demand;
	uint16 adjusted_mail_demand;

	/** The following variables record the proportion of
	 * successful passenger trips generated by this building
	 * in the current and last year respectively.
	 */
	uint16 passengers_generated_commuting;
	uint16 passengers_succeeded_commuting;
	uint16 passenger_success_percent_last_year_commuting;

	uint16 passengers_generated_visiting;
	uint16 passengers_succeeded_visiting;
	uint16 passenger_success_percent_last_year_visiting;

	/**
	* This is the number of jobs supplied by this building
	* multiplied by the number of ticks per month, subtracted
	* from the creation time, to which is added the number
	* of ticks per month whenever a commuter reaches this
	* destination. Further, this value is set so that, 
	* whenever a number is added to it, it will never be less
	* than that number plus the number of ticks per month
	* multiplied by the number of available jobs minus
	* the current time. This is intended to prevent more
	* commuters going to this building each month than there
	* are jobs available for them. 
	* @author: jamespetts
	*/
	sint64 available_jobs_by_time;

	// This is true if the building is in one or more world lists.
	sint8 is_in_world_list;

	// Whether the passenger and mail figures need recalculating or not.
	bool loaded_passenger_and_mail_figres;

	minivec_tpl<const planquadrat_t*> building_tiles;

#ifdef INLINE_OBJ_TYPE
protected:
	gebaeude_t(obj_t::typ type);
	gebaeude_t(obj_t::typ type, koord3d pos,player_t *player, const building_tile_desc_t *t);
	void init(player_t *player, const building_tile_desc_t *t);

public:
	gebaeude_t(loadsave_t *file, bool do_not_add_to_world_list);
	gebaeude_t(koord3d pos,player_t *player, const building_tile_desc_t *t);
#else
protected:
	gebaeude_t();

public:
	gebaeude_t(loadsave_t *file);
	gebaeude_t(koord3d pos,player_t *player, const building_tile_desc_t *t);
#endif
	virtual ~gebaeude_t();

	void rotate90();

	void add_alter(sint64 a);

	void set_fab(fabrik_t *fd);
	void set_stadt(stadt_t *s);

	/**
	 * Ein Gebaeude kann zu einer Fabrik geh�ren.
	 * @return Einen Zeiger auf die Fabrik zu der das Objekt geh�rt oder NULL,
	 * wenn das Objekt zu keiner Fabrik geh�rt.
	 *
	 * A building can belong to a factory. 
	 * return a pointer on the factory to that the object belongs or NULL,
	 * if the object belongs to no factory. (Google)
	 *
	 * @author Hj. Malthaner
	 */
	fabrik_t* get_fabrik() const { return is_factory ? ptr.fab : NULL; }
	stadt_t* get_stadt() const;

#ifdef INLINE_OBJ_TYPE
#else
	obj_t::typ get_typ() const { return obj_t::gebaeude; }
#endif

	/**
	 * waytype associated with this object
	 */
	waytype_t get_waytype() const;

	image_id get_image() const;
	image_id get_image(int nr) const;
	image_id get_front_image() const;
	void mark_images_dirty() const;

	image_id get_outline_image() const;
	PLAYER_COLOR_VAL get_outline_colour() const;

	// caches image at height 0
	void calc_image();

	/**
	 * Called whenever the season or snowline height changes
	 * return false and the obj_t will be deleted
	 */
	bool check_season(const bool) { calc_image(); return true; }

	/**
	 * @return eigener Name oder Name der Fabrik falls Teil einer Fabrik
	 * @author Hj. Malthaner
	 */
	virtual const char *get_name() const;

	void get_description(cbuffer_t & buf) const;

	bool is_townhall() const;

	bool is_headquarter() const;

	bool is_monument() const;

	bool is_attraction() const;

	bool is_city_building() const;

	/**
	 * @return Einen Beschreibungsstring f�r das Objekt, der z.B. in einem
	 * Beobachtungsfenster angezeigt wird.
	 * @author Hj. Malthaner
	 */
	void info(cbuffer_t & buf, bool dummy = false) const;

	void get_class_percentage(cbuffer_t & buf) const;

	void rdwr(loadsave_t *file);

	/**
	 * Play animations of animated buildings.
	 * Count-down to replace construction site image by regular image.
	 */
	sync_result sync_step(uint32 delta_t);

	void set_tile( const building_tile_desc_t *t, bool start_with_construction );

	const building_tile_desc_t *get_tile() const { return tile; }

	virtual void show_info();

	void cleanup(player_t *player);

	void finish_rd();

	// currently animated
	bool is_sync() const { return sync; }

	/**
	 * @returns pointer to first tile of a multi-tile building.
	 */
	const gebaeude_t* get_first_tile() const;

	gebaeude_t* access_first_tile();


	uint16 get_passengers_succeeded_visiting() const { return passengers_succeeded_visiting; }
	uint16 get_passengers_succeeded_commuting() const { return passengers_succeeded_commuting; }

	uint16 get_passenger_success_percent_this_year_commuting() const { return passengers_generated_commuting > 0 ? (passengers_succeeded_commuting * 100) / passengers_generated_commuting : 65535; }
	uint16 get_passenger_success_percent_last_year_commuting() const { return passenger_success_percent_last_year_commuting; }
	uint16 get_average_passenger_success_percent_commuting() const { return (get_passenger_success_percent_this_year_commuting() + passenger_success_percent_last_year_commuting) / 2; }

	uint16 get_passenger_success_percent_this_year_visiting() const { return passengers_generated_visiting > 0 ? (passengers_succeeded_visiting * 100) / passengers_generated_visiting : 65535; }
	uint16 get_passenger_success_percent_last_year_visiting() const { return passenger_success_percent_last_year_visiting; }
	uint16 get_average_passenger_success_percent_visiting() const { return (get_passenger_success_percent_this_year_visiting() + passenger_success_percent_last_year_visiting) / 2; }

	void add_passengers_generated_visiting(uint16 number) { passengers_generated_visiting += number; }
	void add_passengers_succeeded_visiting(uint16 number) { passengers_succeeded_visiting += number; }

	void add_passengers_generated_commuting(uint16 number) { passengers_generated_commuting += number; }
	void add_passengers_succeeded_commuting(uint16 number) { passengers_succeeded_commuting += number; }

	void set_passengers_visiting_last_year(uint16 value) { passenger_success_percent_last_year_visiting = value; }
	void set_passengers_commuting_last_year(uint16 value) { passenger_success_percent_last_year_commuting = value; }

	void new_year();

	void check_road_tiles(bool del);

	uint16 get_weight() const;

	bool get_is_factory() const { return is_factory; }

	/**
	* Call this method when commuting passengers are sent to this building.
	* Pass the number of passengers being sent.
	* @author: jamespetts, August 2013
	*/
	void set_commute_trip(uint16 number);

	uint16 get_population() const;
	uint16 get_adjusted_population() const;

	uint16 get_visitor_demand() const;
	uint16 get_adjusted_visitor_demand() const;
	inline void set_adjusted_visitor_demand(uint16 new_visitor_demand) { adjusted_people.visitor_demand = new_visitor_demand; }

	inline uint16 get_jobs() const { return jobs; }
	inline uint16 get_adjusted_jobs() const { return adjusted_jobs; }
	inline void set_adjusted_jobs(uint16 new_jobs) { adjusted_jobs = new_jobs; }

	inline uint16 get_mail_demand() const { return mail_demand; }
	inline uint16 get_adjusted_mail_demand() const { return adjusted_mail_demand; }
	inline void set_adjusted_mail_demand(uint16 new_mail_demand) { adjusted_mail_demand = new_mail_demand; }

	bool jobs_available() const;

	/**
	 * @returns true if both building tiles are part of one (multi-tile) building.
	 */
	bool is_same_building(gebaeude_t* other) const;

	/**
	* Returns the number of jobs left in this building this month.
	* Note: this is measured in *adjusted* jobs.
	*/
	sint32 check_remaining_available_jobs() const;

	/*
	* Returns a percentage of the staffing level for this building
	*/
	sint32 get_staffing_level_percentage() const;

	uint8 get_random_class(const goods_desc_t* wtyp);

	bool get_is_in_world_list() const { return is_in_world_list > 0; }
	void set_in_world_list(bool value) { value ? is_in_world_list = 1 : is_in_world_list = 0; }

	sint64 get_available_jobs_by_time() const { return available_jobs_by_time; }
	void set_available_jobs_by_time(sint64 value) { available_jobs_by_time = value; }

	bool get_loaded_passenger_and_mail_figres() const { return loaded_passenger_and_mail_figres; }
	void set_loaded_passenger_and_mail_figres(bool value) { loaded_passenger_and_mail_figres = value; }
	
	const minivec_tpl<const planquadrat_t*> &get_tiles();

	void reset_tile_list() { building_tiles.clear(); }
};


template<> inline gebaeude_t* obj_cast<gebaeude_t>(obj_t* const d)
{
	return dynamic_cast<gebaeude_t*>(d);
}

#endif
