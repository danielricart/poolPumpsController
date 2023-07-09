Motor pumps side
--- 
# Connections:
*I1*: Pump Sensor

*RELAY1*: Motor Driver

# LEDs:
## Reset: 
  Green-> Connection established to remote board

  Red -> Cannot connect to remote board

## Status:
1: Pump is read ON

2: Motor is ON

3: NC

4: Overriden. Pump is assumed always ON. Pump Input is ignored

# Buttons:
## USER
Long press (2 seconds) toggles OVERRIDE. Check LED4.

# Behaviour:
## Automatic (default mode):

- When I1 is HIGH for MOTOR_DELAY_ON milliseconds, it sets HIGH RELAY OUTPUT 1.
- When I1 is LOW for MOTOR_DELAY_OFF milliseconds, it sets LOW RELAY OUTPUT 1.

## Overriden:
- Assumes pump is always on
- turns on motor
 
