Motor pumps side
--- 
# Connections:
*I1*: Water Level Sensor

*RELAY1*: Chlorine Driver

# LEDs:
## Reset: 
  Green-> Connection established to remote board

  Red -> Cannot connect to remote board

## Status:
1: Water Level is read ON

2: Remote Motor is ON

3: Chlorine is ON

4: Overriden. Pump is assumed always ON. Pump Input is ignored

# Buttons:
## USER
Long press (2 seconds) toggles OVERRIDE. Check LED4.

# Behaviour:
## Automatic (default mode):

- When I1 is HIGH for MOTOR_DELAY_ON milliseconds, it sends a remote command to turn ON motor.
- When Remote Command is confirmed, it sets HIGH RELAY OUTPUT 1.
- When I1 is LOW for MOTOR_DELAY_OFF milliseconds, it sends a remote command to turn OFF motor.
- When Remote command is confirmed, it sets LOW RELAY OUTPUT 1.

Remote command to turn on motor is tied to water level input. 
Chlorine pump is tied to receiving Motor Confirmation from remote end. 


## Overriden:
- Assumes water level is always on
- sends remote command to turn on motor


