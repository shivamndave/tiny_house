var onboardController = angular.module('appController'),
    CLIENTNAME = "169.233.220.41",
    CLIENTPORT = 9001,
    TEST_CLIENTNAME = "test.mosquitto.org",
    TEST_CLIENTPORT = 8080,
    TEST_MSG = "DH=2 T=1 DT=0",
    TEST_MAC = "00:0a:95:9d:68:16"
    CLIENTID   = "id",
    TOPIC_INTIALPUB = "testbed/nodeDiscover/command/",
    TOPIC_INTIALSUB = "testbed/nodeDiscover/data/#",
    ROOMURL = "api/rooms";


onboardController.controller('OnboardController', function($scope, getDataService) {
  $scope.sensor_data_array = [];
  var initialClient = new Paho.MQTT.Client(TEST_CLIENTNAME, TEST_CLIENTPORT, CLIENTID);
  // var initialClient = new Paho.MQTT.Client(CLIENTNAME, CLIENTPORT, CLIENTID);
  $scope.show_form = false;
  $scope.cancel_button = false;
  $scope.onConnect_button = true;
  $scope.sensordata = null;

  console.log("Now trying to connect...");
  initialClient.connect({onSuccess:onConnect});

  initialClient.onConnectionLost = onConnectionLost;
  initialClient.onMessageArrived = onMessageArrived;

  getDataService(null, ROOMURL).then(function(roomData){
    console.log(roomData.rooms);
    $scope.rms = roomData.rooms;
  });

  $scope.sendOnboardMessage = function() {
    message = new Paho.MQTT.Message("START");
    message.destinationName = TOPIC_INTIALPUB;
    initialClient.send(message);
    initialClient.subscribe(TOPIC_INTIALSUB);
    console.log("subscribed");
    $scope.cancel_button = true;
  }

  function onConnect() {
    console.log("Successful onConnect");
  }

  function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
      console.log("onConnectionLost:" + responseObject.errorMessage);
    }
  }

  function onMessageArrived(message) {
    console.log("onMessageArrived:" + message.payloadString);
    var parseDest = message.destinationName.split("/"),
        mac_address = parseDest[parseDest.length - 1];

    $scope.show_form = true;
    // var parsedSensor = parseSensorAdded(message.payloadString, mac_address);
    var parsedSensor = parseSensorAdded(TEST_MSG, TEST_MAC),
        sensors_array = formPush(parsedSensor);

    console.log(sensors_array);
    $scope.sensor_data_array = sensors_array;
    $scope.$apply();
  }

  function formPush(parsedSensor) {
    var sensors = [];
    for(var i = 0; i < parsedSensor.light_sensors; i++) {
      var iStr = i.toString();
      sensors.push({"name": "Light Sensor " + iStr, "uid": "l" + iStr, "mac_address": parsedSensor.mac_address, "unit": "Lx", "longunit": "Lux"});
    }
    for(var i = 0; i < parsedSensor.humidity_sensors; i++) {
      var iStr = i.toString();
      sensors.push({"name": "Humidity Sensor " + iStr, "uid": "h" + iStr, "mac_address": parsedSensor.mac_address, "unit": "RH", "longunit": "Rel. Humidity"});
    }
    for(var i = 0; i < parsedSensor.temperature_sensors; i++) {
      var iStr = i.toString();
      sensors.push({"name": "Temperature Sensor " + iStr, "uid": "t" + iStr, "mac_address": parsedSensor.mac_address, "unit": "C", "longunit": "Celsius"});
    }
    for(var i = 0; i < parsedSensor.humid_temp_sensors; i++) {
      var iStr = i.toString();
      sensors.push({"name": "Humidity Temp Sensor " + iStr, "uid": "u" + iStr, "mac_address": parsedSensor.mac_address, "unit": "RH", "longunit": "Rel. Humidity"});
    }

    return sensors

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
        case 'D': sensorInfoDict["humidity_sensors"] = parseInt(tempVal[1]); break;
        case 'T': sensorInfoDict["temperature_sensors"] = parseInt(tempVal[1]); break;
        case 'DT': sensorInfoDict["humid_temp_sensors"] = parseInt(tempVal[1]); break;
      }
    });

    console.log(sensorInfoDict);
    return sensorInfoDict;
  }
});

appController.service('sendNewSensor',function($http){
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

onboardController.directive('onboardForm', function($http, sendNewSensor){
  return {
    restrict: 'E',
    scope: {
      sensordata: '@',
      rooms: '@',
      onSubmit: '&',
      ind: '@'
    },
    link: link,
    templateUrl: 'static/onboard/templates/onboard_form.html'
  }
    function link(scope,element,attrs) {
      scope.submitButtonTog = false;
      scope.$watch(attrs.sensordata, function(data) {
        console.log(data);
          scope.fieldmac = data.mac_address;
          scope.fieldsname = data.name;
          scope.fielduid = data.uid;
          scope.fieldunit = data.unit;
          scope.fieldlongunit = data.longunit;
      });

      scope.$watch(attrs.rooms, function(roomData) {
        console.log(roomData);
        scope.room_arr = roomData;
      });

      scope.add = function(fieldsname, fieldunit, fieldlongunit, fieldsinfo, fieldmac, fielduid, selectedRoom) { // <-- here is you value from the input
        var fieldename = "Equipment " + fielduid.toString(),
            fieldeinfo = "No equipment info given",
            fieldlocation = "No location given";
        if(!fieldsinfo){fieldsinfo = "No sensor info given"}
        // console.log(fieldsname + " " + fieldunit  + " " +  fieldlongunit   + " " +  fieldmac  + " " +  fielduid + " " + fieldename  + " " +  fieldsinfo + " " + fieldeinfo + " " +  fieldlocation + " | " + selectedRoom.name);

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
          "room_id": selectedRoom.id.toString()
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
