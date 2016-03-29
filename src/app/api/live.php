<?php
// Based off of Timothy Pace's GreenWharf datapoint retriever, history.php file

// ChromePhp is a logging tool used for console debugging
include 'ChromePhp.php';

// Sets the default timezone to UTC for the timestamp
date_default_timezone_set('UTC');

// Sets the header and includes the database credentials
// information
header("Content-type: application/json");
include('db_credentials.php');


// Gets the live GET requests regarding, if the
// data should be live, what the associated info_id
// is, and how late the data should be 
$live   = $_GET['live'];
$info   = $_GET['info_id'];
$late   = $_GET['latest'];

// Set of debug messages that display each of the GET requests
if (!empty($live)) {
   ChromePhp::log("LVIE " . $live);
}

if (!empty($info)) {
   ChromePhp::log("info " . $info);
}

if (!empty($late)) {
   ChromePhp::log("late " . $late);
}

// Checks if live data is requested, currently live data is only implemented, historical data is 
// based off the history.php, previous file
if (!empty($live)) {
   if(!empty($info)) {
      $t_data_query = "SELECT * FROM `t_data` WHERE info_id = " . $info . " ORDER BY timestamp ASC ";
      if (!empty($late)) {
         $t_data_query = $t_data_query .  "LIMIT 0, " . $late;
      }
      ChromePhp::log($t_data_query);

      $t_data_info_query = "SELECT * FROM `t_data_info` WHERE id = " . $info;

      ChromePhp::log($t_data_info_query);

      $t_data_equip_query = "SELECT * FROM `t_equipment` WHERE id = " . $info;

      ChromePhp::log($t_data_equip_query);

      //Connect to database and execute query
      $conn = mysql_connect($host, $user, $pw) or die('Could not connect: ' . mysql_error());
      mysql_select_db($db, $conn) or die('No Luck: ' . mysql_error() . "\n");

      $json = array('info' => fetch_info($t_data_info_query), 'equip' => fetch_equi($t_data_equip_query), 'values' => fetch_data($t_data_query)); 
   } 
   
   // In this case, live data is requested and therefore fetched based on the GET parameters 
   else {
      
      // MySQL query created to select all the sensors available (gotten by info row)
      $t_sensor_info_query = "SELECT * FROM `t_sensor_info`";
      ChromePhp::log($t_sensor_info_query);

      // Connect to database and execute query
      // Allows us to executre queries on the database and returns an error
      // if unable to connect to the database
      $conn = mysql_connect($host, $user, $pw) or die('Could not connect: ' . mysql_error());
      mysql_select_db($db, $conn) or die('No Luck: ' . mysql_error() . "\n");

      
      // Response gotten based on sensor info query and json array
      // initialized
      $resp = mysql_query($t_sensor_info_query);
      $json = array();

      // Iterates through each sensor info row, as a result
      // each sensor's information is received and an equipment,
      // actuator, status, and data query is ran for each
      while ($row = mysql_fetch_assoc($resp)) {
         $json_temp = array();
         $sensor_id = $row['id'];
         $equipment_id = $row['equipment_id'];

	 $t_equipment_query =  "SELECT * FROM `t_equipment` WHERE id= " . $equipment_id;
	 $t_actuator_query = "SELECT * FROM `t_actuator_info` WHERE sensor_id= " . $sensor_id;  

         $t_sensor_status_query = "SELECT * FROM `t_data` WHERE sensor_id = " . $sensor_id  . " ORDER BY `timestamp` DESC" . " LIMIT 0, 1";
         ChromePhp::log($t_sensor_status_query);

         $t_data_status = fetch_data_for_status($t_sensor_status_query);

         $t_sensor_info_result = fetch_sensor_info($row, $t_data_status);
         $t_equipment_result = fetch_equipment($t_equipment_query);
         $t_actuator_result = fetch_actuator_info($t_actuator_query);
         $temp_actuator_id = fetch_actuator_id($t_actuator_query);

         $t_sensor_data_query = "SELECT * FROM `t_data` WHERE sensor_id = " . $sensor_id; 
         ChromePhp::log($t_sensor_data_query);

         $t_actuator_data_query = "SELECT * FROM `t_data` WHERE actuator_id = " . $temp_actuator_id;
         ChromePhp::log($t_actuator_data_query);

         if (!empty($late)) {
            $t_sensor_data_query = $t_sensor_data_query . " LIMIT 0, " . $late;
         }


         $t_data_result = fetch_data($t_sensor_data_query, $t_actuator_data_query);

         // Creates final JSON for sensor, and then pushes that onto array of sensors (so each
         // sensor's JSON is in an array)
         $json_temp = array('sensor_info' => $t_sensor_info_result, 'actuator_info' => $t_actuator_result,
			    'equipment' => $t_equipment_result, 'values' => $t_data_result);
         ChromePhp::log($json_temp);
         array_push($json, $json_temp);
      }
   }
}
      
ChromePhp::log("FINAL JSON");
ChromePhp::log($json);

function fetch_data($t_sensor_query, $t_actuator_query) {
   $data_sensor_arr = array();
   $data_actuator_arr = array();
   $resp_sensor = mysql_query($t_sensor_query);
   $resp_actuator = mysql_query($t_actuator_query);
   while ($row = mysql_fetch_assoc($resp_sensor)) {
      if ($row['value'] != 9999) {
         array_push($data_sensor_arr, array(strtotime($row['timestamp'])*1000, (float) ($row['value'])));
      }
   }
   while ($row = mysql_fetch_assoc($resp_actuator)) {
      if ($row['value'] != 9999) {
         array_push($data_actuator_arr, array(strtotime($row['timestamp'])*1000, (float) ($row['value'])));
      }
   }
   return array('sensor' => $data_sensor_arr,  'actuator' => $data_actuator_arr);
}


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

function fetch_sensor_info($row, $status) {
    if($row) {
	return array('name' => $row['name'], 'unit' => $row['unit'], 'longunit' => $row['longunit'], 'info' => $row['info'], 'status' => $status);
    }
}

function fetch_equipment($t_query) {
   $resp = mysql_query($t_query);
   if($row = mysql_fetch_assoc($resp)) {
      return array('name' => $row['name'], 'location' => $row['location'], 'info' => $row['info']);
   }
}

function fetch_actuator_info($t_query) {
   $resp = mysql_query($t_query);
   if($row = mysql_fetch_assoc($resp)) {
      return array('name' => $row['name'],  'info' => $row['info']);
   }
}

function fetch_actuator_id($t_query) {
   $resp = mysql_query($t_query);
   if($row = mysql_fetch_assoc($resp)) {
      return $row['id'];
   }
}

// Writes to the page
echo json_encode($json);
?>
