# RAJ-E-Light-Flippr1-
RAJ (Raj Adam Jason) Group LLC
The RAJ-E-Light-Flippr1 is a convenient, cost-effective, and modular solution to wasted energy. 

## Description
### Introduction:
Want to turn on or off the lights from the comfort of your bed?

Want the lights to turn on automatically as you enter the room?

Always forgetting to turn off the lights when you leave the room?

Don't want the hassle or upfront cost of re-wiring the electrical systems in your building of choice?

RAJ Group LLC's **NEW** _E-Light Flippr1™_ is **THE** smart lighting solution for you! 

### Features and Description
Note: Section adapted from Final Project Proposal

Using a threefold combo of a handheld infrared remote, infrared receiver, and micro-servo in conjunction with a small microcontroller, the _E-Light Flippr1™_ allows you to simply point the handheld remote in the direction of the light switch to toggle the light switch from afar (or up close)! Furthermore, our state of the art motion detection technology, the _E-Light Flippr1™_ can detect when nobody is around and automatically turn off the lights for you! And if you want to simply toggle the light switch at the wall, the _E-Light Flippr1™_ has a panel-mounted button that effortlessly toggles the switch!

Wondering how it works? When you point the handheld infrared remote at the _E-Light Flippr1™_, the infrared receiver will detect this and send the appropriate signal to our microcontroller. That signal will be processed and relayed through the microcontroller to the servo, which will then flip the switch to the desired state. Also, if no motion has been detected for approximately 3 minutes, the motion sensor will relay a signal to the device, automatically turning off the lights! 
### Components:
1x TI LaunchPad kit with MSP430 MCU
1x 5C-SR501 PIR Motion Sensor
1x TSOP34440 Infrared Receiver
1x SG90 Micro Servo
1x USB-A to Micro USB cable
1x 4-pin button
1x INIU BI-B41 Power Bank (37 Watt-hours)
1x 17x10 Breadboard
1x RAJ Silicon Group Custom Enclosure
### Visuals
Below is a simplified block diagram describing product functionality.  
![Block Diagram Image](https://github.com/ZeBlur/ELEC-327-RAJ-E-Light-Flippr1-/assets/168153437/79efa356-c2e2-40a2-9cf2-502504b29fa2)
Figure 1: Simplified block diagram of _E-Light Flippr1™_
### Code Overview
At a high level, our code accomplishes the following:
- If button pressed on remote, toggle light state
- If button pressed on device, toggle light state
- If no motion is detected for approximately 5 minutes, if the light is on, turn the lights off.

Our code is described in more detail via comments in the code files.
### Enclosure Design & Iterations

RAJ Group LLC utilized Autodesk Fusion 360 for the design of the _E-Light Flippr1™_ enclosure. The design was made to be functional, elegant, and compact, with space to hold all of the components listed in the Components section above—including a 37 Watt-hour battery, which ensures the device can operate on a wall without receiving power for multiple days at a time. In addition, the device supports constant connection via USB-C.

The design was carefully crafted such that the device can be placed exactly over the center of the light switch to toggle it. In addition, the device features curved and filleted edges to allow it to more seamlessly and elegantly add to the aesthetics of the room it is in.

Iteration 1 of our enclosure design in CAD is shown below:
<img width="866" alt="Screenshot 2024-04-26 at 15 48 31" src="https://github.com/ZeBlur/ELEC-327-RAJ-E-Light-Flippr1-/assets/72717429/78f3e3e2-264f-43dd-934d-b34d4dc7625b">

Figure 2: Iteration 1 in CAD

However, after preliminary testing with Iteration 1, we realized we needed to tweak some aspects of the design to allow the device to fit against the wall more seamlessly, allow for more space for a new battery solution (our initial battery solution failed to provide power), and to adjust the button hole tolerance to allow the button to better fit through the enclosure. Iteration 2 therefore addresses these concerns, and also brings visual enhancements, namely a Rice Wind Energy logo to allow for passive advertising of the team (which RAJ Group LLC is closely affiliated with) as well as more filleted edges.

Iteration 2 (final iteration) of our enclosure design in CAD is shown below:
<img width="869" alt="Screenshot 2024-04-26 at 15 49 01" src="https://github.com/ZeBlur/ELEC-327-RAJ-E-Light-Flippr1-/assets/72717429/1eec7633-f57e-4bc0-a5c1-674ecd9a8d8c">
Figure 3: Iteration 2 in CAD

A render of Iteration 2 of our CAD design is shown below:
![E-Light_Switch_Flippr1_R2_No_Battery_Hole_2024-Apr-26_08-40-51PM-000_CustomizedView20125995546_png](https://github.com/ZeBlur/ELEC-327-RAJ-E-Light-Flippr1-/assets/72717429/0aa75439-21b2-49ad-90de-ff1f665aab40)
Figure 4: Render of Iteration 2


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
