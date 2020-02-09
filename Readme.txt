# ------------------------------------------------RTOS TIC TAC TOE GAME PROJECT------------------------------------------------------------------ 

This project uses uCOS-II RTOS language to create a Tic Tac Toe Game . 

Table of content

1. Confiuration
2. Operation
3. Manifest
4. Copyright and License
5. Contact Information

1. Configuration
	This project requires Visual Studio application in order to compile and run.  
2. Operation
	a) Please download the zip file from github.
	b) Unzip the newly downloaded file and rename the "Tic-Tac-Toe-master" folder to "SOFTWARE".	
	c) Move the "SOFTWARE" folder to C drive.
	d) Click Window button to search for "Developer Command Prompt" and open it.
	e) Change the working directory by using the command "cd C:\SOFTWARE\uCOS-II\Ports\80x86\WIN32\VC\exercises\RTOS_1".
	f) Then issue the command "nmake /f makefile.vc all" to compile the program.
	g) Run the program by using command "ucos_ii".
	h) Once exit is needed, simply press "ESCAPE" button on the keyboard.
3. Manifest (list of files and short description of their roles)
	a) Files inside SOFTWARE\uCOS-II\SOURCE folder
		These are files provided by the uCOS language.
	b) Files inside SOFTWARE\uCOS-II\Ports\80x86\WIN32\VC\src
		These are also provided by the uCOS language.
	c) Files inside SOFTWARE\uCOS-II\Ports\80x86\WIN32\VC\exercises\RTOS_1
		Most notable files:
		- makefile.vc: this is the makefile to instruct the Visual Studio to compile the program.
		- OS_CFG.H: determine which portion of uCOS is included in the compilation.
		- uCOS_II.c: the code for the program.
4. Copyright and License
	Visual Studio is a product of Microsoft Corporation.
	uCOS-II is a trademark of Micrium, Inc.
	Other trademarks and trade names are those of their respective owners.
	The person mentioned in the contact information content has the copyright and license for the code.		
5. Contact Information
	Tin Luu,
	Department of Information Technology
	Vaasa University of Applied Sciences
	Wolffintie 30, Vaasa, Finland
	Home: http://www.cc.puv.fi/~e1700674
	Email: e1700674(at)edu.vamk.fi