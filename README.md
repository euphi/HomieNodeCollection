# HomieNodeCollection

This is a collection of useful nodes for Homie. It makes use of object-orientation, so each HomieNode class can work standalone.

## Sensornode

This Node pubishes temperature and humidity to a MQTT server according to the Homie convention. There are not settable properties.

## RGBWNode

This node is meant to connect LED strips. The current value for R, G, B, and W is settable as percentage from 0% to 100%.
The percentage is converted to a gamma-corrected value for the ESP8266 PWM outputs.
The output pin for each R, G, B, or W value can be set (or disabled) individually.
To connect even more LED strips, a second instance can be instantiated.

## InputPinArrayNode

Reads input GPIOs and publish them as "OPEN" or "CLOSED" on MQTT according to Homie convention. It also supports an callback function the connect other Nodes. See https://github.com/euphi/HomieNodeCollection/blob/master/examples/InputPinArray_Example.cpp for an example how to switch LED strips (RGGWNode) based on input pins.

## RelaisNode

HomieNode that exposes 16 outputs of a PCF8575 module to the Homie MQTT broker.
_Note_ : No longer maintained, because I made some bad experience with the reliability of the PCF8575. Therefore I use MCP23017 now. See my project https://github.com/euphi/ESP_Heizungscontroller/tree/develop_v3_mcp23017 about how to use it.



## LoggerNode

Please note: LoggerNode is no longer a part of HomieNodeCollect, but an own library now. See https://github.com/euphi/HomieLoggerNode
