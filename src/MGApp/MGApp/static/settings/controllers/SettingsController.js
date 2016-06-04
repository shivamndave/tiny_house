var settingsController = angular.module('appController'),
SENSORSURL = "api/sensors",
ROOMSURL = "api/rooms",
ACTUATORSURL = "api/actuators",
EQUIPMENTURL = "api/equipment";

settingsController.controller('SettingsController', function($scope, getDataService) {
  // By default chooses rooms
  getDataService(null, ROOMSURL).then(function(data) {
    $scope.value = "rooms";
    $scope.data_value = data.rooms;
  });

  $scope.idGet = function(value, obj) {
    console.log(obj);
      switch(value) {
        case "sensors":
          if (obj.sensor_info) {
            return obj.sensor_info;
          }
        case "rooms":
          return obj;
        case "actuators":
          if (obj.actuator_info) {
            return obj.actuator_info;
          }
        case "equipment":
          return obj;
      }
  }
  // Changes based on radio button selection
  $scope.newSettingView = function(value) {
    switch(value) {
      case "sensors":
        console.log("We chose: "+ value);
        getDataService(null, SENSORSURL).then(function(sensorData) {
          $scope.data_value = sensorData.all;
        });
        break;
      case "rooms":
        console.log("We chose: "+ value);
        getDataService(null, ROOMSURL).then(function(roomData) {
          $scope.data_value = roomData.rooms;
        });
        break;
      case "actuators":
        console.log("We chose: "+ value);
        getDataService(null, ACTUATORSURL).then(function(actuatorData) {
          $scope.data_value = actuatorData.all;
        });
        break;
      case "equipment":
        console.log("We chose: "+ value);
        getDataService(null, EQUIPMENTURL).then(function(equipmentData) {
          $scope.data_value = equipmentData.equipment;
        });
        break;
    }
    console.log($scope.data_value);
  }
});
