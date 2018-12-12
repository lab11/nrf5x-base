Utility to generate production configuration block
============

This tool reads input such as desired channel masks, installation key etc from file
and generates binary block, which, if uploaded to device shall update ZigBee stack and
application configuration

=============
Help
 
To see available options and list of supported applications:
$ production_config_generator.exe -h

Currently supported applications:
- generic-device
- ias_zone_sensor
- smart_plug

See input.txt for input format list of parameters that can be set in production configuration.

Note that currently maximum production configuration size (stack+application+header) is 132 bytes
