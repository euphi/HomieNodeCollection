# HomieNodeCollection

This is a collection of useful nodes for Homie. It makes use of object-orientation, so each HomieNode class can work standalone.

## Sensornode

This Node pubishes temperature and humidity to a MQTT server according to the Homie convention. There are not settable properties.

## RGBWNode

This node is meant to connect LED strips. The current value for R, G, B, and W is settable as percentage from 0% to 100%.
The percentage is converted to a gamma-corrected value for the ESP8266 PWM outputs.
The output pin for each R, G, B, or W value can be set (or disabled) individually.
To connect even more LED strips, a second instance can be instantiated.

## RelaisNode

HomieNode that exposes 16 outputs of a PCF8575 module to the Homie MQTT broker.
Inputs are not yet supported (*TODO!*).

## LoggerNode

Helper HomieNode that can be used for logging.
The log level property is settable.

