var onboardController = angular.module('appController'),
    TEST_MSG = "DH=2 T=1 DT=0",
    TEST_MAC = "000a959d6816"
    CLIENTID   = "id",
    NODEDISCOVER_COMMAND = "testbed/nodeDiscover/command/",
    NODEDISCOVER_DATA = "testbed/nodeDiscover/data/#";


onboardController.controller('OnboardController', function($scope, sendCommand) {
  $scope.sensor_data_array = [];
  $scope.show_form = false;
  $scope.cancel_button = false;
  $scope.onConnect_button = true;
  $scope.sensordata = null;

  var client = sendCommand(onConnect, onMessageArrived);

  $scope.sendOnboardMessage = function() {
    message = new Paho.MQTT.Message("START");
    message.destinationName = NODEDISCOVER_COMMAND;
    client.send(message);
    client.subscribe(NODEDISCOVER_DATA);
    console.log("subscribed");
    $scope.cancel_button = true;
  };

  function onConnect() {
    console.log("Successful onConnect");
  }

  function onMessageArrived(message) {
    console.log("onMessageArrived:" + message.payloadString);
    var parseDest = message.destinationName.split("/"),
        mac_address = parseDest[parseDest.length - 1];

    $scope.show_form = true;
    var parsedSensor = parseSensorAdded(message.payloadString, mac_address);
    // var parsedSensor = parseSensorAdded(TEST_MSG, TEST_MAC),
    formPush(parsedSensor);
    $scope.$apply();
  }

  function formPush(parsedSensor) {
    // var sensors = [];
    for(var i = 0; i < parsedSensor.light_sensors; i++) {
      var iStr = i.toString();
      $scope.sensor_data_array.push({"name": "Light Sensor " + iStr, "uid": "L" + iStr, "mac_address": parsedSensor.mac_address, "unit": "Lx", "longunit": "Lux"});
    }
    for(var i = 0; i < parsedSensor.humidity_sensors; i++) {
      var iStr = i.toString();
      $scope.sensor_data_array.push({"name": "Humidity Sensor " + iStr, "uid": "DH" + iStr, "mac_address": parsedSensor.mac_address, "unit": "RH", "longunit": "Rel. Humidity"});
    }
    for(var i = 0; i < parsedSensor.temperature_sensors; i++) {
      var iStr = i.toString();
      $scope.sensor_data_array.push({"name": "Temperature Sensor " + iStr, "uid": "T" + iStr, "mac_address": parsedSensor.mac_address, "unit": "C", "longunit": "Celsius"});
    }
    for(var i = 0; i < parsedSensor.humid_temp_sensors; i++) {
      var iStr = i.toString();
      $scope.sensor_data_array.push({"name": "Humidity Temp Sensor " + iStr, "uid": "DT" + iStr, "mac_address": parsedSensor.mac_address, "unit": "RH", "longunit": "Rel. Humidity"});
    }

    // return sensors

  }

  function parseSensorAdded(sensorStr, mac_address) {
    var senssplit = sensorStr.split(" "),
        sensorInfoDict = {},
        light_sensors, temp_sensors, hygro_sensors;

    sensorInfoDict['mac_address'] = mac_address;

    senssplit.forEach(function(val) {
      var tempVal = val.split("=")


      console.log(tempVal[0]);
      switch(tempVal[0]) {
        case 'L': sensorInfoDict["light_sensors"] = parseInt(tempVal[1]); break;
        case 'DH': sensorInfoDict["humidity_sensors"] = parseInt(tempVal[1]); break;
        case 'D': sensorInfoDict["humidity_sensors"] = parseInt(tempVal[1]); sensorInfoDict["humid_temp_sensors"] = parseInt(tempVal[1]);  break;
        case 'T': sensorInfoDict["temperature_sensors"] = parseInt(tempVal[1]); break;
        case 'DT': sensorInfoDict["humid_temp_sensors"] = parseInt(tempVal[1]); break;
      }
    });

    console.log(sensorInfoDict);
    return sensorInfoDict;
  }
});

onboardController.directive('onboardForm', function($http, sendNewSensor){
  return {
    restrict: 'E',
    scope: {
      sensordata: '@',
      onSubmit: '&'
    },
    link: link,
    templateUrl: 'static/onboard/templates/onboard_form.html'
  }
    function link(scope, element, attrs) {
      scope.submitButtonTog = false;
      scope.$watch(attrs.sensordata, function(data) {
        console.log(data);
          scope.fieldmac = data.mac_address;
          scope.fieldsname = data.name;
          scope.fielduid = data.uid;
          scope.fieldunit = data.unit;
          scope.fieldlongunit = data.longunit;
          scope.fieldactua = false;
      });

      scope.$watch(attrs.rooms, function(roomData) {
        console.log(roomData);
        scope.room_arr = roomData;
      });
      scope.newValue = function(val) {
        console.log(val);
      }
      scope.add = function(fieldsname, fieldunit, fieldlongunit, fieldsinfo, fieldmac, fielduid, fieldactua) { // <-- here is you value from the input
        var fieldename = "Equipment " + fielduid.toString(),
            fieldeinfo = "No equipment info given",
            fieldlocation = "No location given",
            actautor_name = null,
            actautor_info = null;
        if(!fieldsinfo){fieldsinfo = "No sensor info given"}
        // console.log(fieldsname + " " + fieldunit  + " " +  fieldlongunit   + " " +  fieldmac  + " " +  fielduid + " " + fieldename  + " " +  fieldsinfo + " " + fieldeinfo + " " +  fieldlocation + " | " + selectedRoom.name);
        if(fieldactua){
          actautor_name = fieldsname + " actuator";
          actautor_info = "Actuator for " + fieldsname;
        }
        var postData = {
          "sensor_name": fieldsname,
          "longunit": fieldlongunit,
          "unit": fieldunit,
          "mac_address": fieldmac,
          "uid": fielduid,
          "sensor_info": fieldsinfo,
          "equipment_name": fieldename,
          "equipment_info": fieldeinfo,
          "location": fieldlocation,
          "room_id": "1",
          "is_actuator": fieldactua,
          "actuator_name": actautor_name,
          "actuator_info": actautor_info
        };

        console.log(postData)
        sendNewSensor(postData).then(function(value) {
          console.log("Success - Button disabled")
          scope.submitButtonTog = true;
        }, function(reason) {
          console.log("Rejected SAHN")
        });
      };
    }
});
