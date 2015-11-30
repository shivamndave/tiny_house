<?php
include 'ChromePhp.php';
// Based off of Timothy Pace's GreenWharf datapoint retriever, history.php file

date_default_timezone_set('UTC');

header("Content-type: application/json");
include('db_credentials.php');


// Gets the start and end time in addition to
// the day these times will occur
$start_date_UTC = $_GET['start'];
$end_date_UTC   = $_GET['end'];
$date_UTC      = $_GET['day'];

// Parses the day and the time from the URL Gets
if (isset($date_UTC)) {
   $date_UTC      .= " ";
   $final_start_UTC = substr_replace($start_date_UTC,":", 2, 1);
   $start_UTC = substr_replace($final_start_UTC,":", 5, 1);
   $final_end_UTC = substr_replace($end_date_UTC,":", 2, 1);
   $end_UTC = substr_replace($final_end_UTC,":", 5, 1);

   $start_ts = $date_UTC;
   $end_ts = $date_UTC;

   $start_ts .= substr($start_UTC, 0, -1);
   $end_ts .= substr($end_UTC, 0, -1);
}

ChromePhp::log("Start " . $start_ts);
ChromePhp::log("End " . $end_ts);

// Checks if the start and end time are within a range that can be gotten
// and if not it it will get the entire database
if (isset($start_ts)) {
   if (isset($end_ts)){
        $t_data_freq = "SELECT * FROM `t_data` WHERE timestamp >= '" . $start_ts . "' and timestamp <= '" . $end_ts . "' and info_id = 1 ORDER BY timestamp asc";
        $t_data_temp = "SELECT * FROM `t_data` WHERE timestamp >= '" . $start_ts . "' and timestamp <= '" . $end_ts . "' and info_id = 2 ORDER BY timestamp asc";
   }
   else {
      $t_data_freq = "SELECT * FROM `t_data` WHERE timestamp >= '" . $start_ts . "' and info_id = 1 ORDER BY timestamp asc";
      $t_data_temp = "SELECT * FROM `t_data` WHERE timestamp >= '" . $start_ts . "' and info_id = 2 ORDER BY timestamp asc";
   }
}
else {
   $t_data_freq = "SELECT * FROM `t_data`" . " WHERE info_id = 1 ORDER BY timestamp asc";
   $t_data_temp = "SELECT * FROM `t_data`" . " WHERE info_id = 2 ORDER BY timestamp asc";
}

ChromePhp::log($t_data_freq);
ChromePhp::log($t_data_temp);

$json['temperature'] = array();
$json['frequency'] = array();

//Connect to database and execute query
$conn = mysql_connect($host, $user, $pw) or die('Could not connect: ' . mysql_error());
mysql_select_db($db, $conn) or die('No Luck: ' . mysql_error() . "\n");

// Queries the database and contructs arrays for the temperature and frequency based on
// the float temp/freq paired with a unix formatted time.
$resp_freq = mysql_query($t_data_freq);
while ($row = mysql_fetch_assoc($resp_freq)) {
   if ($row['value'] != 999) {
   array_push($json['frequency'], array(strtotime($row['timestamp'])*1000, (float) ($row['value'])));
  }
}

$resp_temp = mysql_query($t_data_temp);
while ($row = mysql_fetch_assoc($resp_temp)) {
   if ($row['value'] != 999) {
   array_push($json['temperature'], array(strtotime($row['timestamp'])*1000, (float) ($row['value'])));
  }
}

// Writes to the page
echo json_encode($json);
?>
