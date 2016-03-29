#Backend: API and Database
Currently the information shown is regarding the PHP API and the database (therefore encompassing the entire database). The information here is key if you are trying to the following and more:
  1. Setup the database & data flow
  2. Use the API
  3. Monitor data from the sensors

##PHP API
* About
* Files
 * live.php
 * db\_credentials.php (or db\_credentials\_sample.php)
* URL Requests

###About
The PHP API is used to query the phpMyAdmin database. Currently all queries are for live data. Queries mainly are used to aquire information and measurements/actuator data about sensors. However other information such as units, location, and descriptions have can also be queried and accessed. 

###Files
The API is primarily used with two files (__live.php__ and __db\_credentials.php__), an additional history.php file also has been used in the past, however it has not been updated because of the prioritization of live data and the ability of highcharts to showcase historical data easily

#####live.php (Go to [README_livephp](./docs/README_livephp.md) for a code overview)
The live.php file parses and sends queries in sql for the database based on GET requests made by the frontend (dashboard). Essentially it parses a URL request, performs queries, and then formats the data gotten in JSON arrays. 

#####db\_credentials.php (or db\_credentials_sample.php)
_DO NOT UPLOAD THIS FILE. Currently it is added to `.gitignore`. This file's information should only be stored locally because of it containing sensitive information_

The db_credentials.php is not uploaded, however a sample version of it is. This is imported into the live.php file and allows database access. It holds information regarding what database is being queried, whom is accessing it, and what password is required to access it.
