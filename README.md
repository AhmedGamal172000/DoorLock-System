# DoorLock-System

By using atmega16 i have implemented a doorlock system that have *TWO* microcontrollers, 
the first one is for human interface and didn't take any dicision to open the door or not,
it's only for taking input from keypad and according to Microcontroller2 ,
it displays the needed screen. The other Microcontroller (2) is saving the password in external EEPROM by using I2C (twi) and by using motor and buzzer we check if the password correct after choose to open door. the buzzer will not work and the door will open in 15 sec and close in 15 sec by using timer0 in Normal Mode.
if the password is wrong, Microcontroller(2) communicate with Microcontroller(1) by UART communication protocol to make the user re-enter the password after knowing that it is a wrong password. The password is 5 digits.


The project is implemented by using this components:
KeyPad
LCD 
EEPROM
UART communication protocol
I2C (TWI)
BUZZER
MOTOR (by using Timer0 for openning and closing the door time in Normal Mode)

The system is Secured from Missing Real Time as it implemented in a professional way in a clean writing code.

How to use the Program:
We have our 1st screen to enter 5 digits of password.
Then after pressing enter '=' we will need to re-enter password. if wrong we will go to the 1st screen.
If accepted we will go to main options as we have 2 options, + for OpeningDoor, - for Change password.
for any choice if we have a wrong password we will try to enter it again until we got the right password.
