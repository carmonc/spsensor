/*! @file
 */

#include <stdlib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define USE_BLE

#ifdef USE_BLE
#include "BluefruitConfig.h"
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
#endif

#include "vesselInfo.h"

//Yes, this is global
vessel_t vesselGeneric = {0};

/*!
 * @brief This function initializes the various vessels with their capacities
 *        and current levels.
 *
 *        This function also sets the vessel activity to filling as an
 *        overfull vessel can be the most difficult problem to deal with. The
 *        system will adjust itself accordingly through various means. The
 *        state will change to discharging, if appropriate, through normal
 *        application use.
 */
void initializeVessel()
{
   // worst case is filling on init. 
      vessel_t* subject = &vesselGeneric;
      // Set common values
      subject->vesselType = VESSEL_TYPE_GENERIC_E;
      subject->activity = VESSEL_ACTIVITY_FILLING_E; 
      subject->level = 0;//FIXME! Make a function that fetches current level
      memset(subject->name, 0, MAX_VESSEL_NAME);
      subject->capacity = 80;
      strncpy(subject->name, VESSEL_GENERIC_NAME, MAX_VESSEL_NAME);
}

/**************************************************************************/
/*!
    @brief  Checks for user input (via the Serial Monitor)
*/
/**************************************************************************/
bool getUserInput(char buffer[], uint8_t maxSize)
{
   // timeout in 100 milliseconds
   TimeoutTimer timeout(100);

   memset(buffer, 0, maxSize);
   while( (!Serial.available()) && !timeout.expired() ) 
   { 
      delay(1); 
   }

   if ( timeout.expired() ) 
      return false;

   delay(2);
   uint8_t count=0;
   do
   {
      count += Serial.readBytes(buffer+count, maxSize);
      delay(2);
   } while( (count < maxSize) && (Serial.available()) );

   return true;
}

/*!
 * @brief This method computes the vessel levels and handles the level as
 *        appropriate.
 *
 *        This method computes the vessel levels and handles the level as
 *        appropriate. If the level is too high it should alarm the user.
 *
 *        If the level reaches zero (0) then it should alarm the user by either
 *        audible alerts and/or email/SMS messages.
 */
void computeLevel()
{
   vessel_t* subject = &vesselGeneric;
   char msg[BUFSIZE+1] = {0};
   char inputs[BUFSIZE+1]={0};
   int r = rand()%5+1;

   if(subject->activity == VESSEL_ACTIVITY_FILLING_E)
      subject->level+=r;
   else
      subject->level-=r;

   if( subject->level >= 100 )
      subject->activity = VESSEL_ACTIVITY_DISCHARGING_E;
   else if( subject->level <= 0 )
      subject->activity = VESSEL_ACTIVITY_FILLING_E;
   
   sprintf(msg, "-%s:%i-\n\n", subject->name, subject->level);
   Serial.println(msg);
   
#ifdef USE_BLE
//      if( getUserInput(inputs, BUFSIZE) ) 
   {
      //Send characters to from Arduino, to BTLE, to android app
      ble.print("AT+BLEUARTTX=");
      ble.println(msg);
      //Wait for a response
      if(!ble.waitForOK() )
      {
         Serial.println(F("Failed to send vessel data?"));
      }
      
//         ble.print("AT+BLUEARTTX=");
//         ble.println(inputs);
//         //Wait for a response
//         if(!ble.waitForOK() )
//         {
//            Serial.println(F("Failed to send user input?"));
//         }
   }
#endif

   //If we needed to, write code to take inputs from Android app here
   // Check for incoming characters from Bluefruit
   ble.println("AT+BLEUARTRX");
   ble.readline();
   if (strcmp(ble.buffer, "OK") == 0) {
      // no data
      return;
   }

   // Some data was found, its in the buffer
   Serial.print(F("[Recv] ")); 
   Serial.println(ble.buffer);
   ble.waitForOK();
}

