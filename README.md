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

1. Register a train
2. Display all trains
3. Occupy a track
4. Release a track
5. Display signal status
6. Display system overview
0. Exit

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