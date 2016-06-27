Test for Advertising Man. Specific Data And a Service
=====================================================

Example for sending custom data in an advertisement.

When this app is running, it should advertise `0x02 0xE0 0x01 0x02`
in the manufacturer specific data section of the advertisement.
`0x02E0` is the manufacturer ID for the University of Michigan,
and the next two bytes can be anything. It should also
advertise the service short UUID `0xBEAF`.
