<?php
include 'ChromePhp.php';
// Based off of Timothy Pace's GreenWharf datapoint retriever, history.php file

date_default_timezone_set('UTC');

header("Content-type: application/json");
include('db_credentials.php');


// Gets the start and end time in addition to
// the day these times will occur
$live   = $_GET['live'];
$info   = $_GET['info_id'];
$late   = $_GET['latest'];

if (!empty($live)) {
   ChromePhp::log("LVIE " . $live);
}

if (!empty($info)) {
   ChromePhp::log("info " . $info);
}

if (!empty($late)) {
   ChromePhp::log("late " . $late);
}

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

   } else {
      $t_sensor_info_query = "SELECT * FROM `t_sensor_info`";

      ChromePhp::log($t_sensor_info_query);

      //Connect to database and execute query
      $conn = mysql_connect($host, $user, $pw) or die('Could not connect: ' . mysql_error());
      mysql_select_db($db, $conn) or die('No Luck: ' . mysql_error() . "\n");

      $resp = mysql_query($t_sensor_info_query);
      $json = array();
      while ($row = mysql_fetch_assoc($resp)) {
         $json_temp = array();
         $sensor_id = $row['id'];
         $equipment_id = $row['equipment_id'];

	 $t_equipment_query =  "SELECT * FROM `t_equipment` WHERE id= " . $equipment_id;
	 $t_actuator_query = "SELECT * FROM `t_actuator_info` WHERE id= " . $sensor_id;
         
         $t_data_query = "SELECT * FROM `t_data` WHERE info_id = " . $sensor_id;
         if (!empty($late)) {
            $t_data_query = $t_data_query .  "LIMIT 0, " . $late;
         }

         $t_sensor_info_result = fetch_sensor_info($row);
         $t_equipment_result = fetch_equipment($t_equipment_query);
         //$t_actuator_result
         //$t_data_result
         $json_temp = array('sensor_info' => $t_sensor_info_result, 'equipment' => $t_equipment_result, 'values' => fetch_data($t_data_query));
         ChromePhp::log($json_temp);
         array_push($json, $json_temp);
      }
   }
}
      
// Latest 25
//$t_data_freq_latest_limit = "SELECT * FROM `t_data`" . " WHERE info_id = 1 ORDER BY id ASC LIMIT 0, 25";
//$t_data_temp_latest_limit = "SELECT * FROM `t_data`" . " WHERE info_id = 2 ORDER BY id ASC LIMIT 0, 25";
// Latest
//$t_data_freq_latest = "SELECT * FROM `t_data`" . " WHERE info_id = 1 ORDER BY timestamp ASC";
//$t_data_temp_latest = "SELECT * FROM `t_data`" . " WHERE info_id = 2 ORDER BY timestamp ASC";

ChromePhp::log("FINAL JSON");
ChromePhp::log($json);

function fetch_data($t_data) {
   $data_json = array();
   $resp = mysql_query($t_data);
   while ($row = mysql_fetch_assoc($resp)) {
      if ($row['value'] != 999) {
         array_push($data_json, array(strtotime($row['timestamp'])*1000, (float) ($row['value'])));
      }
   }
   return $data_json;
}

function fetch_sensor_info($row) {
    if($row) {
	return array('name' => $row['name'], 'unit' => $row['unit'], 'longunit' => $row['longunit'], 'info' => $row['info']);
    }
}

function fetch_equipment($t_query) {
   $resp = mysql_query($t_query);
   if($row = mysql_fetch_assoc($resp)) {
      return array('name' => $row['name'], 'location' => $row['location'], 'info' => $row['info']);
   }
}

// Writes to the page
echo json_encode($json);
?>
