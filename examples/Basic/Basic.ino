#include <WiFiManagerExt.h> // https://github.com/tvaira/WiFiManagerExt

#define GPIO_BOOT 0 // BOOT button

WiFiManagerExt wm;
//WiFiManagerExt wm(GPIO_BOOT); // use the BOOT button to perform a default reset

void doReset() {  
    wm.defaultReset();
}

void setup() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

    // put your setup code here, to run once:
    Serial.begin(115200);
    
    //reset settings - wipe credentials for testing
    //wm.resetSettings();

    wm.setTitle("BTS SN LaSalle Avignon");
    //wm.setDarkMode(true);

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    //res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

    //wm.setHoldResetButtonTime(3); // keep pressing for at least 3 seconds (default)
    //wm.setBeforeResetTime(2); // wait 2 seconds before resetting the ESP32 card (default)
    //wm.setActiveDefaultReset(true); // active a default reset or use callback
    wm.setDefaultResetCallback(doReset);
}

void loop() {
    // put your main code here, to run repeatedly    

    // default reset request ?
    wm.isDefaultReset();
}
