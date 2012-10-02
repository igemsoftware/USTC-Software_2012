USTC-Software_2012
==================

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

For more information, please refer to our [wiki page](http://2012.igem.org/Team:USTC-Software/software.html)