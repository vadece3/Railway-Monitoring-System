It will simulate a simple railway monitoring system that allows a user to:

1. Register trains.
- save the added trains(done)
- update saved trains details(done)
- view saved trains(done)
- delete a train detail(done)
- search (in progress)

2. Monitor track occupancy.
3. Display signal status (RED or GREEN).
4. Detect occupied tracks.
5. Prevent two trains from using the same track.
6. Display a railway system overview.


Here is an example of the expected console interface:
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


2. Occupy a track
3. Release a track
4. Display signal status
5. Display system overview

Choose an option: 1

Enter train ID: ICE101
Enter train name: ICE Express

Train successfully registered!

------------------------------------
TRACK STATUS
------------------------------------
Track A: FREE
Track B: OCCUPIED
Track C: FREE

SIGNAL STATUS
------------------------------------
Signal A: GREEN
Signal B: RED
Signal C: GREEN