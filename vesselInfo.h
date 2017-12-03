/*! @file
 */

#ifndef VESSELINFO_H_INCLUDED
#define VESSELINFO_H_INCLUDED

#define MAX_VESSEL_NAME 30

/*! @brief This enum defines the availabled types of vessels to monitor.
 *
 */
typedef enum vessel_type_e 
{
   VESSEL_TYPE_GENERIC_E,       //< Generic Level Tank >
   VESSEL_TYPE_END_E            //< The last item in the list >
} vessel_type_t;

/*! \enum
 * \brief This enum defines the types of activities that a vessel can perform.
 *    - Filling is the state in which it's content type amount is increasing.
 *    - Discharging is the state in which its content type amount is decreasing.
 *    - Offline is the state in which the device is not communicating.
 *    - Stable is the state in which no detectable increase or decrease in level is detectable. 
 */
typedef enum vessel_activity_e {
   VESSEL_ACTIVITY_FILLING_E = 0,
   VESSEL_ACTIVITY_DISCHARGING_E,
   VESSEL_ACTIVITY_END_E
} vessel_activity_t;

/*! \struct
 * \brief This struct defines the 
 *
 */
typedef struct vessel_s 
{ 
   char name[MAX_VESSEL_NAME];     //< The name of the vessel >
   int capacity;                 //< Capacitu of the vessel in US gallons > 
   int level;                    //< The current level of the vessel >
   vessel_type_t vesselType;      //< Enumeration of the vessel type >
   vessel_activity_t activity;  //< Enumeration of the activity the vessel is engaging in >
} vessel_t;

extern vessel_t vesselGeneric;

const char* VESSEL_GENERIC_NAME = "GEN";  

void initializeVessel();
void computeLevel();

#endif

