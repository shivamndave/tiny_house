#PHP API

* About
* Files
 * live.php
 * db_credentials.php (or db_credentials_sample.php)
* URL Requests

###About
The PHP API is used to query the phpMyAdmin database. Currently all queries are for live data. Queries mainly are used to aquire information and measurements/actuator data about sensors. However other information such as units, location, and descriptions have can also be queried and accessed. 

###Files
The API is primarily used with two files (__live.php__ and __db_credentials.php__), an additional history.php file also has been used in the past, however it has not been updated because of the prioritization of live data and the ability of highcharts to showcase historical data easily

#####live.php
The live.php file parses and sends queries in sql for the database based on GET requests made by the frontend (dashboard). Essentially it parses a URL request, performs queries, and then formats the data gotten in JSON arrays. 

#####db_credentials.php (or db_credentials_sample.php)
###URL Requests
