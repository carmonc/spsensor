/*! @file
 */

#ifndef RVTANKINFO_H_INCLUDED
#define RVTANKINFO_H_INCLUDED

#define MAX_TANK_NAME 30

/*! @brief This enum defines the availabled types of tanks to monitor.
 *
 */
typedef enum tank_type_e 
{
   TANK_TYPE_GENERIC_E,       //< Generic Level Tank >
   TANK_TYPE_END_E            //< The last item in the list >
} tank_type_t;

/*! \enum
 * \brief This enum defines the types of activities that a tank can perform.
 *    - Filling is the state in which it's content type amount is increasing.
 *    - Discharging is the state in which its content type amount is decreasing.
 *    - Offline is the state in which the device is not communicating.
 *    - Stable is the state in which no detectable increase or decrease in level is detectable. 
 */
typedef enum tank_activity_e {
   TANK_ACTIVITY_FILLING_E = 0,
   TANK_ACTIVITY_DISCHARGING_E,
   TANK_ACTIVITY_END_E
} tank_activity_t;

/*! \struct
 * \brief This struct defines the 
 *
 */
typedef struct tank_s 
{ 
   char name[MAX_TANK_NAME];     //< The name of the tank >
   int capacity;                 //< Capacitu of the tank in US gallons > 
   int level;                    //< The current level of the tank >
   tank_type_t tankType;      //< Enumeration of the tank type >
   tank_activity_t activity;  //< Enumeration of the activity the tank is engaging in >
} tank_t;

extern tank_t tankGeneric;

const char* TANK_GENERIC_NAME = "GEN";  

void initializeVessel();
void computeLevel();

#endif

