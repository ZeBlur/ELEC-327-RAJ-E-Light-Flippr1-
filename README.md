# RAJ-E-Light-Flippr1-
RAJ (Raj Adam Jason) Group LLC

The RAJ _E-Light Flippr1™_ is a convenient, cost-effective, and modular solution to wasted energy. 

## Description
### Introduction:
Want to turn on or off the lights from the comfort of your bed?

Want the lights to turn on automatically as you enter the room?

Always forgetting to turn off the lights when you leave the room?

Don't want the hassle or upfront cost of re-wiring the electrical systems in your building of choice?

RAJ Group LLC's **NEW** _E-Light Flippr1™_ is **THE** smart lighting solution for you! 

### Features and General Description
Note: Section adapted from Final Project Proposal

Using a threefold combo of a handheld infrared remote, infrared receiver, and micro-servo in conjunction with a small microcontroller, the _E-Light Flippr1™_ allows you to simply point the handheld remote in the direction of the light switch to toggle the light switch from afar (or up close)! Furthermore, our state of the art motion detection technology, the _E-Light Flippr1™_ can detect when nobody is around and automatically turn off the lights for you! And if you want to simply toggle the light switch at the wall, the _E-Light Flippr1™_ has a panel-mounted button that effortlessly toggles the switch!

Wondering how it works? When you point the handheld infrared remote at the _E-Light Flippr1™_, the infrared receiver will detect this and send the appropriate signal to our microcontroller. That signal will be processed and relayed through the microcontroller to the servo, which will then flip the switch to the desired state. Also, if no motion has been detected for approximately 3 minutes, the motion sensor will relay a signal to the device, automatically turning off the lights! 

## Components:
1x TI LaunchPad kit with MSP430 MCU

1x 5C-SR501 PIR Motion Sensor

1x TSOP34440 Infrared Receiver

1x SG90 Micro Servo

1x USB-A to Micro USB cable

1x 4-pin button

1x INIU BI-B41 Power Bank (37 Watt-hours)

1x 17x10 Breadboard

1x RAJ Silicon Group Custom Enclosure

## System Architecture
### Hardware Architecture 
As seen in the general description and components list above, the _E-Light Flippr1™'s_ hardware can be separated into seven primary components:

System Control:
- MSP430 MCU

System Inputs:
- IR Sensor
- Motion Sensor
- General Purpose Button

System Outputs: 
- Micro Servo

Power/Auxillary:
- INIU BI-B41 Power Bank
- RAJ Silicon Group Custom Enclosure

Inputs and outputs are powered and connected via jumper wires and a small breadboard to the MSP430 MCU. No physical wall or computer connection is required due to the integration of a rechargeable 37 Watt-hour internal power bank. All hardware has been designed to fit compactly and seamlessly into a custom-designed container.  

