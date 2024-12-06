# Code Composer Studio
This folder contains the files for the microcontroller program.

BPSU is the main project.

The bootloader contains the files for the bootloader, api and downloader. Because the processor can use different clock sources, speeds it is useful to build your own bootloader.

In the tools folder there are 3 different tools:
 - With the SW_Downloader we can flash our project into the memory. The bootloader only has to be uploaded once to the microcontroller, then all the other flashes will be much faster.
 - The HiTerm program will be our monitoring program. The BPSU project contains the MUCI monitoring library, which helps us to monitor and override the values in the memory.
 - The Dwarf-Converter will convert the main projets .out file to a sorted txt file, which can be used by the HiTerm softver.
