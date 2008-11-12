dbamisc - Miscellaneous DBA Tools
Pietro.Picca@cern.ch - 2007-08

----------------------------------------------------------------------------------------------------------------------------

audit_test.sql        Set of SQL statements and PL/SQL scripts for testing auditing feature on INT2R for condition accounts,
                      including tables, procedures and sequences used within
changeauth.sh         Bash script for archiving current authentication files on AFS and replace them with the most recently
                      produced programmatically by updatepwd.php
index.html            HTML summary page, containing further details and project's progression
phpinfo.php           Web script for showing PHP enviroment's information
readme.txt            Present file
updatepwd.css         Style sheet for the following web script
updatepwd.php         Web script for generating two new passwords for condition accounts, either on INT2R or INT9R,
                      one for readers (12 chars long) and the other for writers (16 chars long), and XML related fragments
updatepwd.sql         Set of SQL statements and PL/SQL scripts for generating new passwords for condition accounts,
                      including tables, grants, views, materialized views, functions and procedures used within
weekly_report.sql     Set of SQL statements and PL/SQL scripts for a weekly report notifying accounts' expiry dates, etc.,
                      including views, procedures and jobs used within