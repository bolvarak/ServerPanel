SQLite format 3   @     7                                                             7 -��  v �� v v  �#''�tableSavedAccountsSavedAccountsCREATE TABLE "SavedAccounts" (
"iAccountId"  INTEGER PRIMARY KEY AUTOINCREMENT,
"iServerId"  INTEGER,
"sUsername"  TEXT(125),
"sPassword"  TEXT,
"sCreated"  TEXT(50),
"sModified"  TEXT(50),
"sSymetricKey"  TEXT,
"sInitializationVector"  TEXT
)                                                                                                                                                                                                                                                                                            �q%%�%tableSavedServersSavedServersCREATE TABLE "SavedServers" (
"iServerId"  INTEGER PRIMARY KEY AUTOINCREMENT,
"sName"  TEXT(255),
"sAddress"  TEXT(255),
"iPort"  INTEGER DEFAULT 1597,
"sCreated"  TEXT(50),
"sModified"  TEXT(50)
)P++Ytablesqlite_sequencesqlite_sequenceCREATE TABLE sqlite_sequence(name,seq)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   � ���                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            7                  $             'SavedAccounts%SavedServers   � ���                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             ,              localhostlocalhost=) --  loki.tserver.netloki.tserver.net=   � ��                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      	 '    tbrownFj8*Qm1597tmb	 &    tbrownFj8*Qm1597tmb