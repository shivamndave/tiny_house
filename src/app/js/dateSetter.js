var GETURL ="api/history.php?",
    LIVEURL ="api/live.php?",
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

// Gets live data and displays it initially
function get_latest_data(latest) {
   var getURL = liveParser(0, latest);
   init = true;
   hideList = [];
   showList = [];
   console.log(getURL);
   $.getJSON(getURL, function(data) {
     console.log("data")
     console.log(data)
     var list_of_charts = [];
     for(iter_i = 0; iter_i < data.length; iter_i++) {
       var parseDOM = 'device-' + iter_i.toString(),
           currentData = data[iter_i];
       if(currentData.actuator_info && currentData.values.sensor.length > 0 && currentData.values.actuator.length > 0) {
        addChartRow(parseDOM, currentData);
        var setpointValue = currentData.values.actuator[currentData.values.actuator.length - 1][1];
        var temp = setup_setpoint_chart('#' + parseDOM, currentData, setpointValue);
        list_of_charts.push(temp);
       } else {
        addChartRow(parseDOM, currentData);
        var temp = setup_sensor_chart('#' + parseDOM, currentData);
        list_of_charts.push(temp);
       }
     }
     // console.log("list_")
     // console.log(list_of_charts)
     // tempStatus = setup_sensor_chart('#tempstatus', data[0]);
     // freqStatus = setup_sensor_chart('#freqstatus', data[1]);
     setInterval(function() {
       $.getJSON(getURL, function(updateData) {
          // checkData(data[0].values);
          // checkData(data[1].values);
          // console.log("Charts Shown: " + showList);
          // updateCharts(data[0], tempStatus);
          // updateCharts(data[1], freqStatus);
          for (iter_i = 0; iter_i < list_of_charts.length; iter_i++) {
            var setpointValue = 0;
            var showChart = checkData(data[iter_i]);
            var parseDOM = 'device-' + iter_i.toString();
            console.log(iter_i);
            if(updateData[iter_i].values.actuator.length > 0){
              var setpointValue = updateData[iter_i].values.actuator[updateData[iter_i].values.actuator.length - 1][1];
            }
            updateCharts(updateData[iter_i], list_of_charts[iter_i], "#setpt-" + parseDOM, setpointValue);
            toggleDisplays(showChart, parseDOM);
          }
       });
     }, 5000);
   });
}

function addChartRow(statusID, currentData) {
    var chartDiv = document.createElement('div');

    chartDiv.className = 'row';

    chartDiv.innerHTML = '<div>\
        <div class="col-md-6 chart" id=' + statusID + ' style="margin: 0 auto;">\
        </div>\
        <h2 id=' + statusID + '-msg> was under maintainance. Charts cannot be displayed </h2>\
        </div>\
        <div class="col-md-6">\
        <p>' + 'Sensor Name: ' + currentData.sensor_info.name + '</p>' + 
        '<p>' + 'Sensor Info: ' + currentData.sensor_info.info + '</p>' + 
        '<p>' + 'Equipment Name: ' + currentData.equipment.name + '</p>' + 
        '<p>' + 'Eq Info: ' + currentData.equipment.info + '</p>' + 
        '<p>' + 'Location: ' + currentData.equipment.location + '</p>' + 
        '<p id=setpt-' + statusID + '>' + '</p>' + 
        '</div>\
        </div>';

     document.getElementById('content').appendChild(chartDiv);
     $("#" + statusID + "-msg").hide();
     if(currentData.values.actuator.length > 0){
        $("#setpt-" + statusID).text("Loading...");
        $("#setpt-" + statusID).css("font-weight","Bold");
     }
 
}

// Contstructs the url using the gotten
// date information
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

function liveParser(id, latest) {
   var tempURL = LIVEURL + "live=1&info_id=" + id.toString();

   if(latest > 5) {
      tempURL += "&latest=" + latest.toString();
   }

   return tempURL;
}

// Gets only the date from the datepicker
function getDate(date) {
   var tempDate = date.slice(0, 10);
   return tempDate;

}

// Gets only the time from the timepicker
// and formats it from 00:11:22 into 00h11m22s
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

// Checks if an object name is in either
// the hide or show lists, based on that
// the display is shown or hidden, and the
// message is hidden or shown.
// function toggleDisplays(showList, hideList) {
//    if(showObj(showList, hideList, "temperature")) {
//       $("#tempstatus").show();
//       $("#temphide").hide();
//    } else {
//       $("#tempstatus").hide();
//       $("#temphide").show();
//    }
//    if(showObj(showList, hideList, "frequency")) {
//       $("#freqstatus").show();
//       $("#freqhide").hide();
//    } else {
//       $("#freqstatus").hide();
//       $("#freqhide").show();
//    }
// }

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

// function setupCharts (data) {
//    setup_sensor_chart('#freqstatus', data[0]);
//    setup_sensor_chart('#tempstatus', data[1]);
// }

function updateCharts (dataType, status, statusID, setpointValue) {
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
    } else if (lateVal < setpointValue){
      t_set = diff + " under ";
    } else {
       t_set = " equal to ";
    }
    var textVal = lateVal + " " + dataType.sensor_info.longunit;
    var text = "Latest value " + textVal + " is currently " + t_set + "the setpoint of " + setpointValue.toString();
    $(statusID).text(text);
    $(statusID).css("font-weight","Bold");
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
   }
}
