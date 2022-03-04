NOTES:

- Single player functions properly for a single client connection. Multiple clients are allowed to connect and 
  the server will accept multiple connections but the game logic breaks for single player when this happens.
- Forking has not been implemented for the dictionary reading, scoreboard sharing, and input checking.
- Multiplayer has not been implemented.

POSIX QUEUE:

- POSIX queue works and functions properly and has been implemented into single player. This can be seen for the
  dictionary checking in the gameLogic() method.
- The intended use for POSIX queue is to share between processes, but since we have not implemented multiplayer
  we are working under a single process.
- If this was complete, the POSIX queue would fit its intended purpose but its functionality is sound
  in our code.

INSTRUCTIONS FOR COMPILE AND RUN:

Server start: gcc server.c -o server -lrt -pthread
Client start: gcc client.c -o client -lrt -pthread
