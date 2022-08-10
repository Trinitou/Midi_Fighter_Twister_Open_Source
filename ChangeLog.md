# Changes in 2022_08_10

## New native mode

* Native mode can be entered/left via fixed SysEx commands
* As long as native mode is active, it is possible to
  * change the individual knob led properties on the fly:
    * indicator appearance (via SysEx)
    * indicator position (via cc)
    * RGB led colors (via SysEx)
  * receive user interaction:
    * relative knob-turning (via cc)
    * knob button pressed states (via cc)
    * side button pressed states (via cc)
* All native mode SysEx communication uses hard-coded SysEx/cc addresses
  * This allows for reliable integration into other software
