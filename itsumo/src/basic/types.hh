/*
    Copyright (C) 2005 by Ana Lucia Bazzan (bazzan@inf.ufrgs.br)
                          Bruno Castro da Silva (bcs@inf.ufrgs.br)
                          Diogo Becker de Brum (dbrum@inf.ufrgs.br)
            http://www.inf.ufrgs.br/~mas/traffic/siscot

    This file is part of Itsumo.

    Itsumo is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Itsumo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Itsumo; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/
/*   Type definition header file
   */

class Calibrator;
class Car;
class Driver;
class Lane;
class LaneSet;
class Network;
class Node;
class Section;
class Topology;
class TopologyElement;
class TrafficLight;

#ifndef __HAS_TYPES_H
#define __HAS_TYPES_H

#include <cassert>			// define assert()

#include <time.h>                      // define time()
#include <unistd.h>

#include "MATSim.h"
#include "../socket/ServerSocket.h"


#include <iomanip>

#define FRONT 0
#define REAR 1
#define LEFT 2
#define RIGHT 3
#define FRONT_LEFT 4
#define FRONT_RIGHT 5
#define REAR_LEFT 6
#define REAR_RIGHT 7

// defines for the lanes' zones
#define INITIAL_ZONE_NO_CHANGES 0
#define MIDDLE_ZONE_NORMAL_BEHAVIOUR 1
#define MIDDLE_ZONE_CORRECT_PLACEMENT_ONLY 2
#define FINAL_ZONE_READAPTATION 10

#define READAPTATION_ZONE_SIZE 0
#define NON_EXISTANT_ZONE -1


#define CONSTANT_PROB 0
#define VARIABLE_PROB 1
#define CONSTANT_FLOW 2
#define VARIABLE_FLOW 3

#include <iostream>
using std::cout;
using std::endl;

using namespace std;

#include <string>
using std::pair;
using std::string;

#include <vector>
using std::vector;

#include <list>
using std::list;

#include <queue>
using std::queue;

using std::pair;

typedef unsigned int iteration_t;

// changelog (Bruno) ID's and dist_t are not unsigned int
// anymore.. this change makes possible to use, for instance,
// getFrontalGapFromCell(-1)
typedef int dist_t;
typedef dist_t speed_t;
typedef speed_t accel_t;
typedef float prob_t;


// changelog (Bruno): added a new field used to specify the cellsize (in meters)
typedef float cellSize_t;

// changelog (Bruno): added a new field used to specify how many seconds each
// iteration lasts
typedef int iterationPeriod_t;

// changelog (Bruno) ID's and dist_t are not unsigned int
// anymore.. this change makes possible to use, for instance,
// getFrontalGapFromCell(-1)
typedef int id;
typedef id driver_id_t;

typedef id topology_element_id_t;
/****************************************/

typedef pair < Lane * , LaneSet * > trafficLight_direction_t;
typedef list < trafficLight_direction_t > trafficLight_phase_t;
typedef vector < trafficLight_phase_t > trafficLight_phase_vector_t;


// types for the variable sources (variable flow and variable probability)
// changelog (bruno) 15/10/2004
typedef vector < pair < iteration_t, int > > variable_flow_vector_t;
typedef vector < pair < iteration_t, double > > variable_probability_vector_t;


struct trafficLight_plan_phase_t {
	iteration_t startIter;
	iteration_t endIter;
	iteration_t iterationInterval;
	int phaseIndex;
};
typedef vector < trafficLight_plan_phase_t > trafficLight_plan_phase_vector_t;
struct trafficLight_plan_t {
	iteration_t startIter;
	trafficLight_plan_phase_vector_t phases;
	int planIndex; //plan id in db
    string additional_info;
};
typedef vector < trafficLight_plan_t > trafficLight_plan_vector_t;


/****************************************/

typedef vector < TopologyElement * > topology_element_vector_t;
typedef vector < Car * > active_cars_vector_t;
typedef queue < Car * > inactive_cars_queue_t;


typedef vector < Network * > network_vector_t;
typedef vector < Section * > section_vector_t;
typedef vector < LaneSet * > laneSet_vector_t;
typedef vector < Lane * > lane_vector_t;
typedef vector < LaneSet * > laneset_vector_t;

typedef vector < Node * > node_vector_t;

typedef pair < Node *, Node * > node_pair_t;
typedef vector < node_pair_t > node_pair_vector_t;


