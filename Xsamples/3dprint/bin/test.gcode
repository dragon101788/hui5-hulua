M109 S210.000000
;Sliced at: Sat 06-06-2015 21:27:36
;Basic settings: Layer height: 0.1 Walls: 2.4 Fill: 20
;Print time: 1 hour 16 minutes
;Filament used: 2.491m 7.0g
;Filament cost: None
;M190 S0 ;Uncomment to add your own bed temperature line
;M109 S210 ;Uncomment to add your own temperature line
G21        ;metric values
G90        ;absolute positioning
M82        ;set extruder to absolute mode
M107       ;start with the fan off
G28 X0 Y0  ;move X/Y to min endstops
G28 Z0     ;move Z to min endstops
G1 Z15.0 F9000 ;move the platform down 15mm
G92 E0                  ;zero the extruded length
G1 F200 E3              ;extrude 3mm of feed stock
G92 E0                  ;zero the extruded length again
G1 F9000
;Put printing message on LCD screen
M117 Printing...

;Layer count: 198
;LAYER:-2
;RAFT
G0 F9000 X85.000 Y85.000 Z0.300
;TYPE:SUPPORT
G1 F1200 X115.000 Y85.000 E3.74177
G1 X115.000 Y115.000 E7.48353
G1 X85.000 Y115.000 E11.22530
G1 X85.000 Y85.000 E14.96706
G0 F9000 X85.500 Y84.850
G1 F1200 X85.500 Y115.150 E18.74624
G0 F9000 X88.500 Y115.150
G1 F1200 X88.500 Y84.850 E22.52543
G0 F9000 X91.500 Y84.850
G1 F1200 X91.500 Y115.150 E26.30461
G0 F9000 X94.500 Y115.150
