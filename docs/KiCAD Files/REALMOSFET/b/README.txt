IN PCB FILE
- the +5v connections may look unwired but that is because in the electrical layout  
both of the discharge and charge use +5v, so it automatically assumes they are the same thing.

- The component numbers are all over the place so there is no logic for that, change if you'd like.


-Since we are using the spider 8-pin MOSFET instead of a 3-pin MOSFET it is possible to make a connection under the component.

-It is possible to take the Q2/Q1/Q7/Q8 section and put it horizontally next to the other MOSFET section, but having an inside/outside logic for the charge/discharge is bad because it can cause noise, therefore left/right is the suggested choice.

-KICAD ALLOWS YOU TO HIGHLIGHT THE COMPONENT ON THE SCHEMA, IT WILL ALSO BE HIGHLIGHTED ON THE PCB FILE IF YOU HAVE BOTH OPEN.

-Ground is not done