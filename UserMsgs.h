#define WM_TRAYNOTIFICATION	(WM_USER +  1)
#define WM_STARTUP			(WM_USER +  2)
#define WM_ACCOUNTSTATUS	(WM_USER +  3)

#define WM_SCANCOMPLETE		(WM_USER + 10)
#define WM_SCANSTART		(WM_USER + 11)
#define WM_FILESTATUS		(WM_USER + 12)
#define WM_FILECOMPLETED	(WM_USER + 13)
#define WM_VERSION			(WM_USER + 14)
#define WM_PLAYSONG			(WM_USER + 15)
#define WM_PLAYLIST			(WM_USER + 16)
#define WM_MASTERSTATE		(WM_USER + 17)
#define WM_SERVERSTATE		(WM_USER + 18)
#define WM_REJECTED			(WM_USER + 20)

// possible states for the account creation status:
// (Don't change these, by the way -- these are the values the server is going to send)
//
// It is possible for there to be multiple reasons for failure.  So, if a person
// was really messed up, the value sent back from the server might be
// ACS_DUPE_EMAIL | ACS_DUPE_USERNAME | ACS_OLD_VERSION
//
// The ACS_ERROR flag is set if there was a problem with the database setting up the 
// account.  This doesn't mean the data is bad, it could just mean we were rebooting
// the machines.  
//
// It is an error for ACS_CREATED and anything else to be set at the  same time.
#define ACS_UNKNOWN 		0
#define ACS_CREATED 		1
#define ACS_DUPE_EMAIL 		2
#define ACS_DUPE_USERNAME 	4
#define ACS_OLD_VERSION		8
#define ACS_ERROR   		16


// Definitions when rejected by server is sent
#define INVALID_ACCOUNT 1
#define OLD_VERSION 2
#define DUPE 3
