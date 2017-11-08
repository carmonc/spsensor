/*! @file
*/

#include <Arduino.h>
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
#include <SoftwareSerial.h>
#endif

#define USE_BLE


#ifdef USE_BLE
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"
/*=========================================================================
  APPLICATION SETTINGS

      FACTORYRESET_ENABLE       
  
      Perform a factory reset when running this sketch
     
  Enabling this will put your Bluefruit LE module in a 'known good' state and
  clear any config data set in previous sketches or projects, so running this
  at least once is a good idea.          
  
  When deploying your project, however, you will want to disable factory reset
  by setting this value to 0.  If you are making changes to your Bluefruit LE
  device via AT commands, and those changes aren't persisting across resets,
  this is the reason why.  Factory reset will erase the non-volatile memory
  where config data is stored, setting it back to factory default values.

  Some sketches that require you to bond to a central device (HID mouse,
  keyboard, etc.) won't work at all with this feature enabled since the factory
  reset will clear all of the bonding data stored on the chip, meaning the
  central device won't be able to reconnect.

  MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
  MODE_LED_BEHAVIOUR        LED activity, valid options are
  "DISABLE" or "MODE" or "BLEUART" or
  "HWUART"  or "SPI"  or "MANUAL"
  -----------------------------------------------------------------------*/
#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/
#endif

#include <stdlib.h>
#include "vesselInfo.h"

#ifdef USE_BLE
extern Adafruit_BluefruitLE_SPI ble;

// A small helper
void error(const __FlashStringHelper* err) {
   Serial.println(err);
   while (1);
}
#endif

void enableBTModule()
{
#ifdef USE_BLE
   Serial.begin(F("Initializing Bluefruit LE Module"));
   Serial.begin(115200);

   if(!ble.begin(VERBOSE_MODE))
   {
      error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
   }
   
   Serial.println( F("Initialization Complete: OK!") );

   if( FACTORYRESET_ENABLE )
   {
      Serial.println(F("Performing factory reset of BLE."));
      if(! ble.factoryReset() ) 
      {
         error(F("Could not factory reset device."));
      }
   }

   ble.echo(false);

   ble.info();

   Serial.println(F("Ready to communicate via Bluetooth!"));
   Serial.println(F("Please use Adafruit LE App to connect in UART mode"));
   Serial.println(F("Then enter characters to send to Bluefruit"));

   ble.verbose(false);

   while(!ble.isConnected()) {
      delay(500);
   }

   if(ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION)) 
   {
      //Change Mode LED Activity
      Serial.println(F("*******************************"));
      Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
      ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
      Serial.println(F("*******************************"));
   }
#endif

}

/*! @brief This function is run once at program startup and can be used to
 * initialize variables and application data.
 *
 */
void setup()
{
   // Very tight loop here!
   while(!Serial); //Required for FLora & Micro
   
   delay(500);

   Serial.begin(9600);

   enableBTModule();

   initializeVessel();
}

/*!
 * @brief This function is called repeatedly and contains the main logic of the
 *        parent application.
 *
 *        This function is called repeatedly and contains the main logic of the
 *        parent application. Program logic that will be called repeatedly
 *        should be placed in this function.
 *
 *
 */
void loop() {

   computeLevel();

   delay(500);
}

