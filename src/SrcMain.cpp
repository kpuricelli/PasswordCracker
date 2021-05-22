#include "SrcMain.h"
#include "PasswordCracker.h"

#include <iostream>
void ProcessCommandArgs(int argc, const char* argv[])
{
	// argv[1] is dictionary filename, argv[2] is password filename
	if (argc == 3)
	{
		PasswordCracker passwordCracker(argv[1], argv[2]);
	}
}