typedef section_vector_t street_sections_t;

// changelog (Bruno, 26-sept-2003): a network object
// will contain a vector formed by pairs of <street-id> and
// the correspondent sections that form that street
typedef vector < pair < pair < topology_element_id_t, string >,  street_sections_t > > street_vector_t;


// changelog (Bruno, 12-feb-2004): each laneset contains a vector
// of pairs indicating the probability of turning to each possible 
// destination laneset
typedef vector < pair < LaneSet *, double > > turning_probabilities_vector_t;


// changelog (Bruno, 03-may-2004): each lane contains a vector indicating
// the possible lanesets (that is, from lane 'x' the car can go to <laneset1, laneset2, etc>
typedef vector < LaneSet * > turning_possibilities_vector_t;

typedef vector < topology_element_id_t > driven_cars_vector_t;



typedef vector < Calibrator * > calibrator_vector_t;

typedef vector < Car * > car_vector_t;

typedef pair < Lane * , prob_t > lane_probability_t;
typedef list < lane_probability_t > lane_probability_list_t;

/****************************************/


struct point_t {
	float x, y;
};

struct decision_t {
	//  accel_t acceleration;   not needed? (Bruno)
	speed_t speed;
	Lane *lane;

	// decision about which lane/laneset the car will go after leaving the current laneset
	LaneSet *nextLaneSet;
	Lane *laneInNextLaneSet;

};

// changelog (Bruno 07-apr-2004): this is the type that
// is going to be returned by the chooseNextLane_LaneSet function
typedef pair < Lane *, LaneSet * > lane_laneset_pair_t;

/*
   Some useful macros for vector insertion handling.
   V is the vector, D is the data to be inserted in it, N is the position where D should be inserted.
   */
/*
   If the elements vector cannot hold more elements, set its size
   to twice its current size + 1.
   Avoids lots of resizes.
   */


// changelog (bruno 29-sept-2003): changed 'id==0' to 'internalId==0' due to the some changes made in TopologyElement,
// namely the addition of a field used to store the object's id according to the DB, and a field to store the internal
// id
#define CHECK_VECTOR_SIZE(VECTOR, DATA) ((internalId == 0) ? (VECTOR .resize (1, DATA)) : (VECTOR .resize ((VECTOR .size () * 2), DATA)))

#define time_in_ms(a) (a.tv_sec*1000 + a.tv_usec/1000) // (tv_sec => 1s, tv_usec => 10^-6s)



											   // OLD
											   /**
											    * Defines
											    */
#define NO_TOPOLOGY_ELEMENT_ID -1
#define NO_MODULE_TYPE -1
#define NO_GRAPH_ELEMENT -1
#define INFINITY_WEIGHT HUGE_VALL
#define LOWEST_GRAPH_WEIGHT 0.0
#define GRAPH_DEFAULT_EDGE_WEIGHT 1.0
#define MODULE_ELEMENT 0
#define ABSTRACT_HELPER_MODULE 1
#define ABSTRACT_DRIVER_MODULE 2

#define DRIVER_MOD_RAND_START_POINT -1
#define DRIVER_MOD_NO_START_POINT -2

#define DRIVER_MOD_DEFAULT_START_TIME 0
#define DRIVER_MOD_NO_START_TIME -1
#define DRIVER_MOD_WAIT_START_TIME -2
#define DRIVER_MOD_RAND_START_TIME -3

#define EXTFW_ALLOWED_LOG_TAGS ""
#define EXTFW_ALLOWED_LOG_TAGS_PARAM "extfw_allowed_log_tags"

#define CAR_DEFAULT_MAX_SPEED 5
#define CAR_DEFAULT_MAX_SPEED_PARAM "car_maximum_speed"
#define CAR_DEFAULT_MAX_ACCEL 1
#define CAR_DEFAULT_MAX_ACCEL_PARAM "car_maximum_acceleration"

#define OBJECTIVE_DRIVER_AMOUNT 0
#define OBJECTIVE_DRIVER_AMOUNT_PARAM "objective_drivers_amount"
#define OBJECTIVE_DRIVER_INS_START 1
#define OBJECTIVE_DRIVER_INS_START_PARAM "objective_drivers_start_insert"
#define OBJECTIVE_DRIVER_INS_END 100
#define OBJECTIVE_DRIVER_INS_END_PARAM "objective_drivers_end_insert"

