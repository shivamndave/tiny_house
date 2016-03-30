## Overview of `data_setter_color.js`
```JavaScript
    LIVEURL ="../../api/live.php?",
```
This is the base URL that performs the PHP API requests. The live.php file is for live data and is the API file used for this version of the dashboard

```JavaScript
function get_latest_data(latest) {
```
The initial function that is shown when the user loads the page. This kicks off with initial data and within this function data is updated and "refreshed periodically for the user

```JavaScript
   var getURL = liveParser(0, latest);
```
```JavaScript
function liveParser(id, latest) {
   var tempURL = LIVEURL + "live=1&info_id=" + id.toString();

   if(latest > 5) {
      tempURL += "&latest=" + latest.toString();
   }

   return tempURL;
}
```
The `liveParser` URL that is generated is perform GET requests using the live PHP API. The URL structure is described in the API README file. however this is essentially what grabs the call the sensor data objects available from the database.


```JavaScript
     for(iter_i = 0; iter_i < data.length; iter_i++) {
       var parseDOM = 'device-' + iter_i.toString(),
           currentData = data[iter_i];
       
       // Checks if the sensor is also an actuator, if actuator information is associated
       // with the sensor, then its display is slightly altered with a setpoint chart
       if(currentData.actuator_info && currentData.values.sensor.length > 0 && currentData.values.actuator.length > 0) {
        addChartRow(parseDOM, currentData);
        var setpointValue = currentData.values.actuator[currentData.values.actuator.length - 1][1];
        var temp = setup_setpoint_chart('#' + parseDOM, currentData, setpointValue);
        list_of_charts.push(temp);
       }
```
The sensor data objects with actuator information are iterated through. This is the initial iteration that is used to setup the charts and load in information. View the `addChartRow` function details to see how the display is created for each sensor data object that has been iterated through. This utilizes a different setup than nonactuators because its chart will involve setpoints.

In addtion to this, the `list_of_charts` array is used to keep track of each generated display and what will eventually be displayed. This array is checked for empty data sets, therefore sensors that are inactive and have no data flow currently or historically.

```JavaScript
       else {
        addChartRow(parseDOM, currentData);
        var temp = setup_sensor_chart('#' + parseDOM, currentData);
        list_of_charts.push(temp);
       }
```
Sensor data objects with no actuator info are setup otherwise. View the `addChartRow` function details to see how the display is created for each sensor data object that has been iterated through. This utilizes a different setup than above because its chart will be slightly different and involve no setpoints.

```JavaScript
     setInterval(function() {
     	
       // Another JQuery call is used to consistently get the updated data/information
       $.getJSON(getURL, function(updateData) {
       	
       	  // Similar to above, the updateData is iterated through, to get information
       	  // & data for each sensor
          for (iter_i = 0; iter_i < list_of_charts.length; iter_i++) {
            var setpointValue = 0;
            var showChart = checkData(data[iter_i]);
            var parseDOM = 'device-' + iter_i.toString();
            console.log(iter_i);
            
            // If an actuator value is set for the measurement, than the latest one is used as the
            // setpoint value
            if(updateData[iter_i].values.actuator.length > 0){
              var setpointValue = updateData[iter_i].values.actuator[updateData[iter_i].values.actuator.length - 1][1];
            }
            
            // The sensor's chart is updated or toggled based on if it should be shown
            updateCharts(updateData[iter_i], list_of_charts[iter_i], parseDOM, setpointValue);
            toggleDisplays(showChart, parseDOM);
          }
       });
     }, 5000);
```
At an interval of 5 seconds, a GET request is performed to pull updated data. Utilizing the updateCharts function, the information and data being displayed is updated. Also here is where the charts will be toggled based on if they are empty or not. As a result charts that have no data will be hidden and as soon as they are running and collecting data, they will be displayed. Actuator values are also updated here.

Utilizing JQuery, information and data values that require updating are also changed in updateCharts and passed in values here.

### Getting new data using `updateCharts`
```JavaScript
function updateCharts (dataType, status, statusID, setpointValue) {
```
The updateCharts function is where all the updates and utilization of the API GET requests is done. The `dataType` (sensor or actuator), `status` of the the device, `statusID` (DOM id associated with the display), and `setPointValue` (if available) are all required and passed into the function.

```JavaScript
    if(dataType.actuator_info && dataType.values.sensor.length > 0 && dataType.values.actuator.length > 0) {
```
The check done is to see if the sensor is an actuator. Actuator's have different information and display data that must be taken into account

```JavaScript
	var lateVal = dataType.values.sensor[dataType.values.sensor.length - 1][1],
            t_set = "",
        val = Math.abs(setpointValue - lateVal),
        roundVal = Number(Math.round(val+'e2') + 'e-2'),
        diff = roundVal.toString() + " " + dataType.sensor_info.longunit;
```
The latestactuator value is grabbed and compared with the latest data value. Depending on the difference between the two, a different message is shown (below). Also the difference is displayed for the user to know how much the setPoint differs from incoming data.

