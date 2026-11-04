# Cold Chain Monitor Firmware
Arduino code for cold chain monitoring.

## Requirements

* arduino-cli or arduino IDE

## Compiling

``` bash
arduino-cli compile .

```

## Uploading
``` bash
arduino-cli upload -p <USB-PORT-HERE> --fqbn esp8266:esp8266:nodemcuv2 .

```

### getting arduino's USB connected port

``` bash
# this will output all devices connected 
# usually something like: '/dev/ttyUSB0'
arduino-cli board list

```

## Configuration
To avoid hardcoding addresses and credentials in the firmware, the configuration is stored separately. This allows devices to have different configurations without recompiling the firmware every time.

You can do this by editing `data/config.json`'s placeholder fields with your own desired configuration

> [!WARNING]
> Anything inside `data/` directory **won't** be added when compiling, unless you create and flash an image of it using **LittleFS**.

### Creating the image

``` bash
## This will create a fs.bin image file
<path-to-mklittlefs> -c data -p 256 -b 8192 -s 2072576 fs.bin 

```
**mklittlefs** path can be found by running:

* **Linux** 
``` bash
find ~/.arduino15/packages/esp8266 -type f -name mklittlefs

``` 
* **windows** 
``` powershell
(Get-ChildItem "$env:LOCALAPPDATA\Arduino15\packages\esp8266" -Recurse -Filter mklittlefs.exe).FullName

```


### Flashing to arduino

You can flash using **esptool**

``` bash
esptool --port <USB-PORT-HERE> write_flash 0x200000 fs.bin

```

Or alternatively via a python script

* **Linux**
``` bash
python ~/.arduino15/packages/esp8266/hardware/esp8266/3.1.2/tools/upload.py --chip esp8266 --port <USB-PORT-HERE> --baud 115200 write_flash 0x200000 fs.bin
```
* **Windows**
``` powershell
python "$env:LOCALAPPDATA\Arduino15\packages\esp8266\hardware\esp8266\3.1.2\tools\upload.py" --chip esp8266 --port <USB-PORT-HERE> --baud 115200 write_flash 0x200000 fs.bin
```



## Monitoring

``` bash
arduino-cli monitor -p <USB-PORT-HERE> --config baudrate=115200

```

# Arduino IDE

## Setup

### ESP8266 core

Go to `Tools > Board > Boards Manager` then select "esp8266"

or add `http://arduino.esp8266.com/stable/package_esp8266com_index.json` to `File > Preferences > Additional boards manager URLs`

### Libraries
From `Tools > Manage Libraries` add:
  - PubSubClient (2.8.0)
  - ArduinoJson (7.4.3)
  - DHT sensor library (1.4.7)
  - Adafruit Unified Sensor (1.1.15)
  - NewPing (1.9.7)

## Building and uploading
Open the project and go to `Tools > Board > ESP8266 Boards` then select ESP8266 board; for example: `NodeMCU 1.0 (ESP-12E Module)`

Compile using `Sketch > Verify/Compile` then connect the board to your computer and select a usb port `Tools > Port > YOUR-USB-PORT-HERE` and upload the firmware with `Sketch > Upload`

## Configuration
Edit `data/config.json` with your own values. 

To flash the configuration install [Arduino LittleFS Upload](https://github.com/earlephilhower/arduino-littlefs-upload) plugin to your IDE and create/flash an image by pressing `Ctrl + Shift + P` then select `Upload LittleFS to Pico/ESP8266/ESP32`

> [!NOTE]
> Close the 'Serial Monitor' panel before flashing or it will fail with a 'port-busy' error
