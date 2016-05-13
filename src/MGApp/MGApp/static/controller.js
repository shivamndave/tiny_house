var appController = angular.module('appController', []);
<<<<<<< HEAD
=======

appController.service('urlParser', function() {
      this.getLive = function(id, url) {
          tempURL = url;
          if(id != null) {
            console.log(id.toString());
            tempURL += "/" + id.toString();
          }
          return tempURL;
      }

      this.getHist = function(id, latest) {
          var tempURL = GETURL,
              parDay = day.slice(0, 10),
              parStart = getTime(start),
              parEnd = getTime(end);

          console.log("day: " + parDay + " | str time: " + parStart + " | end time: " + parEnd);
          if (day && parStart && parEnd) {
              tempURL += "&day=" + parDay + "&start=" + parStart + "&end=" + parEnd;
              console.log("API call URL: " + tempURL);
          }

          return tempURL
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
      function replaceAt(str, ind, charac) {
          return str.substr(0, ind) + charac + str.substr(ind + charac.length);
      }
  });

appController.service('getDataService',function($http, urlParser){
  return getDataMethod;

  function getDataMethod(id, url){
    var url_value = urlParser.getLive(id, url),
        request = $http({
        method: 'GET',
        url: url_value
    }).then(function successCallback(response) {
       console.log('data')
       console.log(response.data)

       return response.data;
    });

    return request;
  }
});
>>>>>>> a7d8c55ccc192596468c502cd9d56da65a0486bf
