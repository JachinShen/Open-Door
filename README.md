# Open-Door

上海交通大学《工程学导论》遥控开门作品源代码
这个项目是为了解决在寝室里开门的问题，经常有室友回来没拿钥匙需要给他开门，但是自己又腾不出手，这时候能远程开门是很方便的。
这个项目包含两部分，一部分是安卓App的源代码，另一部分是 TI MPS430 单片机的源代码。
两者之间通过 HC-05 蓝牙模块进行通讯，手机与蓝牙模块通讯，单片机通过串口与蓝牙模块通讯，从而实现了远程遥控。


SJTU engineering course project code

The project is aimed to open the door for the roommate in dormitory remotely when you are in busy, such as playing the computer games, doing the homework and so on.

This includes codes of Android App and MSP430.

Use bluetooth to communicate between Android and MSP430. MSP430 accepts the signal and run the motor and servo to open the door.
