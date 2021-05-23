/**
 * WiFiManagerExt.cpp
 * 
 * WiFiManager, a library for the ESP8266/Arduino platform
 * for configuration of WiFi credentials using a Captive Portal
 *
 * @author Creator tzapu
 * @author tablatronix
 *
 * WiFiManagerExt add reset button
 * @author tvaira
 * @version 1.0.0
 * @license MIT
 */

#include "WiFiManagerExt.h"

static portMUX_TYPE mutex = portMUX_INITIALIZER_UNLOCKED;

/**
 * --------------------------------------------------------------------------------
 *  WiFiManagerExt
 * --------------------------------------------------------------------------------
**/

void IRAM_ATTR WiFiManagerExt::interruptResetButton()
{  
  portENTER_CRITICAL_ISR(&mutex);
  _lastState = digitalRead(_resetButtonPin);
  if(!_lastState) {
    _startTriggerTime = xTaskGetTickCount();
  } else {
    _holdDuration = xTaskGetTickCount() - _startTriggerTime;
    if(_holdDuration >= _holdResetButtonTime)
      _reset = true;
    else 
      _reset = false;
  }
  portEXIT_CRITICAL_ISR(&mutex);
  #ifdef WM_DEBUG_LEVEL
  if(_lastState)
  {
    Serial.print("RESET BUTTON -> ");
    Serial.print(_holdDuration);
    Serial.print(" ms >= ");
    Serial.print(_holdResetButtonTime);
    Serial.print(" ms = ");
    Serial.print(_reset);
    Serial.println( " ?");
  }
  #endif
}

// constructors
WiFiManagerExt::WiFiManagerExt(Stream& consolePort, int resetButtonPin):WiFiManager(consolePort),_holdResetButtonTime(HOLD_RESET),_beforeResetTime(BEFORE_RESET),_resetButtonPin(resetButtonPin),_activeDefaultReset(false),_defaultresetcallback(NULL),_reset(false),_startTriggerTime(0),_holdDuration(0) {
  WiFiManagerExtInit();
}

WiFiManagerExt::WiFiManagerExt(int resetButtonPin):WiFiManager(),_holdResetButtonTime(HOLD_RESET),_beforeResetTime(BEFORE_RESET),_resetButtonPin(resetButtonPin),_activeDefaultReset(false),_defaultresetcallback(NULL),_reset(false),_startTriggerTime(0),_holdDuration(0) {
  WiFiManagerExtInit();  
}

void WiFiManagerExt::WiFiManagerExtInit() {
  pinMode(_resetButtonPin, INPUT);
  pinMode(_resetButtonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(_resetButtonPin), std::bind(&WiFiManagerExt::interruptResetButton, this), CHANGE);
}

// destructor
WiFiManagerExt::~WiFiManagerExt() {
}

void WiFiManagerExt::setBeforeResetTime(unsigned long seconds) {
  _beforeResetTime = seconds * 1000;
}

void WiFiManagerExt::setHoldResetButtonTime(unsigned long seconds) {
  _holdResetButtonTime = seconds * 1000;
}

void WiFiManagerExt::setDefaultResetCallback( std::function<void()> func ) {
  _defaultresetcallback = func;
}

void WiFiManagerExt::setActiveDefaultReset(bool active) {
    _activeDefaultReset = active;
}

void WiFiManagerExt::defaultReset() {  
  delay(_beforeResetTime);
  resetSettings();
  reboot();
}

void WiFiManagerExt::isDefaultReset() {  
  bool active = false;
  portENTER_CRITICAL_ISR(&mutex);
  if(_reset) {
    active = true;
    _reset = false;
  }
  portEXIT_CRITICAL_ISR(&mutex);
  
  if(active) {    
    if ( _defaultresetcallback != NULL) {
        #ifdef WM_DEBUG_LEVEL
        Serial.println("Callback Reset");
        #endif
        _defaultresetcallback();
    } else {        
        if(_activeDefaultReset)
        {
            #ifdef WM_DEBUG_LEVEL
            Serial.println("Default Reset");
            #endif
            defaultReset();
        }
    }
  }  
}
