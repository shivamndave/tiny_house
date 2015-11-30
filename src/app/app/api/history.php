<?php
include 'ChromePhp.php';
// Based off of Timothy Pace's GreenWharf datapoint retriever, history.php file

date_default_timezone_set('UTC');

header("Content-type: application/json");
include('db_credentials.php');

$start_date_UTC = $_GET['start'];
$end_date_UTC   = $_GET['end'];
$date_UTC      = $_GET['day'];

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

ChromePhp::log($start_ts);
ChromePhp::log($end_ts);

if (isset($start_ts)) {
   if (isset($end_ts)){
        $t_data = "SELECT * FROM `t_data` WHERE timestamp >= '" . $start_ts . "' and timestamp <= '" . $end_ts . "' ORDER BY timestamp asc";
   }
   else {
      $t_data = "SELECT * FROM `t_data` WHERE timestamp >= '" . $start_ts . "' ORDER BY timestamp asc";
   }
}
else {
   $t_data = "SELECT * FROM `t_data`";
}

ChromePhp::log($t_data);

$json['temperature'] = array();
$json['frequency'] = array();

//Connect to database and execute query
$conn = mysql_connect($host, $user, $pw) or die('Could not connect: ' . mysql_error());
mysql_select_db($db, $conn) or die('No Luck: ' . mysql_error() . "\n");

$resp = mysql_query($t_data);
while ($row = mysql_fetch_assoc($resp)) {
   if ($row['value'] != 999) {
   array_push($json['temperature'], array(strtotime($row['timestamp'])*1000, (float) ($row['value'])));
   array_push($json['frequency'], array(strtotime($row['timestamp'])*1000, (float) ($row['value'])));
  }
}

echo json_encode($json);
?>