#define CHAR_BUFFER_SIZE 128

											   /**
											    * Macros
											    */
#define SECURE_CAST_MACRO(toClass, fromObjRef) (typeid(toClass) != typeid(*fromObjRef) ? NULL : static_cast<toClass*>(fromObjRef))
#define CERR_DEBUG std::cerr << "[DEBUG] " << "line " << __LINE__ << ", file " << __FILE__ << ", in " << __FUNCTION__ << "" << std::endl;
#define CERR_DEBUG_STR(name, value) std::cerr << "[DEBUG] " << "line " << __LINE__ << ", file " << __FILE__ << ", in " << __FUNCTION__ << ", var " << name << ", value [" << value << "]" << std::endl;
#define CERR_DEBUG_INT_INIT char BUFFER[CHAR_BUFFER_SIZE];
#define CERR_DEBUG_INT(name, value) snprintf(BUFFER, CHAR_BUFFER_SIZE, "%li", value); std::cerr << "[DEBUG] " << "line " << __LINE__ << ", file " << __FILE__ << ", in " << __FUNCTION__ << ", var " << name << ",value [" << BUFFER << "]" << std::endl;

#define COUT_INT_INIT char BUFFER[CHAR_BUFFER_SIZE];
#define COUT_STR_SPACE(name, value) std::cout << name << value << " ";
#define COUT_INT_SPACE(name, value) snprintf(BUFFER, CHAR_BUFFER_SIZE, "%li", value); std::cout << name << BUFFER << " ";
#define COUT_STR(value) std::cout << value;
#define COUT_INT(value) snprintf(BUFFER, CHAR_BUFFER_SIZE, "%li", value); std::cout << BUFFER;

#define CATCH_EXCEPTION(exceptionReason) std::cerr << "Exception catch, file: " << __FILE__ << ", method: " << __FUNCTION__ << ", line: " << __LINE__ << std::endl << exceptionReason << std::endl;


											   class Exception; ///< Exception class and base class for other Exceptions.
											   class NullPointerException;
											   class EmptyArgumentException;
											   class TypeCastException;
											   class InvalidArgumentException;
											   class InvalidFileException;

											   class ParseConfFile; ///< Parse configuration file.
											   class StreamTokenizer; ///< Used by ParseConfFile to get next token on an attribution.

											   /**
											    * \brief A pair of void* to store extra data.
											    */
											   typedef std::pair<void*, void*> extra_data_pair_t;

											   /**
											    * \struct car_gap_t
											    * \brief Gap of a Car.
											    */
											   typedef struct {
												   dist_t frontGap; ///< gap in front.
												   dist_t rearGap; ///< gap in rear.
												   dist_t leftFrontGap; ///< gap in front in next Lane on left.
												   dist_t leftRearGap; ///< gap in rear in next Lane on left.
												   dist_t rightFrontGap; ///< gap in front in next Lane on right.
												   dist_t rightRearGap; ///< gap in rear in next Lane on right.
											   } car_gap_t;

/**
 * \struct nearby_cars_ptr_t
 * \brief Nearby cars, but it stores Car* information.
 */
typedef struct {
	Car *frontCarRef; ///< nearest Car in front.
	Car *rearCarRef; ///< nearest Car in rear.
	Car *leftCarRef; ///< nearest Car in left.
	Car *leftFrontCarRef; ///< nearest Car in front in next Lane on left.
	Car *leftRearCarRef; ///< nearest Car in rear in next Lane on left.
	Car *rightCarRef; ///< nearest Car in right.
	Car *rightFrontCarRef; ///< nearest Car in front in next Lane on right.
	Car *rightRearCarRef; ///< nearest Car in rear in next Lane on right.
} nearby_cars_ptr_t;

/**
 * \struct nearby_cars_t
 * \brief Nearby cars, but ID reference.
 */
typedef struct {
	id_t frontCar; ///< nearest Car in front.
	id_t rearCar; ///< nearest Car in rear.
	id_t leftCar; ///< nearest Car in left.
	id_t leftFrontCar; ///< nearest Car in front in next Lane on left.
	id_t leftRearCar; ///< nearest Car in rear in next Lane on left.
	id_t rightCar; ///< nearest Car in right.
	id_t rightFrontCar; ///< nearest Car in front in next Lane on right.
	id_t rightRearCar; ///< nearest Car in rear in next Lane on right.
} nearby_cars_t;

