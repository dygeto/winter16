/*******************************
 * Dylan Tomlinson
 * Adventure
 * CS 344
 * Benjamin Brewster
 * *****************************/
//notes: sometimes userPath does not print out
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


void createConnections(char *[], char [], char []);
void pickRooms(char *[], char *[]);
void gameLogic(char *[], char [], char []);

int main(void)
{
   char buffer[512];
   int fileStuff;
   ssize_t nread;
   ssize_t nwritten;
   char roomDirectory[] = "./tomlinsd.rooms.";
   char processIDString[10];
   struct stat st = {0};
   char pidString[10];
   srand(time(NULL));
	
   char *roomNames[10] = { "Kitchen", "Diningroom", "Basement", "Laundryroom", "SECRETROOM", "Livingroom", "Bedroom", "Observatory", "Bathroom", "Trophyroom" };
   char *rooms[7];
   int connections[7][7] = {{ 0 },{ 0 }};

   //adding the process ID to the directory name
   pid_t processID = getpid();
   sprintf(pidString, "%d", processID);
   strcat(roomDirectory, pidString);
   if (stat(roomDirectory, &st) == -1) {	
      mkdir(roomDirectory, 0777);
   }
  
   //who you gonna call? (these functions)
   pickRooms(roomNames, rooms);
   createConnections(rooms, roomDirectory, buffer);
   gameLogic(rooms, roomDirectory, buffer);  
   
   exit(0);
}


