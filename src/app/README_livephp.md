# Overview of [live.php](/live.php)
### Major functions and areas of the [live.php](/live.php) file are broken down below 


Query used to obtain sensor information. This is where we are able to go through each sensor and also setup a connection to the database based on the db_credential's file credentials. Either you will be able to connect to the database or the desiganted arror will be returned.
```php
$t_sensor_info_query = "SELECT * FROM `t_sensor_info`";
$conn = mysql_connect($host, $user, $pw) or die('Could not connect: ' . mysql_error());
mysql_select_db($db, $conn) or die('No Luck: ' . mysql_error() . "\n");

$resp = mysql_query($t_sensor_info_query);
```
Based on the sensor information query, each row in the t_sensor_info table is iterated through. As a result we are given the information regarding each sensor, one at a time during the loop until all sensors have been iterated through. This allows us to get the relative data and information regarding the sensor during each loop,  populating the dasboard basedon how many sensors are available to display.
```php
while ($row = mysql_fetch_assoc($resp)) { ...
```

Queries for the equipment, actuator, and sensor status are parsed. Essentially, for each sensor (that we are currently on) the equipment, actuator, and sensor status information query is created here.
```php
$t_equipment_query =  "SELECT * FROM `t_equipment` WHERE id= " . $equipment_id;
$t_actuator_query = "SELECT * FROM `t_actuator_info` WHERE sensor_id= " . $sensor_id;  
$t_sensor_status_query = "SELECT * FROM `t_data` WHERE sensor_id = " . $sensor_id  . " ORDER BY `timestamp` DESC" . " LIMIT 0, 1";
```

Queries for data based on `sensor_id` gotten from the current sensor's sensor information. This is used to obtain measurement data (sensor data) and setpoints (actuator data). 
```php
$t_sensor_data_query = "SELECT * FROM `t_data` WHERE sensor_id = " . $sensor_id; 
$t_actuator_data_query = "SELECT * FROM `t_data` WHERE actuator_id = " . $temp_actuator_id;
```

`$t_data_result` is used to hold the data gathered from the `fetch_data` function. The `fetch_data` function essentially uses a sensor query (`$row` that is gotten when iterating through sensor information rows) and the actuator query. All the rows of data associated with each query is pushed onto 2 different arrays and those arrays are returned in JSON format. 
```php
$t_data_result = fetch_data($t_sensor_data_query, $t_actuator_data_query);
```
```php
function fetch_data($t_sensor_query, $t_actuator_query) {
   // Sensor data array created
   $data_sensor_arr = array();
   $resp_sensor = mysql_query($t_sensor_query);
   
   while ($row = mysql_fetch_assoc($resp_sensor)) {
      if ($row['value'] != 9999) {
         array_push($data_sensor_arr, array(strtotime($row['timestamp'])*1000, (float) ($row['value'])));
      }
   }
   
   // Actuator data array created
   $data_actuator_arr = array();
   $resp_actuator = mysql_query($t_actuator_query);
   
   while ($row = mysql_fetch_assoc($resp_actuator)) {
      if ($row['value'] != 9999) {
         array_push($data_actuator_arr, array(strtotime($row['timestamp'])*1000, (float) ($row['value'])));
      }
   }
   
   // Both arrays returned using dictionary
   return array('sensor' => $data_sensor_arr,  'actuator' => $data_actuator_arr);
}
```

`t_data_status` get's the status of the sensor using the `fetch_data_for_status` function. The query for the sensor_status is the same as the sensor data query (`$t_sensor_data_query` however with a limit on the number of results. The latest data point is gotten and based on its value, a true or false (1 or 0) value is returned.
```php
$t_data_status = fetch_data_for_status($t_sensor_status_query);
```
```php
function fetch_data_for_status($t_sensor_query) {
   $data_sensor_arr = -1;
   $resp_sensor = mysql_query($t_sensor_query);
    ChromePhp::log("STATUS");
	
   if ($row = mysql_fetch_assoc($resp_sensor)) {
    ChromePhp::log((float) $row['value']);
      if ((float) $row['value'] != 9999) {
         $data_sensor_arr = 1;
      } else if ((float) $row['value'] == 9999) {
	 $data_sensor_arr = 0;
      }
   }
  return $data_sensor_arr;
}
```

Simply gets the current sensor information which is the `$row` that represents the current sensor that we are on in the loop. This also utilizes the `$t_data_status` which is obtained above and is the true or false value that let's us know if the sensor is operating or not.
```php
$t_sensor_info_result = fetch_sensor_info($row, $t_data_status);
```
```php
// Fetches sensor information
function fetch_sensor_info($row, $status) {
    if($row) {
	return array('name' => $row['name'], 'unit' => $row['unit'], 'longunit' => $row['longunit'], 'info' => $row['info'], 'status' => $status);
    }
}
```

Using `$t_equipment_query`, the equipment information for the current sensor is requested and allowed to be used by the frontend.
```php
$t_equipment_result = fetch_equipment($t_equipment_query);
```
```php
function fetch_equipment($t_query) {
   $resp = mysql_query($t_query);
   if($row = mysql_fetch_assoc($resp)) {
      return array('name' => $row['name'], 'location' => $row['location'], 'info' => $row['info']);
   }
}

```
Using `$t_actuator_query`, the actuator information for the current sensor is requested and allowed to be used by the frontend. The result is in JSON format.  The result is in JSON format.
```php
$t_actuator_result = fetch_actuator_info($t_actuator_query);
```
```php
function fetch_actuator_info($t_query) {
   $resp = mysql_query($t_query);
   if($row = mysql_fetch_assoc($resp)) {
      return array('name' => $row['name'],  'info' => $row['info']);
   }
}
```

Helper function that only returns the actuator id for the current sensor. This allows us to get actuator data in the `fetch_data` function.
```php
$temp_actuator_id = fetch_actuator_id($t_actuator_query);
```
```php
function fetch_actuator_id($t_query) {
   $resp = mysql_query($t_query);
   if($row = mysql_fetch_assoc($resp)) {
      return $row['id'];
   }
}
```

Used if the latest flag is used. This limits the results based on the number specified by the frontend request. The result is in JSON format.
```php
if (!empty($late)) {
	$t_sensor_data_query = $t_sensor_data_query . " LIMIT 0, " . $late;
}
```

Creates final JSON. This is what is displayed on the url request page and what is parsed through by the frontend to display information and data.
```php
$json_temp = array('sensor_info' => $t_sensor_info_result,
				   'actuator_info' => $t_actuator_result,
                   'equipment' => $t_equipment_result,
                   'values' => $t_data_result);

array_push($json, $json_temp);
```
