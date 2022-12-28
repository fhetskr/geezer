#Geezer version 0.1

##Disclaimer
This project is entirely an amateur endeavour, and under no circumstances should any responsible individual use this tool in a production setting.

##What is this?
Geezer is a generalist interpreted language largely inspired by Python, C, and to a lesser extent Lisp. 

#What can Geezer do?
Right now, not much. The bones of the interpreter are there (i.e. the parser and memory management tools are largely complete), but semantic analysis is only just getting started. As of now only addition is supported.

##How do I use Geezer?
Geezer depends on [Flex](https://github.com/westes/flex) and [Bison](https://github.com/akimd/bison). Once you have those utilities properly installed, you can compile geezer from the root of this repository using `make`, and run scripts using `PATH/TO/GEEZER/geezer < my_script.gzr`

##Who can I blame for the existence of this abomination?
Direct your hatemail to the maintainer of the repository, one Peter Schmidt. You may contact via email at peter.dominic.schmidt@gmail.com