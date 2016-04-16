# Source2016
Source Code for NASA RMC 2016
# Packet Definitions
## Arduino/Teensy
### Teensy to Server Communication
#### Receiving Data
|Command|Regular Expression |Value  |Purpose|
|:------|:----------------: |:---:  |-------|
|C # +/-    |C\d[+-]        |+ = ON , - = OFF, # = 1-6  |Control Single Conveyor Motors(1-5) and Digger Motor(6)|
|C +/-      |C[+-]          |+ = All On , - = All off   |Control All Conveyor Motors    |
|L +/-      |L[+-]          |+ = UP , - = DOWN  |Set Dumper Linear Actuator UP or DOWN  |
|P #        |P\d{1,2}       |# = 0-90           |Sets Digger Position with an angle via Linear Actuator |
|D #        |D\d{1,2}       | # = 0-50         |Control Rack and Pinion Motor|
|W A/F/R    |W[AFR]         |A = All;F = Front;R = Rear |Sets Robot Drive Mode  |
|M # +/- #  |M\d[+-]\d{1,3} |1st # = 1-2, 2nd # = 0-100 |Sets Drive Motor Output |
|H          | H             |   | Halts robot actions the Robot
|N          | N             |   | Resets the Robot

#### C
Motors are ordered front to rear.
1 is front most and 5 is rear most.

---
#### Transmitting Data

|Command|Regular Expression |Value  |Purpose|
|:------|:----------------: |:---:  |-------|
|S [+-] #  |S[+-]\d{1,3} | + = Digger, - = First Conveyor Motor, # = 0-255 | Tells how many amps in motor circuit|
|E # +/- #  |E\d[+-]\d{1,4} | 1st # = 1-3, 2nd # = 0-9999|Tells how many counts the encoders have turned|
#### S
1. Digger Motor
2. First Conveyor

#### E
1. Front Left Motor
2. Front Right Motor
3. Rack and Pinion

## Server PC
### Server to Teensy Communication
- Flip the Transmitting and Receiving Data for the Teensy
- Forward the Client data to Teensy

### Server to Client
- Include Server to Teensy Communication & exclude the Transmitting Data section

#### Receiving Data
- It is the same data as the Teensy

#### Transmitting Data

|Command|Regular Expression |Value  |Purpose|
|:------|:----------------: |:---:  |-------|
|V #    |V\d{1,2}   |# = 0-99   |Tells how much amps is on the first Conveyor|
|I #    |I\d{1,2}   |# = 0-99   |Tells how many amps is being pulled on Digger|
|D #    |D\d{1,3}   |# = 0-105  |Tells how far the rack and pinion is in cm|
|L #    |L\d{1,4}   |# = 0-9999 |Tells how far the Left Motor is in cm|
|R #    |R\d{1,4}   |# = 0-9999 |Tells how far the Right Motor is in cm|
## Client PC
### Client to Server Communication
#### Transmitting
- The same data receiving as Teensy to Server

#### Receiving
- Same as data Transmitting as Server to Client
