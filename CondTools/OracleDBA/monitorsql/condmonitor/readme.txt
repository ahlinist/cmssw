condmonitor - Condition DB Monitoring Tools
Pietro.Picca@cern.ch - 2007-08

---------------------------------------------------------------------------------------------------------------------------

cmspietro.css       Style sheet for graphical layout and tables' aspect
cmsrmon.php         Web script to show summary information about condition accounts on CMSR, a.k.a. CMS_ORCOFF_PROD
detactivity.php     Web script to show condition accounts' activity on offline servers
detdetails.php      Web script to show detailed information about condition accounts' database object on offline servers
hist_mon.sql        Set of SQL statements and PL/SQL scripts for recording historical data for condition accounts,
                    including tables, grants, views, indexes, procedures and jobs used within
int2rmon.php        Web script to show summary information about condition accounts on INT2R, a.k.a. CMS_ORCOFF_PREP
int9rmon.php        Web script to show summary information about condition accounts on INT9R, a.k.a. CMS_ORCOFF_INT
mon.php             Web script containing the engine for int2rmon.php, int9rmon and cmsrmon.php
mon_log.sql         Set of SQL statements and PL/SQL scripts for detecting and monitoring utilization of the present tools,
                    including tables, indexes, views, packages, triggers and sequences used within