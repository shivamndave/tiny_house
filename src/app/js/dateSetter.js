var GETURL ="api/history.php?",
    hideList = [],
    showList = [],
    init;

// Reloads the date range when submitting the date range through the
// entry area
function reload_date_range() {
   var getURL = dateParser($( "#day-picker" ).datepicker( "getDate" ).toISOString(),
                           $( "#start-time-picker" ).val(),
                           $( "#end-time-picker" ).val());
   console.log("URL:" + getURL);
   init = false;
   hideList = [];
   showList = [];
   $.getJSON(getURL, function(data) {
      checkData(data);
      console.log("Charts Shown: " + showList);
      updateCharts(data, init);
      toggleDisplays(hideList, showList);
   });
}

// Inital chart loading using passed in default time
// period
function initial_date_range(day, start, end) {
   var getURL = dateParser(day, start, end);
   init = true;
   hideList = [];
   showList = [];
   setupCharts();
   $.getJSON(getURL, function(data) {
      checkData(data);
      console.log("Charts Shown: " + showList);
      updateCharts(data, init);
      toggleDisplays(hideList, showList);
   })
}

// Contstructs the url using the gotten
// date information
function dateParser(day, start, end) {
   var tempURL = GETURL,
       parDay = getDate(day),
       parStart = getTime(start),
       parEnd = getTime(end);

   console.log("day: " + parDay);
   console.log("str time: " + parStart);
   console.log("end time: " + parEnd);
   if (day && parStart && parEnd) {
      tempURL += "&day=" + parDay + "&start=" + parStart + "&end=" + parEnd;
   }

   return tempURL
}

// Gets only the date from the datepicker
function getDate(date) {
   var tempDate = date.slice(0, 10);
   return tempDate;

}

// Gets only the time from the timepicker
// and formats it from 00:11:22 into 00h11m22s
function getTime(time) {
   tempTime = replaceAt(time, 2, "h");
   tempTime = replaceAt(time, 5, "m");
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
function toggleDisplays(showList, hideList) {
   if(showObj(showList, hideList, "temperature")) {
      $("#tempstatus").show();
      $("#temphide").hide();
   } else {
      $("#tempstatus").hide();
      $("#temphide").show();
   }
   if(showObj(showList, hideList, "frequency")) {
      $("#freqstatus").show();
      $("#freqhide").hide();
   } else {
      $("#freqstatus").hide();
      $("#freqhide").show();
   }
}

// Returns whether or not a name is contained
// within the show or hide arrays.
function showObj(hideArr, showArr, name) {
   if(showArr.indexOf(name) > -1) {
      return true;
   }
   return false;
}

// Calls iterateData to get information
// on if object contains data. Based on
// response, if it has data, adds to a list
function checkData(data) {
   var dataObj;
   for (var prop in data) {
      dataObj = data[prop];
      if(iterateData(dataObj)){
         showList.push(prop)
      } else {
         hideList.push(prop);
      }
   }
}

// Recursively iterates through
// an object and data it contains. Iterates
// through the data's "subobjects" until a
// nonzero array is found (returns true) otherwise
// returns false.
function iterateData(data) {
   var dataObj;
   for (var prop in data) {
      dataObj = data[prop];
      if (isNonzeroArray(dataObj)) {
         return true;
      } else if (!(Array.isArray(dataObj))) {
         return iterateData(data);
      }
   }
   return false;
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

function setupCharts () {
   setup_temp();
   setup_freq();
}

function updateCharts (data, initial) {
   freqStatus.series[0].setData(data.frequency, true);
   tempStatus.series[0].setData(data.temperature, true);
}