/**
 * \struct car_state_t
 * \brief Driver related data types.
 *
 * This is the data type used to store a car state.
 * 
 * There are some categories of data:
 * - Simulator internal objects reference.
 * 	- lane
 * - driver
 * - Static data about the Car. It usualy express Car phisical limits:
 * - maxSpeed
 * - maxAccel
 * - Dinamic data about the Car, it can change in every simulation step:
 * - position
 * - accel
 * - speed
 */
typedef struct {
	id_t car; ///< own ID.
	id_t lane; ///< Lane where the Car relies on.
	id_t laneset; ///< LaneSet where the Car relies on.
	id_t driver; ///< Driver that guide the Car.
	speed_t maxSpeed; ///< Car maximum speed.
	accel_t maxAccel; ///< Car maximum acceleration.
	dist_t length; ///< Car length.
	dist_t position; ///< Position, cell count, on current Lane.
	accel_t accel; ///< Acceleration adopted by the Car.
	speed_t speed; ///< Speed assumed by the Car.
	extra_data_pair_t *extraData; ///< A pointer to extra data vector structure, meanless for a Car.
	car_gap_t gap; ///< Gap observed by the Car.
	nearby_cars_t nearbyCars; ///< Cars in the neighborhood.
} car_state_t;

/**
 * \struct driver_decision_t
 * \brief A driver decision will be stored on this data type.
 *
 * There are levels of decision:
 * - Route related:
 *	- lane
 *	- nextLaneSet
 * - Local guiding:
 *	- accel
 *	- speed
 */
typedef struct {
	id_t lane; ///< In which Lane the Car suppose to go.
	id_t nextLaneSet; ///< Next direction on the following Node.
	accel_t accel; ///< Speed increments can be expressed in terms of acceleration, i. e., the amount to be add to the current speed.
	speed_t speed; ///< Next desired speed.
} driver_decision_t;

/***************************************************************************************************
 *************                      Driver Backend Types                               *************
 ***************************************************************************************************/

/**
 * \brief Decision type of next destination, on the following Node
 *
 * It must be on the following order:
 * - lane_laneset_decision_t.first next Lane ID on the next LaneSet
 * - lane_laneset_decision_t.second next LaneSet ID on the folloing Node
 */
typedef std::pair<id_t, id_t> lane_laneset_decision_t;

/**
 * \brief All turning probabilities will be mapped into this strutcture, one for each possibility.
 *
 * The elements are:
 * - turning_probability_pair_t.first LaneSet ID.
 * - turning_probability_pair_t.second turning probability.
 */
typedef std::pair<id_t, double> turning_probability_pair_t;

/**
 * \brief Carry all turning probablities for a LaneSet.
 */
typedef std::list<turning_probability_pair_t> turning_probabilities_t;

/**
 * \brief Carry a vector of objects ID.
 */
typedef std::vector<id_t> object_id_vector_t;

/**
 * \brief Carry a list of objects ID.
 */
typedef std::list<id_t> object_id_list_t;

/**
 * \brief A pair of best position, on a Lane, to insert a Car and it frontal gap.
 */
typedef std::pair<dist_t, dist_t> position_fgap_pair_t;

typedef struct 
{
	int laneset;
	double probability;
} Route;

/***************************************************************************************************
 *************                         Module Related Types                            *************
 ***************************************************************************************************/

/**
 * \brief Traffic light agents: socket <-> nodes map.
 */
typedef std::pair<ServerSocket*, node_vector_t> tlight_agent_t;

/**
 * \brief Traffic light agents: socket <-> nodes map vector.
 */
typedef std::vector<tlight_agent_t> tlight_agents_t;

//dcop specific
typedef std::pair<int, int> copVar;
typedef std::pair<copVar, copVar> copConstraintVars;
typedef std::pair<copConstraintVars, double> copConstraint;
typedef std::vector<copConstraint> copConstraints;

typedef std::pair<Node*, double> copRelation;

typedef struct
{
 Node* node;
 copRelation ns;
 copRelation lo;
 int priority;
} copObject;

typedef std::vector<copObject> copObjects;

typedef struct
{
	int alg;
	double alpha;
	double gammasynch;
	double gammaunsynch;
	double aa;
	double an;
	double na;
	double nn;
} dcopparams;

//end dcop


#endif
