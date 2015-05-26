minLOGGER is a logger realized with an ATmega328p using a MicroSD, barometer and GPS.

It is usable with NMEA, UBX and the special DJI GPS protocols.

The data is logged as plain text csv file which can be converted to e.g KML with free available tools.

The converted data can be visualized in e.g. google earth.

This project is GNU GPL v3 and using it is at your own risk.


---


Status:

  * ATmega328p and MicroSD tested and working with UBX and DJI GPS

  * BMP085 baro not ready yet, needs 5 minutes for stable measurements, maybe a better one has to be used

  * 10DOF module with MS5611 baro (and some other nice gadgets) ordered, waiting for delivery ... waiting ... waiting ...


---


Preliminary connection overview:

![http://minlogger.googlecode.com/svn/wiki/images/001_Connections.jpg](http://minlogger.googlecode.com/svn/wiki/images/001_Connections.jpg)