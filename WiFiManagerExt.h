/**
 * WiFiManagerExt.h
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

#ifndef WiFiManagerExt_h
#define WiFiManagerExt_h

#include <WiFiManager.h>
#include <FunctionalInterrupt.h> // for std::bind

#define DEFAULT_GPIO_BOOT 0       //!< BOOT Button is reset button (default)

#define HOLD_RESET        3000    //!< hold the reset button for at least 3 seconds (default)
#define BEFORE_RESET      2000    //!< wait 2 seconds before resetting the ESP32 board (default)

class WiFiManagerExt : public WiFiManager
{
  public:
    WiFiManagerExt(Stream& consolePort, int resetButtonPin=DEFAULT_GPIO_BOOT);
    WiFiManagerExt(int resetButtonPin=DEFAULT_GPIO_BOOT);
    virtual ~WiFiManagerExt();
    void WiFiManagerExtInit();
    
    void setBeforeResetTime(unsigned long seconds=BEFORE_RESET);
    void setHoldResetButtonTime(unsigned long seconds=HOLD_RESET);
    void setActiveDefaultReset(bool active);
    //called when reset button have been triggered
    void setDefaultResetCallback( std::function<void()> func );
    void defaultReset();    
    void isDefaultReset();

  private:
    unsigned long _holdResetButtonTime;
    unsigned long _beforeResetTime;
    int _resetButtonPin;
    bool _activeDefaultReset;
    std::function<void()> _defaultresetcallback; // callback
    volatile bool _lastState;
    volatile bool _reset;
    volatile uint32_t _startTriggerTime;
    volatile int32_t _holdDuration;
    
    void IRAM_ATTR interruptResetButton(); // interrupt routine    
};

#endif
