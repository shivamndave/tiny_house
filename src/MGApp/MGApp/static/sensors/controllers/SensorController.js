var sensorController = angular.module('appController'),
    specSensorController = angular.module('appController'),
    TOPIC_INTIALPUB = "testbed/gateway/mqtt/0013a20040daebd0",
    TOPIC_INTIALSUB = "testbed/nodeDiscover/data/#",
    SENSURL = "api/sensors";

specSensorController.controller('SpecSensorController', function($scope, $routeParams ,getDataService, sendCommand) {
  getDataService($routeParams.sensorid, SENSURL).then(function(data){
    console.log(data);
    $scope.tempdata = data.sensor;
  });

  $scope.sendOnMessage = function() {
    var client = sendCommand(onConnect, onMessageArrived);
    function onConnect() {
      console.log("Successful onConnect");
      message1 = new Paho.MQTT.Message("\x41\x41\x30\x3D\x31\x0A");
      message2 = new Paho.MQTT.Message("\x41\x41\x31\x3D\x31\x0A");
      message3 = new Paho.MQTT.Message("\x41\x41\x32\x3D\x31\x0A");
      message1.destinationName = TOPIC_INTIALPUB;
      message2.destinationName = TOPIC_INTIALPUB;
      message3.destinationName = TOPIC_INTIALPUB;
      setTimeout(function() {
        client.send(message1);
      }, 1000);
      setTimeout(function() {
        client.send(message2);
      }, 1000);
      setTimeout(function() {
        client.send(message3);
      }, 1000);
    }
  };

  $scope.sendOffMessage = function() {
    var client = sendCommand(onConnect, onMessageArrived);
    function onConnect() {
      console.log("Successful onConnect");
      message1 = new Paho.MQTT.Message("\x41\x41\x30\x3D\x30\x0A");
      message2 = new Paho.MQTT.Message("\x41\x41\x31\x3D\x30\x0A");
      message3 = new Paho.MQTT.Message("\x41\x41\x32\x3D\x30\x0A");
      message1.destinationName = TOPIC_INTIALPUB;
      message2.destinationName = TOPIC_INTIALPUB;
      message3.destinationName = TOPIC_INTIALPUB;
      setTimeout(function() {
        client.send(message1);
      }, 1000);
      setTimeout(function() {
        client.send(message2);
      }, 1000);
      setTimeout(function() {
        client.send(message3);
      }, 1000);
    }
  };

  function onMessageArrived(message) {
    console.log("onMessageArrived:" + message.payloadString);
  }
});

sensorController.controller('SensorController', function($scope,getDataService) {
  getDataService(null, SENSURL).then(function(data){
    // console.log(data.all);
    $scope.SenInfo = data.all;
  });
});

specSensorController.directive('kgDisplay', function($interval, getDataService, createSensorChart) {
    return {
        restrict: 'E',
        scope: {
            display: '&',
            ind: '@',
            status: '@'
        },
        link: link,
        templateUrl: 'static/home/templates/display.html'
    }

    function link(scope, element, attrs) {
        scope.$watch(scope.display, function(data) {
            if (data){
            // console.log(scope);
            scope.sensorinfo_name = data.sensor_info.name;
            scope.sensorinfo_info = data.sensor_info.info;
            scope.equipment_name = data.equipment.name;
            scope.equipment_location = data.equipment.location;
            scope.equipment_info = data.equipment.info;

            scope.latest_time = "Loading latest time...";
            scope.latest_status = "Loading latest status...";

            console.log("attrs.ind")
            console.log(attrs.ind)
            scope.chart_id = angular.element("#"+String(attrs.ind));
            scope.status = scope.chart_id.highcharts('StockChart', {
                plotOptions: {
                    line: {
                        marker: {
                            enabled: false
                        }
                    }
                },
                chart: {
                    height: 300
                },
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
                    }
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
            console.log(data.values.sensor);
            var sensorInterval = $interval(function() {
                getDataService(data.sensor_info.id, SENSORSURL).then(function(tempData) {
                    console.log(tempData)
                    var currentData = tempData.sensor;
                    console.logcurrentData
                    console.log(scope.chart_id.highcharts().series[0]);
                    scope.latest_time = "Latest time: None";
                    if(currentData.values.sensor.length > 0) {
                        scope.chart_id.highcharts().series[0].setData(currentData.values.sensor, true);

                        var latestTimeVal = currentData.values.sensor[currentData.values.sensor.length - 1][0],
                        latestTimeConv = new Date(latestTimeVal);
                        scope.latest_time = "Latest time: " + String(latestTimeConv);
                    }

                    var latestStatVal = currentData.sensor_info.status,
                    statusText = "";

                    if(latestStatVal == 0) {
                        statusText = "OFF";
                    } else {
                        statusText = "ON";
                    }
                    scope.latest_status = "Latest status: " + statusText;
                });
            }, 5000);

            scope.$on("$destroy", function(){
                console.log("destroy");
                $interval.cancel(sensorInterval);
                element.remove();
            });
        }});
    }
});
