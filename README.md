### weather-station

#### description

small repo which contains the code that runs on the board (the cpp code) and the code for the api (flask+sqlalchemy).

the setup is fairly simple:

every half hour, the script on the board runs, collects the data from the sensor (via the gpio pins) and sends it back
to an api. the api inserts the temps/pressure/etc in the db. 

from there on, the idea is to set up alerts for when the batteries are low (which might be harder than anticipated as
it's not particularly easy to get the battery level via the jst connector) or making graphs based on the data. the 
possibilities are endless!

#### components used:

- [adafruit feather huzzah with ESP8266 wifi headers](https://www.kiwi-electronics.nl/adafruit/adafruit-development-boards/assembled-adafruit-feather-huzzah-with-esp8266-wifi-with-headers)
- [BME680 temperature/humidity/pressure/gas sensor](https://www.antratek.nl/bme680-temperature-humidity-pressure-and-gas-sensor)
- [battery holder with jst connector](https://www.kiwi-electronics.nl/componenten-onderdelen/batterijhouders/3x-aaa-battery-holder-with-jst)

#### connections: 

- Connect Vin to the power supply, 3V or 5V is fine. Use the same voltage that the microcontroller logic is based off of
- Connect GND to common power/data ground
- Connect the SCK pin to Digital #13 but any pin can be used later
- Connect the SDO pin to Digital #12 but any pin can be used later
- Connect the SDI pin to Digital #11 but any pin can be used later
- Connect the CS pin Digital #10 but any pin can be used later

the pins must match the BME_* constants in sensor.cpp:

```
#define BME_SCK 14
#define BME_MISO 12
#define BME_MOSI 13
#define BME_CS 15 
```

#### todo:

- [ ] solder the connections
- [ ] find a way to reliably read battery levels
- [ ] "deploy" collector on the raspberry pi
- [ ] graphs?