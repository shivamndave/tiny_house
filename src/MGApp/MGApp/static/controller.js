var appController = angular.module('appController', []),
CLIENTID = "id"
CLIENTNAME = "128.114.63.86",
CLIENTPORT = 9001,
TEST_CLIENTNAME = "test.mosquitto.org",
TEST_CLIENTPORT = 8080;

appController.service('sendCommand', function(){
  return sendCommand;

  function sendCommand(onConnect, onMessageArrived){
    // var client = new Paho.MQTT.Client(TEST_CLIENTNAME, TEST_CLIENTPORT, CLIENTID);
    var client = new Paho.MQTT.Client(CLIENTNAME, CLIENTPORT, CLIENTID);

    console.log("Now trying to connect...");
    client.connect({onSuccess:onConnect});

    client.onConnectionLost = onConnectionLost;
    client.onMessageArrived = onMessageArrived;

    function onConnectionLost(responseObject) {
      if (responseObject.errorCode !== 0) {
        console.log("onConnectionLost:" + responseObject.errorMessage);
      }
    }

    return client;
  }
});

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

  appController.service('sendNewSensor', function($http){
    return sendDataMethod;

    function sendDataMethod(value){
      var conf = {headers : {'Content-Type': 'application/x-www-form-urlencoded;charset=utf-8;'}};
      return $http({
        url: "/api/post/new_sensor",
        method: "POST",
        data: JSON.stringify(value),
        config: conf
      }).success(function(data) {
        console.log("SUCCESS POST");
        console.log(data);
        return true;
      }).error(function(data, status) {
        console.log("FAILED POST");
        console.log(data);
        return false;
      });
    }
  });

appController.service('createSensorChart', function(){
  return createSensorChartMethod;

  function createSensorChartMethod(chart_id, data) {
    var tempPlotlines = [];
    if(data.actuator_info && data.values.actuator.length > 0) {
      var setpointValue = data.values.actuator[data.values.actuator.length - 1][1];
      tempPlotlines = [{
        value: setpointValue,
        color: 'red',
        width: 2,
        id: 'plot-line-1',
        color: 'red',
        dashStyle: 'shortdash',
        label: {
          text: 'Setpoint'
        }
      }]
    }

    return chart_id.highcharts('StockChart', {
      plotOptions: {
        line: {
          marker: {
            enabled: false
          }
        }
      },
      chart: {height: 300},
      rangeSelector: {
          enabled: true,
          selected: 4,
          buttons: [{
              type: 'minute',
              count: 1,
              text: '1m'
          }, {
              type: 'hour',
              count: 3,
              text: '1h'
          }, {
              type: 'day',
              count: 1,
              text: '1d'
          }, {
              type: 'all',
              text: 'All'
          }]
        },
        title: {
            text: data.sensor_info.name
        },
        subtitle: {
            text: data.equipment.name
        },
        xAxis: {
            type: 'datetime'
        },
        yAxis: {
            title: {
                text: data.sensor_info.longunit
            },
            plotLines: tempPlotlines
        },
        series: [{
            name: data.sensor_info.longunit,
            data: data.values.sensor,
            tooltip: {
                valueDecimals: 2,
                valueSuffix: " " + data.sensor_info.unit,
            }
        }]
    });
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