Below is a simplified block diagram describing hardware functionality and interconnectivity:
![Block Diagram Image](https://github.com/ZeBlur/ELEC-327-RAJ-E-Light-Flippr1-/assets/168153437/79efa356-c2e2-40a2-9cf2-502504b29fa2)
Figure 1: Simplified block diagram of _E-Light Flippr1™_

### Software Architecture 
At a high level, the device's logic accomplishes the following:
- If button pressed on remote, toggle light state
- If button pressed on device, toggle light state
- If no motion is detected for approximately 5 minutes, if the light is on, turn the lights off

Code architecture consists of four auxilliary files and one primary control file.

Main Control and Logic File:
- _FLIPPER.c_

Auxilliary Helper/Header Files:
- _initialize.c_
- _initialize.h_
- _servo.c_
- _servo.h_

The _FLIPPER.c_ main file implements the _E-Light Flippr1™'s_ custom control logic with support from helper functions defined in the auxilliary files. Fundamental logic revolves around two states—'UP' and 'DOWN'— and the numerous timers and flags used to determine when to switch between them. Logic also includes polling the IR and Motion sensors, supported by additional functionality to account for the possibility of noise or erroneous inputs. 

The _initialize.c_ file include functions to initialize the MSP430's I/O ports and internal clocks. The _servo.c_ file includes a function to control the angle at which the servo arm moves to hit the light switch. 

Our exact code is described in more detail via comments in the included code files.

## Enclosure Design & Iterations

RAJ Group LLC utilized Autodesk Fusion 360 for the design of the _E-Light Flippr1™_ enclosure. The design was made to be functional, elegant, and compact, with space to hold all of the components listed in the Components section above—including a 37 Watt-hour battery, which ensures the device can operate on a wall without receiving power for multiple days at a time. In addition, the device supports constant connection via USB-C.

The design was carefully crafted such that the device can be placed exactly over the center of the light switch to toggle it. In addition, the device features curved and filleted edges to allow it to more seamlessly and elegantly add to the aesthetics of the room it is in.

Iteration 1 of our enclosure design in CAD is shown below:
<img width="866" alt="Screenshot 2024-04-26 at 15 48 31" src="https://github.com/ZeBlur/ELEC-327-RAJ-E-Light-Flippr1-/assets/72717429/78f3e3e2-264f-43dd-934d-b34d4dc7625b">
Figure 2: Iteration 1 in CAD

However, after preliminary testing with Iteration 1, we realized we needed to tweak some aspects of the design to allow the device to fit against the wall more seamlessly, allow for more space for a new battery solution (our initial battery solution failed to provide power), and to adjust the button hole tolerance to allow the button to better fit through the enclosure. Iteration 2 therefore addresses these concerns, and also brings visual enhancements, namely a Rice Wind Energy logo to allow for passive advertising of the team (which RAJ Group LLC is closely affiliated with) as well as more filleted edges.

Iteration 2 (final iteration) of our enclosure design in CAD is shown below:
<img width="883" alt="Screenshot 2024-04-26 at 20 04 36" src="https://github.com/ZeBlur/ELEC-327-RAJ-E-Light-Flippr1-/assets/72717429/a6f1e9e4-3c40-44f8-af93-9fb4ad4e004a">
Figure 3: Iteration 2 in CAD

A dimensioned drawing of Iteration 2 of our CAD design is shown below:
![2_E-Light Switch Flippr1 R2 No Battery Hole Drawing v1](https://github.com/ZeBlur/ELEC-327-RAJ-E-Light-Flippr1-/assets/72717429/5102ae1d-9f32-4909-b16a-aabd064a68be)
Figure 4: Iteration 2 Dimensioned CAD Drawing



## Installation
The _E-Light Flippr1™_ is easily installed or detached from a wide range of walls (using velcro, which is strong but not permanent). This means you can automate your light switch worry-free and without worrying about needing to access your house or building’s electrical wiring!

## Support
Contact our project development team @rajsilicongroup@gmail.com.

## Roadmap
We are currently exploring several additional features to the RAJ-E-Light-Flippr1: <br />

- Wi-Fi integration
- Ambient light sensing and adaptability
- Digital user controls and customization
- Reducing product size and weight
- Integrate into the McMurtry Innovation Space long-term


## Contributing

We are not currently open to contributions. 

If you would like to request a new feature, please email our project development team @rajsilicongroup@gmail.com.

## Authors and acknowledgment  
Team members and contributions:  

Raj Anthony - Lead CAD and Project Integration

- Led CAD design and 3D printing for device
- Helped with writing and debugging of logic

Adam Swartz - Lead Software Design

- Led design and writing of device logic
- Helped with wiring & hardware debugging

Jason Yang - Lead Hardware Design

- Led hardware integration & project documentation
- Helped with writing and debugging of logic

##
We would also like to thank our sponsors:

- Rice Wind Energy, for their charitable donation of wiring components and spare parts

- Oshman Engineering Design Kitchen, for the use of working space and facilities 

- McMurtry Innovation Space, for the use of testing space

## License

[MIT](https://choosealicense.com/licenses/mit/)

## Project Status

Preliminary prototyping has been completed, with all current intended features implemented. 
Additional design improvements being explored are listed above in the roadmap section
