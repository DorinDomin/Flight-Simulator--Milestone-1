 <p align="center">
 <img src="https://user-images.githubusercontent.com/58748407/138861103-662cd857-a202-440f-aac0-4947f771d077.png" width="120" height="120">
</p>

# Flight-Simulator--Milestone-1

#### Creators: 
##### Dorin Domin
##### Netanel Tamir

#####  [our GitHub](https://github.com/DorinDomin/-Flight-Simulator--Milestone-1)

### **Basic information**
* The purpose of this extercise- to create code that allows remote control of an aircraft.
* Links we used for this project:
  - [FlightGear simulator](https://www.flightgear.org/)
  - The two lines we inserted the flight gear: "--telnet=socket,in,10,127.0.0.1,5402,tcp" "--generic=socket,out,10,127.0.0.1,5400,tcp,generic_small"
* Files we used for this project:
  - Fly.txt
  - Generic_small.xml

### **How Does It Work?**
Our program receives a file and breaks it into parts using the lexer function. We checked which of the strings were commands, and created a map that holds commands as the key, and a value that is a command variable from Of the right type.

* For example: we inserted the command "open data server" into the map, with a variable "OpenServerCommand" as the value.

After inserting the map with all the right commands, we started to execute each commnad by turn using the parser function. For the server and the client, we opened two different threads that runs simultaneously, and two different sockets. Next, the parser executed all of the variables. For each one, we check the direction of the arrow, and accordingly changes the values in the maps. We also payed attention to the loops in the file, and made a command variable that is responsible to take care of such cases.

Through the whole running of the programm, we received values from the simulator (using the server socket), and sent value to the simulator (using the client socket). That way, the connection stays open through the whole run.

After wer'e done with the file, the programm prints "done" and we can close the sockets, and the threads.



