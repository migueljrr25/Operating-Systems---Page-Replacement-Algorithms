File Names
___________________________________________________________________________________________________________
| 1) main.c-------------------------> main file prints the output, it opens the trace file, choose the trace and the algorithm.
| 2)options.c-----------------------> code that choose which replacement algorithm to use
  3)options.h-----------------------> .h of the options 
  4)page.c--------------------------> page code handle the pages 
  5)page.h--------------------------> .h of the page.c
  6)replacement_algo.h--------------> include the replacement algorithm code

	Please do not take point of fo having more that one file. It was to hard to keep everything organized in a single C program. 
                       
|_________________________________________ _________________________________________________________________|

Intructions to Compile and Run

___________________________________________________________________________________________________________
| A makefile is included                                                                                   | 
| type "make"  in order to create the executables. This program is compiled with 
	gcc -std=c99  main.c  options.c  page.c in case you encounter any issue with 		makefile                                                                                   |
| to run the first part "./memsim <tracefile> <nframes> <rdm|lru|fifo|vms> <debug|quiet>"                                                                   |
| to remove the created executables type "make clean"                                                      |                                                  
|Thabsk for your time
__________________________________________________________________________________________________________|