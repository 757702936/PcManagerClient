	HANDLE hToken; // handle to process token

	TOKEN_PRIVILEGES tkp; // pointer to token structure

	BOOL fResult; // system shutdown flag

	// Get the current process token handle so we can get shutdown
	// privilege.
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		ErrorHandler("OpenProcessToken failed.");

	// Get the LUID for shutdown privilege.
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
	&tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1; // one privilege to set
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get shutdown privilege for this process.
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
	(PTOKEN_PRIVILEGES) NULL, 0);

	// Cannot test the return value of AdjustTokenPrivileges.
	if (GetLastError() != ERROR_SUCCESS)
		ErrorHandler("AdjustTokenPrivileges enable failed.");

	// Display the shutdown dialog box and start the time-out countdown.
	fResult = InitiateSystemShutdown( NULL, // shut down local computer
	"Click on the main window and press \
	the Escape key to cancel shutdown.", // message to user
	20, // time-out period
	FALSE, // ask user to close apps
	TRUE); // reboot after shutdown

	if (!fResult)
	{
		ErrorHandler("InitiateSystemShutdown failed.");
	}
	
	// Disable shutdown privilege.
	tkp.Privileges[0].Attributes = 0;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
	(PTOKEN_PRIVILEGES) NULL, 0);

	if (GetLastError() != ERROR_SUCCESS
	{
		ErrorHandler("AdjustTokenPrivileges disable failed.");
	} 