```JavaScript
    if(lateVal > setpointValue){
	t_set = diff + " over ";
	$(setptStatusID).css("color","#1630F2");
    } else if (lateVal < setpointValue){
      t_set = diff + " under ";
      $(setptStatusID).css("color","#F21616");
    } else {
       t_set = " equal to ";
       $(setptStatusID).css("color","#000000");
    }
    var textVal = lateVal + " " + dataType.sensor_info.longunit,
	text = "Latest value " + textVal + " is currently " + t_set + "the setpoint of " + setpointValue.toString() + " " + 		   dataType.sensor_info.longunit,

	latestTimeText = getLatestTime(dataType),
	latestStatText = getLatestStat(dataType);
```
The setpoint comparison will display whether the incoming data is under, over, or equal to the setpoint. Depending on this message, styling is adjusted and the dynamic text (status, time, and setpoint comparison) is inserted here

```JavaScript
   else {
    console.log("dataType sensor");
    
    // Updates data shown on chart
    status.series[0].setData(dataType.values.sensor, true);
    
    // Instead of an actuator message, the latest time, status, and data value is displayed
    var lateVal = dataType.values.sensor[dataType.values.sensor.length - 1][1],
	textVal = lateVal + " " + dataType.sensor_info.longunit,
	text = "Latest value: " + textVal,
	latestTimeText = getLatestTime(dataType),
	latestStatText = getLatestStat(dataType);

    // Inserts the setpoint text (which is just the latest data
    // value since no setpoint) into the display and formats it
    $(setptStatusID).text(text);
    $(setptStatusID).css("font-weight","Bold");
    $(setptStatusID).css("color","#000000");
    
    // Inserts the status and latest time text
    $(latestdtStatusID).text(latestTimeText);
    $(lateststatStatusID).text(latestStatText);
   }
}
```
If the sensor is not an actuaor, a similar process is done, however with the actuator (over/under) info being changed to a message regarding what the last piece of data was. This is also used on actuators that have no setpoint yet.

### Display Insertion
These function components are regarding adding new displays and preparing/setting up the displays to be updated
```JavaScript
    if (currentData.actuator_info != null) {
	actuatorDiv = "<p>Actuator Name: " + currentData.actuator_info.name + "</p>" +
		      "<p>Actuator Info: " + currentData.actuator_info.info + "</p>";
    } else {
	actuatorDiv = "<p>No actuator attached to this sensor</p>";
    }
```
The actuator information is filled in here, if the sensor is not an actuator, a placeholder lets us know the type of device instead.


```JavaScript
    chartDiv.innerHTML = "<div class='rx-ish'>"+
	"<div class='col-md-6 chart chart-container' id=" + statusID + 
	" style='margin: 0 auto;'> </div> <h2 id=" + statusID + "-msg> was under maintainance. Charts cannot be displayed </h2>"+
        "</div><div class='col-md-6'>"+
	    "<div class='row row-nrm'>"+
		"<div class='col-md-4 back-sensor'>"+
	            "<p>Sensor Name: " + currentData.sensor_info.name + "</p>" + 
		    "<p>Sensor Info: " + currentData.sensor_info.info + "</p>" +
		"</div>" +
		"<div class='col-md-4 back-equip'>" +
		    "<p>Equipment Name: " + currentData.equipment.name + "</p>" +
		    "<p>Location: " + currentData.equipment.location + "</p>" +
		    "<p>Eq Info: " + currentData.equipment.info + "</p>" +
		"</div>" +
		"<div class='col-md-4 back-actua'>" + 
		    actuatorDiv +
		"</div>" +
	    "</div>" +

	    "<div class='col-md-12 row row-nrm back-set'"+
		"<p id=setpt-" + statusID + "></p>" +
	    "</div>" +


	    "<div class='col-md-12 row row-nrm back-ts'>" +
		"<div class='col-md-6'>" +
		    "<p id=latestdt-" + statusID  + ">Loading latest time...</p>" +
		"</div>" +
		"<div class='col-md-6'>" +
		    "<p id=lateststat-" + statusID  + ">Loading latest status...</p>" +
		"</div>" +
	    "</div>" +

        "</div></div>";
```
Embedded HTML that is appended to the page as a display. This also uses BootStrap to maintain a consisten and clean look. 
\
It is 2 columns:
1. _Chart_

The chart column is simply the HighChart that is updated with the latest data or historical data. Within the chart, the bottom bar allows for time resolution changes and the top buttons allow to change the view increment of the time displayed

2. _Information_

The Information portion is split into three rows, the first row with three columns. The first row shows current sensor's equipment, general, and actuator (if available) information. The next row is dynamically updated and it is the setpoint (if an actuator) or latest data point for the sensor. THe Last row is split into two columns, where both are dynamically updated. One is the status of the device while the other is the latest time that data was collected.

```JavaScript
function toggleDisplays(show, dom) {
   if(show) {
      console.log("SHO " + dom);
      $("#" + dom).show();
      $("#" + dom + "-msg").hide();
   } else {
      console.log("HID " + dom);
      $("#" + dom).hide();
      $("#" + dom + "-msg").show();
   }
}
```
Utilizes helper functions to check whether a display should be shown or hidden. If a sensor has no data values for it, than it will not be displayed



