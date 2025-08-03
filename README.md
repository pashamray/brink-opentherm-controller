# Brink OpenTherm controller

## Device Features

The device supports the following features:

- [x] Accepting commands
    - [x] `ping` – responds with `pong`, used for testing  
    - [x] `restart` – restart ESP32 controller
    - [x] `set` – sets parameters  
        - [x] `set speed 0, 10, 50, 100...`  
        - [x] `set wifi ssid@pass`  
    - [x] `get` – reads parameters  
        - [x] `get speed`  
        - [x] `get wifi`
- [x] Saving settings to NVS ([Preferences](https://docs.arduino.cc/libraries/preferences/))  
- [x] Device control via serial port  
- [ ] Device control via web interface  
- [ ] Device control via app local network (offline)  
- [ ] Device control via app global network (online)  

## Hardware
- [Wemos D1 Mini](https://www.wemos.cc/en/latest/d1/d1_mini.html) or compatible
- [OpenTherm master shield (ihormelnyk)](https://ihormelnyk.com/opentherm_adapter) or
- [OpenTherm master shield (diyless)](https://diyless.com/product/master-opentherm-shield)

## Firmware
Arduino IDE

#### Libs
https://github.com/ihormelnyk/opentherm_library

#### References
- https://otgw.tclcode.com/
- https://github.com/jpraus/arduino-opentherm
- https://portegi.es/blog/opentherm-wtw-1
- https://portegi.es/blog/opentherm-wtw-2
- https://github.com/Sidiox/opentherm_library
- https://github.com/Sidiox/hrv-control
- https://github.com/tijsverkoyen/Home-Assistant-BrinkRenoventHR
- https://github.com/raf1000/brink_openhab
- https://github.com/ihormelnyk/opentherm_library
- https://ihormelnyk.com/opentherm_adapter
- https://github.com/Jeroen88/EasyOpenTherm
- https://www.opentherm.eu/wp-content/uploads/2016/06/OpenTherm-Function-Matrix-v1_0.xls
