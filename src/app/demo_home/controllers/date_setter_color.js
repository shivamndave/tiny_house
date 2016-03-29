var GETURL ="../../api/history.php?",
    LIVEURL ="../../api/live.php?",
    hideList = [],
    showList = [],
    init;

// Reloads the date range when submitting the date range through the
// entry area
// function reload_date_range() {
//    var getURL = dateParser($( "#day-picker" ).datepicker( "getDate" ).toISOString(),
//                            $( "#start-time-picker" ).val(),
//                            $( "#end-time-picker" ).val());
//    init = false;
//    hideList = [];
//    showList = [];
//     $.getJSON(getURL, function(data) {
//       checkData(data);
//       // console.log("Charts Shown: " + showList);
//       updateCharts(data, init);
//       toggleDisplays(hideList, showList);
//       setTimeout(reload_date_range, 1000);
//     });
// }

// Gets and sets up live data charts/displays
// This is what is initially loaded onto the page
function get_latest_data(latest) {
   // Initial variables involving the parsed URL and what
   // displays will be hidden and shown
   var getURL = liveParser(0, latest);
   init = true;
   hideList = [];
   showList = [];
   console.log(getURL);
   
   // JQuery call occurs here to GET the latest data from the database
   // through the PHP API
   $.getJSON(getURL, function(data) {
     console.log("data")
     console.log(data)
     
     // Outlines which charts are being displayed
     var list_of_charts = [];
     
     // Iterates through the data received from the GET request
     // which is an array of all sensors with JSON objects associated with
     // each. This array of sensors is stored in data
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
       
       // If the sensor has no actuator information, than it is displayed as a measurement data chart
       else {
        addChartRow(parseDOM, currentData);
        var temp = setup_sensor_chart('#' + parseDOM, currentData);
        list_of_charts.push(temp);
       }
     }
     
     // setInterval is used to refresh the chart every 5 seconds
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
   });
}

// During initialization of charts, based on how many sensors exist
// within the fetched data object (from API) a display is made for each
function addChartRow(statusID, currentData) {
    var chartDiv = document.createElement("div"),
	actuatorDiv = "";

    chartDiv.className = "row";

    // Actuator infomration is displayed if needed, otherwise a placeholder is shown
    if (currentData.actuator_info != null) {
	actuatorDiv = "<p>Actuator Name: " + currentData.actuator_info.name + "</p>" +
		      "<p>Actuator Info: " + currentData.actuator_info.info + "</p>";
    } else {
	actuatorDiv = "<p>No actuator attached to this sensor</p>";
    }

    // Display div that holds the chart and 3 (sensor, actuator, and equipment) information panels
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

     // The div is appended to the page
     document.getElementById('content').appendChild(chartDiv);
     
     // Actuator or sensor info message is set to be loading
     // prior to being updated with latest information
     $("#" + statusID + "-msg").hide();
     if(currentData.values.actuator.length > 0 || currentData.values.sensor.length > 0){
        $("#setpt-" + statusID).text("Loading...");
        $("#setpt-" + statusID).css("font-weight","Bold");
     }
 
}

// Contstructs a date url using the gotten date information, for history.php API (history.php usage)
function dateParser(day, start, end) {
   var tempURL = GETURL,
       parDay = getDate(day),
       parStart = getTime(start),
       parEnd = getTime(end);

   console.log("day: " + parDay + " | str time: " + parStart + " | end time: " + parEnd);
   if (day && parStart && parEnd) {
      tempURL += "&day=" + parDay + "&start=" + parStart + "&end=" + parEnd;
      console.log("API call URL: " + tempURL);
   }

   return tempURL
}

// Parses the URL for the GET Request to the API, see API
// for example/description of this line. (history.php usage)
function liveParser(id, latest) {
   var tempURL = LIVEURL + "live=1&info_id=" + id.toString();

   if(latest > 5) {
      tempURL += "&latest=" + latest.toString();
   }

   return tempURL;
}

// Gets only the date from the datepicker (history.php usage)
function getDate(date) {
   var tempDate = date.slice(0, 10);
   return tempDate;

}

// Gets only the time from the timepicker
// and formats it from 00:11:22 into 00h11m22s (history.php usage)
function getTime(time) {
   var tempTime;
   tempTime = replaceAt(time, 2, "h");
   tempTime = replaceAt(tempTime, 5, "m");
   tempTime += "s";
   return tempTime;
}

