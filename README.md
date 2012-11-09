USTC-Software_2012
==================
#Best Software Tools Project for iGEM 2012#

## REBORN: Reverse Engineering for BiOlogical Regulatory Networks ##

This software contains four small parts, dealing with seperate functions in reverse engineering process.
#####1.Map.#####
#####2. Console.#####
#####3. SandBox.#####
#####4. Report.#####

###Map###

**Map** has the function of showing input behaviors including some details about input data such as the minimum value and the maximum value. **Map** also privdes a quick and direct view of the behavior of the input data. With a clean and neat _Graphic User Interface_, it is very simple for users to check their input. In addtion, **Map** has the connection with **Console**.

###Console###
**Console** undertakes the functions of generating _ODEs_, generating _GRNs_ and rebuilding the system. In **Console**, users can either use the default parameters or input their own parameter. All the calculations begin after _"Let's REBORN"_ is clicked. More over, the whole calculating process will be shown in the _Graphic User Interface_. All the results can be sent to _FTP_. Therefore, users can view the calculating outputs either on _GUI_ or in _FTP_. Apart from the _FTP_ service, **Console** has the interface with other parts of our software, including **Map**, **SandBox** and **Report**.

###SandBox###
**SandBox** is used to present the GRN models of the system. Its 3-D design makes it fullly interactive for users. Users can drag or scroll to change their point of view and the camera angle.Flags on **SandBox** represent genes and the lines are the regulatory relations. **SandBox** also works with **Report** to make the software a seamless one.

###Report###
**Report** has the function of presenting output results. It has a two-column structure showing different aspects of outputs. The left column presents graph of output behavior, and the right column gives details about the models.

This software can be built on Windows, Linux and MacOS operating platform.

For more information, please refer to our [wiki page](http://2012.igem.org/Team:USTC-Software/software.html).

###Instructions###
####Source Files####
**source** folder contains all the source files including command line source files in **src** folder and GUI source files in **Reborn_GUI** folder.

The command line source files are written in C++ language, and can be complied across platforms, external libraries includes SBML library can be downloaded [here](http://sbml.org/Software/libSBML).

The GUI source files are written in C++ language with Qt SDK, it can also be compiled across platforms using Qt SDK, which can be found [here](http://qt-project.org/downloads).

####Executables####
The software executables are in the **executables** folder, in this folder contains separated folder for each platform, and for each platform we provide both command line tool and GUI versions.


####Future Work####
1. Expanding input types. At present, in our software, only time courses and prior knowledge can be the input. In the next version of **REBORN**, logical behaviors such and _AND Gate_ can also be input to our software.

2. More mutation types. In this version, our software contains five kinds of mutations: changing degradation rate, changing kinetic constants, adding a new gene, adding a regulation and adding post-transcriptional modifications. In the future, more mutations will be added, such as adding logical gates, adding reactions with metabolites and changing states of proteins and promoters.

3. Optimizing selection method. By now, our software represents all the outputs without selection. The future version will contain the function of selecting those outputs with more stable and credible properties. More over, users can use default scoring functions or they can define new scoring functions as they want.

4. Expanding database. The present version of **REBORN** only contains the database of E.coli K-12. In the future, more genome information will be added to the database. Therefore, the output will be much more varied and comprehensive.

5. Setting up a platform. Even though the first version of **REBORN** provides the source code on _github_, the programmers still have the difficulty modifying these codes. In the future, we hope there could be a more generalized platform for both programmers and users. In the platform, users can download applications as they want and programmers can design applications for different groups of users.

###Contacts###

For any questions, feel free to contact:

Command line tool: Kai Kang(myfavouritekk@gmail.com)

GUI tool: Sen Jiang(jayxon@gmail.com)
