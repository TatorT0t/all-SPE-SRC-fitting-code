# all-SPE-SRC-fitting-code
The code is able to process the fitting for all SPE and SRC runs in the "Run Log" format. 
The runs must be in the same directory as this code. 
This is not meant for external use due to messiness and lack of clarity.

Overview of files found in repository:

CMS*.root - Root files that you want to fit

testing.txt - 
	Copy in pasted information on all runs that want to run (from google sheets). 
	SPE and SRC runs are taken back to back. This formatt is nessasary for the fit to run the file.
	I would look inside this files to gain an understanding on the input format for fit_runs_V2.cpp.

testing_ouput.txt - the output for testing.txt files after running fit_runs_V2()



Steps for execution fit for all runs:
  
  (1) Load in fit_runs_V2.cpp into ROOT
  
  (2) run fit_runs_V2() function in root
  
