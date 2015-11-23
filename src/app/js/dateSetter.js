var GETURL ="api/", // Base link to the date
hideList = [],
showList = [],
init;

function reload_date_range() {
   var getURL = dateParser($( "#day-picker" ).datepicker( "getDate" ).toISOString(),
                           $( "#start-time-picker" ).timepicker('getTime').toISOString(),
                           $( "#end-time-picker" ).timepicker('getTime').toISOString());
   console.log("URL:" + getURL);
   init = false;
   $.getJSON(getURL, function(data) {
      checkData(data);
      console.log("Charts Shown: " + showList);
      updateCharts(data, init);
      toggleDisplays(hideList, showList);
   });
}

function initial_date_range(day, start, end) {
   var getURL = dateParser(day, start, end);
   init = true;
   setupCharts();
   $.getJSON(getURL, function(data) {
      checkData(data);
      console.log("Charts Shown: " + showList);
      updateCharts(data, init);
      toggleDisplays(hideList, showList);
   })
}

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

function getDate(date) {
   var tempDate = date.slice(0, 10);
   return tempDate;

}

function getTime(time) {
   var tempTime = time.slice(11, time.length - 5);
   tempTime = replaceAt(tempTime, 2, "h");
   tempTime = replaceAt(tempTime, 5, "m");
   tempTime += "s";
   return tempTime;
}

function replaceAt (str, ind, charac) {
    return str.substr(0, ind) + charac + str.substr(ind + charac.length);
}

function resetLists(){
   hideList = [];
   showList = [];
}

// Checks if an object name is in either
// the hide or show lists, based on that
// the display is shown or hidden, and the
// message is hidden or shown.
function toggleDisplays(showList, hideList) {
   if(showObj(showList, hideList, "power")) {
      $("#power").show();
      $("#powerHide").hide();
   } else {
      $("#power").hide();
      $("#powerHide").show();
   }
   if(showObj(showList, hideList, "freq")) {
      $("#freq").show();
      $("#freqHide").hide();
   } else {
      $("#freq").hide();
      $("#freqHide").show();
   }
}

// Returns whether or not a name is contained
// within the show or hide arrays.
function showObj(hideArr, showArr, name) {
   if(hideArr.indexOf(name) > -1) {
      return false;
   } else if(showArr.indexOf(name) > -1) {
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
   console.log("SETUP CHART(S) HERE");
   setup_power();
   setup_freq();
}

function updateCharts (data, initial) {
   console.log("UPDATE CHART(S) HERE");
}
