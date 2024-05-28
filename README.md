## Project name: Plant Tracking By Pic877
A plant tracking system can help monitor the health and growth of plants by tracking various parameters such as temperature, humidity, soil moisture
## About this project
This project use PIC116F877A as a microcontroller
Simulation on proteus
![image](https://github.com/phamhduc/Embedded_Project-Plant-tracking-system/assets/101264143/46b90e6f-f168-4d63-b641-4cde7d7dc65a)
## Working Principle of the Project
There are 4 buttons and 1 switch to control the system
**Button 1: Date time configuration**
![image](https://github.com/phamhduc/Embedded_Project-Plant-tracking-system/assets/101264143/75bd9844-d4bb-42dc-a33b-d83c6d142b9d)

* Press this button to config Date and time
* After pressing, you can use 4 button to control Date Time configuration function
* 
 ![image](https://github.com/phamhduc/Embedded_Project-Plant-tracking-system/assets/101264143/b23791e9-06e7-44f2-bfaf-e23d1598e9f6)
  
* Button 1 to increase the value of the cursor.
* Button 2 to decrease the value of the cursor.
* button 3 to move the cursor to the next value.
 ![image](https://github.com/phamhduc/Embedded_Project-Plant-tracking-system/assets/101264143/06362301-4ee1-4cd1-a5a8-d2e8ed29d103)

* Button 4 to exit and save current time value, after saving, it will automatically display your configuration.
  
**Button 2: Tracking Moisture and temperature**
![image](https://github.com/phamhduc/Embedded_Project-Plant-tracking-system/assets/101264143/88e77953-ff21-437d-bc46-1d38477ed639)

Simulation in proteus provides us a tool to work with temperature sensor LM35 with ADC signal.
In this project I demonstrate an moisture sensor by using ground resistor. Applying voltage divider principle to convert the  signal to ADC value.
* in this mode you can use button 4 to exit to main program.

**Button 3: Setting Alarm**
* In this system, i provide 2 auto-watering alarm.
  
* press button 1 for choosing alarm 1 and 2 for alarm 2.
![image](https://github.com/phamhduc/Embedded_Project-Plant-tracking-system/assets/101264143/79e360b3-dab5-42aa-98c1-69d1d1ef8d08)
* After choosing  alarm, you can use directional button the same as setting date and time to adjust your alarm in hh:mm
* Press button 4 for saving the alarm
![image](https://github.com/phamhduc/Embedded_Project-Plant-tracking-system/assets/101264143/d9f09498-922f-434c-9335-13d94ff61137)
* press button 4 again to exit to main program.
**Switch: Alarm enable**
* When the switch close: alarm enable.
* When the switch open: alarm disable.
*  alarm enable: automatically activate the waterbump when the current time matches the time set in the alarm function (in this simulation, i used LED to demonstate the activating signal of Waterbump)
*  For example, I set  the alarm at 4:20.
![image](https://github.com/phamhduc/Embedded_Project-Plant-tracking-system/assets/101264143/14093f30-e1cf-4697-9c8d-ec821ff4dd70)
* activate the bump for 1 minute, the activating time can be config.
**Interupt indication**
![image](https://github.com/phamhduc/Embedded_Project-Plant-tracking-system/assets/101264143/1c1a0ab4-fb24-4e6c-9590-aa91301f6d8e)
* When the moisture and the temperature exceed the setting range, it will activate LED D2(This LED can be the buzzor or something that can announce the signal to the operator)


  


