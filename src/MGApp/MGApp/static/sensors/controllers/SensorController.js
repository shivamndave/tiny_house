var sensorController = angular.module('appController'),
    specSensorController = angular.module('appController'),
    TOPIC_INTIALPUB = "testbed/gateway/mqtt/0013a20040daebd0",
    TOPIC_INTIALSUB = "testbed/nodeDiscover/data/#",
    SENSURL = "api/sensors";

appController.service('sendNewSetpoint',function($http, $timeout, sendCommand){
  return sendDataMethod;

  function sendDataMethod(value){
    var conf = {headers : {'Content-Type': 'application/x-www-form-urlencoded;charset=utf-8;'}},
        client = sendCommand(onConnect, onMessageArrived);

        function onConnect() {
          console.log("Successful onConnect");
            console.log("Test3");
            var mess4 = ("AP0=").concat(value.data.toString());
            var message4 = new Paho.MQTT.Message(mess4);
            message4.destinationName = TOPIC_INTIALPUB;
            client.send(message4);
        }

      function onMessageArrived(message) {
        console.log("onMessageArrived:" + message.payloadString);
      }

    return $http({
      url: "/api/post/actuator/new_data",
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


specSensorController.controller('SpecSensorController', function($scope, $routeParams,  $timeout, getDataService, sendCommand, sendNewSetpoint) {
  getDataService($routeParams.sensorid, SENSURL).then(function(data){
    console.log(data);
    $scope.tempdata = data.sensor;

    $scope.showLight = function() {
      console.log($scope.tempdata);
      return $scope.tempdata.actuator_info && $scope.tempdata.sensor_info.uid[0] == 'L';
    }

    $scope.showHumid = function() {
      return $scope.tempdata.actuator_info && $scope.tempdata.sensor_info.uid.substring(0,2) == 'DH';

    }

    $scope.showTHumid = function() {
      return $scope.tempdata.actuator_info && $scope.tempdata.sensor_info.uid.substring(0,2) == 'DT';
    }

    $scope.showTempe = function() {
      return $scope.tempdata.actuator_info && $scope.tempdata.sensor_info.uid[0] == 'T';
    }
  });
  $('.dial2').knob(
  {
    'min':10,
    'max':50,
    'width':200,
    'height':200,
    'displayInput':true,
    'release' : function (setpoint_value) {
      var text = 'Are you sure you want change your set point value to '+ setpoint_value;
      noty({
        text: text,
        type: 'warning',
        buttons: [
          {addClass: 'btn btn-primary', text: 'Yes change set point', onClick: function($noty) {
              $scope.sendActuation(setpoint_value);

              $noty.close();
              noty({text: 'you have successfully changed your set point value', type: 'success'});
            }
          },
          {addClass: 'btn btn-danger', text: 'Cancel', onClick: function($noty) {
              $noty.close();
              noty({text: 'Mistake avoided!', type: 'error'});
            }
          }
        ]
      });
    }
  });

  $scope.sendActuation = function(setpoint_value) {
    var postData = {
      "actuator_id": $scope.tempdata.actuator_info.id,
      "data": setpoint_value,
      "uid": $scope.tempdata.sensor_info.uid
    };

    console.log(postData)
    sendNewSetpoint(postData).then(function(value) {
      console.log("Success - Button disabled")
    }, function(reason) {
      console.log("Rejected SAHN")
    });
  }

  $scope.sendAS = function() {
    var client = sendCommand(onConnect, onMessageArrived);
    function onConnect() {
      console.log("Successful onConnect");
        console.log("Test2");
        var mess2 = "AS0=L0";
        var message2 = new Paho.MQTT.Message(mess2);
        message2.destinationName = TOPIC_INTIALPUB;
        client.send(message2);
    }
  };

  $scope.sendAO1 = function() {
    var client = sendCommand(onConnect, onMessageArrived);
    function onConnect() {
      console.log("Successful onConnect");
      console.log("Test3");
      var mess3 = ("AO0=1");
      var message3 = new Paho.MQTT.Message(mess3);
      message3.destinationName = TOPIC_INTIALPUB;
      client.send(message3);
    }
  };

  $scope.sendAO2 = function() {
    var client = sendCommand(onConnect, onMessageArrived);
    function onConnect() {
      console.log("Successful onConnect");
      console.log("Test3");
      var mess3 = ("AO1=1");
      var message3 = new Paho.MQTT.Message(mess3);
      message3.destinationName = TOPIC_INTIALPUB;
      client.send(message3);
    }
  };

  $scope.sendAO3 = function() {
    var client = sendCommand(onConnect, onMessageArrived);
    function onConnect() {
      console.log("Successful onConnect");
      console.log("Test3");
      var mess3 = ("AO2=1");
      var message3 = new Paho.MQTT.Message(mess3);
      message3.destinationName = TOPIC_INTIALPUB;
      client.send(message3);
    }
  };

  $scope.sendAA = function() {
    var client = sendCommand(onConnect, onMessageArrived);
    function onConnect() {
      console.log("Successful onConnect");
        console.log("Test1");
        var message1 = new Paho.MQTT.Message("AA0=1");
        message1.destinationName = TOPIC_INTIALPUB;
        client.send(message1);
      }
  };
$scope.sendOnMessage1 = function() {
  var client = sendCommand(onConnect, onMessageArrived);
  function onConnect() {
     console.log("Test");
     message1 = new Paho.MQTT.Message("AA0=1");
     message1.destinationName = TOPIC_INTIALPUB;
     client.send(message1);
   }
};
$scope.sendOnMessage2 = function() {
  var client = sendCommand(onConnect, onMessageArrived);
  function onConnect() {
     console.log("Test");
     message2 = new Paho.MQTT.Message("AA1=1");
     message2.destinationName = TOPIC_INTIALPUB;
     client.send(message2);
  }
};
$scope.sendOnMessage3 = function() {
  var client = sendCommand(onConnect, onMessageArrived);
  function onConnect() {
     console.log("Test");
     message3 = new Paho.MQTT.Message("AA2=1");
     message3.destinationName = TOPIC_INTIALPUB;
     client.send(message3);
  }
}

$scope.sendOffMessage1 = function() {
  var client = sendCommand(onConnect, onMessageArrived);
  function onConnect() {
     console.log("Test");
     message1 = new Paho.MQTT.Message("AA0=0");
     message1.destinationName = TOPIC_INTIALPUB;
     client.send(message1);
  }
}
$scope.sendOffMessage2 = function() {
  var client = sendCommand(onConnect, onMessageArrived);
  function onConnect() {
     console.log("Test");
     message2 = new Paho.MQTT.Message("AA1=0");
     message2.destinationName = TOPIC_INTIALPUB;
     client.send(message2);
  }
}
$scope.sendOffMessage3 = function() {
  var client = sendCommand(onConnect, onMessageArrived);
  function onConnect() {
     console.log("Test");
     message3 = new Paho.MQTT.Message("AA2=0");
     message3.destinationName = TOPIC_INTIALPUB;
     client.send(message3);
  }
}

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
            scope.sensorinfo_name = data.sensor_info.name;
            scope.sensorinfo_info = data.sensor_info.info;
            scope.equipment_name = data.equipment.name;
            scope.equipment_location = data.equipment.location;
            scope.equipment_info = data.equipment.info;

            scope.latest_time = "Loading latest time...";
            scope.latest_status = "Loading latest status...";

            scope.chart_id = angular.element("#"+String(attrs.ind));
            scope.status = createSensorChart(scope.chart_id, data);

            // console.log(data.values.sensor);
            var sensorInterval = $interval(function() {
                getDataService(data.sensor_info.id, SENSORSURL).then(function(tempData) {
                    var currentData = tempData.sensor;

                    scope.latest_time = "Latest time: None";
                    if(currentData.values.sensor.length > 0) {
                      if(currentData.actuator_info && currentData.values.sensor.length > 0 && currentData.values.actuator.length > 0) {
                          setpointValue = currentData.values.actuator[currentData.values.actuator.length - 1][1];
                          console.log(setpointValue);
                          var chart_yAxis = scope.status.highcharts().yAxis;
                          console.log(chart_yAxis);
                          if(chart_yAxis.length == 0) {
                            chart_yAxis[0].addPlotLine({
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
                            chart_yAxis[0].removePlotLine('plot-line-1');
                            chart_yAxis[0].addPlotLine({
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
                      }
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