// Replaces at a specified character
function replaceAt (str, ind, charac) {
    return str.substr(0, ind) + charac + str.substr(ind + charac.length);
}

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

// Returns whether or not a name is contained
// within the show or hide arrays.
function showObj(hideArr, showArr, name) {
   if(showArr.indexOf(name) > -1) {
      return true;
   } else {
    return false;
  }
}

// Calls iterateData to get information
// on if object contains data. Based on
// response, if it has data, adds to a list
function checkData(dataValues) {
  if (iterateData(dataValues.values.sensor)){
    return true;
  } else {
    return false;
  } 
}

// Recursively iterates through
// an object and data it contains. Iterates
// through the data's "subobjects" until a
// nonzero array is found (returns true) otherwise
// returns false.
function iterateData(data) {
  return isNonzeroArray(data);
}

// Checks if passed in data is an array, 
// non-zero, and is of length > 0.
// Returns if this statement is true
function isNonzeroArray(data) {
  return (Array.isArray(data) &&
    (notEmpty(data)) &&
    (data.length !== 0));
}

// Checks an array contains nonzero numbers
// Returns true if has nonzero numbers, otherwise
// returns false
function notEmpty(dataArray) {
   for (var item in dataArray) {
      if (dataArray[item] !== 0) { 
        return true;
     }
  }
  return false;
}

function updateCharts (dataType, status, statusID, setpointValue) {
    var setptStatusID = "#setpt-" + statusID,
	latestdtStatusID = "#latestdt-" + statusID,
	lateststatStatusID = "#lateststat-" + statusID;
    console.log(dataType.values.sensor);
    if(dataType.actuator_info && dataType.values.sensor.length > 0 && dataType.values.actuator.length > 0) {
	console.log("dataType setpoint");
	var lateVal = dataType.values.sensor[dataType.values.sensor.length - 1][1],
        t_set = "",
        val = Math.abs(setpointValue - lateVal),
        roundVal = Number(Math.round(val+'e2') + 'e-2'),
        diff = roundVal.toString() + " " + dataType.sensor_info.longunit;
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
	text = "Latest value " + textVal + " is currently " + t_set + "the setpoint of " + setpointValue.toString() + " " + dataType.sensor_info.longunit,
	latestTimeText = getLatestTime(dataType),
	latestStatText = getLatestStat(dataType);

    $(setptStatusID).text(text);
    $(setptStatusID).css("font-weight","Bold");

    $(latestdtStatusID).text(latestTimeText);
    $(lateststatStatusID).text(latestStatText);

    status.series[0].setData(dataType.values.sensor, true);
    console.log("ADDING PLOT LINE");
    if(status.yAxis.length == 0) {
      status.yAxis[0].addPlotLine({
          value: setpointValue,
          color: 'red',
          width: 2,
          id: 'plot-line-1',
          color: 'red',
          dashStyle: 'shortdash',
          label: {
            text: 'Setpoint'
          }
      });
    } else {
      status.yAxis[0].removePlotLine('plot-line-1');
      status.yAxis[0].addPlotLine({
        value: setpointValue,
        color: 'red',
        width: 2,
        id: 'plot-line-1',
        color: 'red',
        dashStyle: 'shortdash',
        label: {
          text: 'Setpoint'
        }
    });
    }
   } else {
    console.log("dataType sensor");
    status.series[0].setData(dataType.values.sensor, true);
    var lateVal = dataType.values.sensor[dataType.values.sensor.length - 1][1],
	textVal = lateVal + " " + dataType.sensor_info.longunit,
	text = "Latest value: " + textVal,
	latestTimeText = getLatestTime(dataType),
	latestStatText = getLatestStat(dataType);;

    $(setptStatusID).text(text);
    $(setptStatusID).css("font-weight","Bold");
    $(setptStatusID).css("color","#000000");
    $(latestdtStatusID).text(latestTimeText);
    $(lateststatStatusID).text(latestStatText);
   }
}

// Displays the time of the latest piece of data
function getLatestTime(dataType) {
    var latestTimeVal = dataType.values.sensor[dataType.values.sensor.length - 1][0],
	latestTimeConv = new Date(latestTimeVal);
    return "Latest time: " + String(latestTimeConv);
}

// Used for the status message and formatting of it
function getLatestStat(dataType) {
    var latestStatVal = dataType.sensor_info.status,
	statusText = "";

    if(latestStatVal == 0) {
	statusText = "OFF";
    } else {
	statusText = "ON";
    }
    return "Latest status: " + statusText;
}
