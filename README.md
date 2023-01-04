# Clock-Timer
The application for practicing work with Qt-framework

## Build
- Selected OSs - Windows and Android
- Version of C++ - 17
- Version of Qt - 5.15.2
- Version of JDK - 17.0.4.1
- Version of Android SDK - 33
- Build Tools Version - 28.0.3
- Version of Gradle - 7.0.0

## A brief review of the application
This project represents a clock (analog and digit) with a timer. Like a user, you can see the current time; like a user, you can select a time on the circle of an analog clock or type numbers in the digit clock and start a timer; after that, you can restart and stop the timer; when timer stopped, there will be a record in the history table with current date and time, and interval of work timer. You can clear the table and unselect the selected time like a user. Also, the application can save data before closing and restore it after the start of the application. All functions are the same for the Android version, except that the clocks and the history table are in different tabs.

## General application view
1. Windows
<p align="center">
  <img src="https://user-images.githubusercontent.com/34779566/210522171-aa16f168-29c9-46e8-8cce-f54841bfca54.png" height="500" alt="Windows application view">
</p>

2. Android
<p align="center">
  <img src="https://user-images.githubusercontent.com/34779566/210523696-ab8f00ac-3429-4bb4-afc9-55852160f0ab.png" height="500" alt="Android first tab of application view">
  <img src="https://user-images.githubusercontent.com/34779566/210524084-360100c2-7a7f-44ad-bdac-14ca00439754.png" height="500" alt="Android second tab of application view">
</p>

## Saving
To save all intervals in the table and the properties of the window (only on Windows), I use QSettings, where I keep data nearby to a .exe file and in .ini format:
<p align="center">
  <img src="https://user-images.githubusercontent.com/34779566/210525743-803d6b23-1be3-4eff-9deb-133ce88a2147.png" height="300" alt="The ini (save) file">
</p>
