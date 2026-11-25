It will simulate a simple railway monitoring system that allows a user to:

1. Register trains.
- save the added trains(done)
- update saved trains details(done)
- view saved trains(done)
- delete a train detail(done)
- search (done)

2. Monitor track occupancy.
- save the added track(done)
- update saved track details(done)
- view saved track(done)
- delete a track detail(done)
- search (done)

3. Display signal status (RED or GREEN).(done)
4. Detect occupied tracks.(done)
5. Prevent two trains from using the same track.(done)
6. Prevent two tracks from being used by the same train.(done)
7. Display a railway system overview.(done)

#Security#
8. Secure Database by giving controled access

====================================
       Requirements To Run
====================================
1. If you want to run directly, just double click on main.exe.
2. If you want to compile and run from VS code you have to use the MSVC compiler and run it in x64 bits (https://code.visualstudio.com/docs/cpp/config-msvc#_prerequisites), then you have to clone vcpkg into the working directory (the directory where you cloned the project Railway-Monitoring-System). i.e
Enter your terminal and run the command:
 
git clone https://github.com/microsoft/vcpkg.git

Then enter in .../Railway Monitoring System\vcpkg\ and run the following commands:

.\bootstrap-vcpkg.bat

.\vcpkg integrate install

.\vcpkg install yaml-cpp:x64-windows

After that you can run.



Here is an example of the console interface:
====================================
       RAILWAY MONITORING SYSTEM
====================================
Welcome to the Railway Monitoring System!
System: Railway Network

Service List:
====================================
***Please enter a letter from the list and enter "exit" to end.***

Select the service you want to use by entering the coresponding letter:

1. Train Settings (Enter letter T) -> ***Please enter a letter from the list and enter "exit" to end.***

                                      Select the service you want to use by entering the coresponding letter:
                                   
                                      1. Add New Train (Enter letter C)
                                   
                                      2. View Train List (Enter letter R)
                                   
                                      3. Update Train Details (Enter letter U)
                                   
                                      4. Delete Train (Enter letter D)

                                      5. Search for a particular train (Enter letter S)

                                      6. Exit (Enter exit)

2. Track Settings (Enter letter Tr) -> ***Please enter a letter from the list and enter "exit" to end.***

                                      Select the service you want to use by entering the coresponding letter:

                                      1. Add New Track (Enter letter C)

                                      2. View Track List (Enter letter R)

                                      3. Update Track Details (Enter letter U)

                                      4. Delete Track (Enter letter D)

                                      5. Search for a particular track (Enter letter S)

                                      6. Exit (Enter exit)
                                   
3. Display signal status (RED or GREEN) (Enter letter S) -> ***Please enter a letter from the list and enter "exit" to end.***

                                                            Select the service you want to use by entering the coresponding letter:

                                                            1. View all Track signal (Enter letter A)

                                                            2. View Red signal Track List (Free Tracks) (Enter letter R)

                                                            3. View Green signal Track List (Occupied Tracks) (Enter letter G)

                                                            4. Search signal of a particular Track (Enter letter S)

                                                            5. Exit (Enter exit)



4. Occupy a track and Release a track
5. Exit


 