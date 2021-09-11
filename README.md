# Missing-Migrants
A C program that reads  a .csv text with info about missing migrants, till 2020, and prints date, location, coordinates, the number of men, women and children, the cause of missing and the URL from which extracted the information.

 The code is compatible with Windows, Linux and MAC. In order to do so, a copy of the original getline() had to be made so the code would run on another OS except Linux which does not support getline(). Windows for example. This issue has been resolved by defining "getline(x,y,z)"  to "read_line(x,y,z)" if the OS is one of the them.
 
 ## Code Mechanics
1. The csv file is parsed and every white space character except of ';' is deleted, so the .csv is properly formated.
 
2. When the code starts, the user can either type the command or type 'help' for a help menu.
    
3. The code is **not** key-sensitive


### Commands
1. help: Prints a menu with commands and explains the use of each command

2. exit: Terminates the running code    

3. newincident: Creates a new incident from command line and adds it to incident list  

4. load <file path>: loads incident from specified csv file 

5. count |men|women|children: Counts missing|men|women|children

6. findl: Finds the location of the incident 

7. findc: Finds the cause of death of the incident 

8. findr: Finds Region of the incident printa (valid) date: 

9. printa: Prints all incidents after the exact date given

10. printb (valid) date: Prints all incidents untill the exact date given

11. deletn: Deletes the **newest** added incident

12. deleteo: Deletes the **oldest** added incident

13. save: Saves incidents to a formatted text file