//big function, reads from the files and contains entire game logic
void gameLogic(char *rooms[], char roomDirectory[], char buffer[]) {
   
   //so many variables!
   char* bufferToken;
   int readFile;
   char* file = malloc(50);
   int i = 0;
   int j = 0;
   ssize_t numRead[7];
   int lastConnection = 0;
   int win = 0;
   char* connections = malloc(50);
   char userInput[512];
   int userSteps = 0;
   char* userPath = malloc(5000);
   char* currentRoom = malloc( 1 + strlen(rooms[0]));
   
   //only needed to pass rooms[0] into the function to get the loop going, but oh well
   strcpy(currentRoom, rooms[0]);

   //where the magic happens, this loop contains the game logic
   while (win == 0) {
      printf("\nCURRENT LOCATION: %s\n", currentRoom);
      printf("POSSIBLE CONNECTIONS: ");
      
      int numConnections = 0;;
      
      //reading the file into the buffer
      strcpy(file, roomDirectory);
      strcat(file, currentRoom);
      readFile = open(file, O_RDONLY);
      numRead[i] = read(readFile, buffer, 512);
      
      //token to connections
      bufferToken = strtok(buffer, " ");
      bufferToken = strtok(NULL, " ");
      bufferToken = strtok(NULL, "\n");
      
      
      while (lastConnection == 0) {
         //should be officially there, note there is no missing file check
         bufferToken = strtok(NULL, " ");

         //reads connections into the buffer, then cats them onto connections
         if (strcmp(bufferToken, "CONNECTION") == 0) {
            bufferToken = strtok(NULL, " ");
            bufferToken = strtok(NULL, "\n");
            strcat(connections, bufferToken);
            strcat(connections, ", ");
            printf("%s", bufferToken);
            printf(", ");
            numConnections++;
         }
         else {
            lastConnection = 1;
         }
      }

      //using tokens to get to the room type
      bufferToken = strtok(NULL, " ");
      bufferToken = strtok(NULL, "\n");

      //if it's the end room a winner is you!
      if (strcmp(bufferToken, "END_ROOM") == 0) {
         win = 1;
         continue;
      }

      //gets user input
      printf(".\nWHERE TO? >");
      fgets(userInput, 512, stdin);
      strtok(userInput, "\n");

      //some cool variables necessary for the next loop
      char *tempPath;
      char *connectionCompare;
      char *allConnections = malloc(1 + strlen(connections));

      strcpy(allConnections, connections); //temp string for connections, used for reset
      connectionCompare = strtok(connections, ", ");

      //Complicated loop that determines if the given room exists
      for (j = 1; j <= numConnections; j++) {
         
         //if the room is found, increments steps and cats the room onto
         //userPath
         if (strcmp(userInput, connectionCompare) == 0) {
            userSteps++;
            tempPath = strtok(userInput, "");
            strcat(userPath, tempPath);
            strcat(userPath, "\n");
            break;
         } 
         //If all connections are compared and the room isn't found
         //then it reprompts the user
         else if (j == numConnections) {
            printf("\nHUH? I DIDN'T UNDERSTAND THAT ROOM. TRY AGAIN.");
            j = 1;
            printf("\nCURRENT LOCATION: %s\n", currentRoom);
            printf("POSSIBLE CONNECTIONS: ");
            printf("%s", allConnections);
            printf("\nWHERE TO? >");
            fgets(userInput, 512, stdin);
            strtok(userInput, "\n");
            strcpy(connectionCompare, "");
            strcpy(connections, allConnections);
            connectionCompare = strtok(connections, ", ");
         } 
         //simply moves along string to the next room to be compared
         else 
         {
            connectionCompare = strtok(NULL, ", ");
         }
      }
      //resetting variables for next go at the while loop 
      strcpy(currentRoom, userInput);
      memset(connections, 0, 50);
      lastConnection = 0;
      i++;
   }
   //you finally won, nice job
   printf("\n\n\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
   printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", userSteps);
   printf("%s", userPath);
}

//Simple function that decides which 7 rooms to use (out of 10)
void pickRooms(char *roomNames[], char *rooms[]) {
   int i,j;
   for (i = 0; i < 10; i++) {
      j = rand() % 10;
      char *temp = roomNames[j];
      roomNames[j] = roomNames[i];
      roomNames[i] = temp;
   }

   for (i = 0; i < 7; i++) {
      rooms[i] = roomNames[i];
   }
}

/****************
 * Function: creatConnections: creates the files for the room's and their info.
 * Which includes: name, connected rooms, room type.
 * 
 * 
 * **************/
void createConnections(char *rooms[], char roomDirectory[], char buffer[]) {

   int inputConnections;
   int a,b,c;
   char intHolder[3];
   int numConnections,currentConnections;
   ssize_t numWrite;
   ssize_t numWritten[7];
   int connections[7][7] = {{0},{0}};
   
   //2x2 matrix to represent room connections 1 = connected, 0 = not connected
   for (a = 0; a < 7; a++) {
      currentConnections = 0;
      numConnections = (rand() % (6-3))+3; //cool formula
      while (currentConnections < numConnections) {
         b = rand() % 7;
         connections[a][b] = 1;
         connections[b][a] = 1;
         currentConnections++;
      }
   }
   
   //you should be with the other variables
   char file[50];
   
   //more loop magic, creates the room files
   for (a = 0; a < 7; a++) {

      //opens the file, puts in the room name
      int connectionCounter = 0;
      strcpy(file, roomDirectory);
      strcat(file, rooms[a]);
      inputConnections = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
      sprintf(buffer, "ROOM NAME: %s\n", rooms[a]);
      
      //writes the connections in
      for (b = 0; b < 7; b++) {
	 if (connections[a][b] == 1 && a!= b) {
            strcat(buffer, "CONNECTION ");
            sprintf(intHolder, "%d", (connectionCounter + 1));
            strcat(buffer, intHolder);
            strcat(buffer, ": ");
            strcat(buffer, rooms[b]);
            strcat(buffer, "\n");
            connectionCounter++;
         }
      }

      //assigns room types!
      strcat(buffer, "ROOM TYPE: ");
      if (a > 0 && a < 6) {
         strcat(buffer, "MID_ROOM");
      }
      else if ( a == 0) {
         strcat(buffer, "START_ROOM");
      }
      else {
         strcat(buffer, "END_ROOM");
      }
      strcat(buffer, "\n");

      numWrite = strlen(buffer) + 1;
      numWritten[a] = write(inputConnections, buffer, numWrite);
      close(inputConnections);
   }

}

