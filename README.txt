======
README
======

This is an extension of PRISM, developed for the paper
"Prioritizing Methods to Accelerate Probabilistic Model Checking of Discrete Time Markov Models"

-------------
INSTALLATION
-------------

Installation is as for normal versions of PRISM. In short:

 - enter the PRISM directory and type "make"
 
For detailed installation instructions, check the online manual at:

  http://www.prismmodelchecker.org/manual/InstallingPRISM/Instructions
  
----
LOGS
----

The log files for the results presented in the paper can be found in 'logs-paper'

-------
EXAMPLE & RESULTS
-------

To run all the experiments please run:

cd prism
./run_all.sh

To run individual experiments for e.g. herman please run:

cd prism
./herman.sh

Results in the paper have been obtained on a machine with 128GB of memory.
To run all experients at least 8GB of RAM is needed.
Please note that the experiments may experience a considerable slowdown if run on a machine
with limited memory. The default in the run scripts is to use up to 2GB of RAM for Java.
To increase this amount (or to decrease it if you have less), see "Java memory" here:

http://www.prismmodelchecker.org/manual/ConfiguringPRISM/OtherOptions

The sack size for running topological methods should be more than its default value. 
Set PRISM_JAVASTACKSIZE to 500MB or more (in bin/prism).

To run improved GS methods for herman case studies, use -nocompat option.
 

