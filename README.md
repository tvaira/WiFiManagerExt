# WiFiManagerExt

## Présentation

`WiFiManagerExt` est une extension de `WiFiManager` permettant d'intégrer un bouton pour effectuer un effacement des paramètres WiFi.

Lien : [WiFiManager](https://github.com/tzapu/WiFiManager)

Cela fonctionne pour une carte ESP32 avec la plateforme Arduino : [https://github.com/espressif/arduino-esp32](https://github.com/espressif/arduino-esp32).

## Utilisation

La broche du bouton utilisé se fait à l'instanciation :

```cpp
#include <WiFiManagerExt.h>

#define GPIO_X x // Button

WiFiManagerExt wm(GPIO_X); // use the button on GPIO_X to perform a default reset
```

Sinon, c'est le bouton nommé BOOT (GPIO0) qui est utilisé par défaut :

```cpp
#include <WiFiManagerExt.h>

WiFiManagerExt wm; // use the BOOT button to perform a default reset
```

Il faut maintenir l'appui pendant un certain temps pour déclencher la prise en compte du reset. Ce temps est fixé à 3 secondes par défaut. On peut le modifier avec `setHoldResetButtonTime()`.

La prise en compte du reset se fait en appelant `isDefaultReset()` dans `loop()`. Cela déclenchera un effacement des paramètres WiFi et un redémarrage si `setActiveDefaultReset(true)` a été réalisé. Sinon, il est possible d'installer une fonction de rappel pour programer sa propre séquence de reset :

```cpp
void doReset()
{  
    wm.defaultReset();
}

wm.setDefaultResetCallback(doReset);
```

Pour le reste, c'est `WiFiManager` qui effectue le travail.

<img src="https://github.com/tvaira/WiFiManagerExt/raw/main/screenshots/WiFiManagerExt.gif" alt="WiFiManagerExt" width="50%" height="50%">

### Exemple d'utilisation

```cpp
#include <WiFiManagerExt.h> // https://github.com/tvaira/WiFiManagerExt

WiFiManagerExt wm; // use the BOOT button to perform a default reset

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
```

Pour la bibliothèque `WiFiManager`, voir aussi les [exemples](https://github.com/tzapu/WiFiManager/tree/master/examples).

### PlatformIO

Pour utiliser `WiFiManagerExt` avec [PlatformIO](https://platformio.org/), il faut l'indiquer dans le fichier `platformio.ini` comme ceci :

```
[env]
lib_deps =
	https://github.com/tvaira/WiFiManagerExt.git
```

Sinon, pour utiliser seulement `WiFiManager` :

```
[env]
lib_deps =
	https://github.com/tzapu/WiFiManager.git
```
