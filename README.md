 <p align="center">
 <img src="https://user-images.githubusercontent.com/58748407/138865062-296148ad-4b85-41eb-9b43-a4dc9565690d.png" width="120" height="120">
</p>

# Flight Simulator- Interpreter
In this exercise we are wrote an interpreter between a text file ,with lines in a coding language given to us in the assignment instructions, and commands to the plane in the flight simulator. Our project is able to read the file, line by line, interpret them accordingly, and send the desired commands to the plane in the simulator (FlightGear). Furthermore, the simulator will also send data in the form of values of specific fields defined in generic_small.xml. 

## Installation

First you need to install FlightGear and define in the advanced settings 

```
--generic=socket,out,10,127.0.0.1,5400,tcp,generic_small   
--telnet=socket,in,10,127.0.0.1,5402,tcp
```
Then you need to add the generic_small.xml file to the protocol folder inside of the flightgear path on your computer.

Finally you need to add the fly.txt file to the location you plan on running the project from.
## Usage

In order to use the project, you need to do the following steps:

First, run the following commands in the terminal (first cd to the right path)
```
g++ -std=c++14 *.cpp -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -o a.out -pthread

./a.out fly.txt
```

After that you can start FlightGear. The plane will now fly according to the instructions in the text file (fly.txt).

## How it works:

### The Lexer

The lexer can be found as a function inside main.cpp

It's purpose is to read the given text fiile line by line and tokenize it's contents

### The Parser

The parser can be found in the file by the same name.

It's purpose is to take the output of the lexer and form commands out of the said output. It works by looking for key words that indicate the start or end of a line. (We used '$' to indicate the end of a line in the text file)

### The Commands

We created a inheritance tree that starts with Command.h. Out of it we created classes for Opening the server, Connecting the client, Var commands, Print,Sleep, If ,and While Commands. All of which are key words in the coding language assigned to us and indicate a logic which we implement in the relevent classes.

## Version Control

We used Git for this project

https://github.com/DorinDomin/Flight-Simulator--Milestone-1

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.


## Authors
Dorin Domin, Netanel Tamir
