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

  // Changes based on radio button selection
  $scope.newSettingView = function(value) {
    switch(value) {
      case "sensors":
        console.log("We chose: "+ value);
        getDataService(null, SENSORSURL).then(function(data) {
          $scope.data_value = data.all;
        });
        break;
      case "rooms":
        console.log("We chose: "+ value);
        getDataService(null, ROOMSURL).then(function(data) {
          $scope.data_value = data.rooms;
        });
        break;
      case "actuators":
        console.log("We chose: "+ value);
        getDataService(null, ACTUATORSURL).then(function(data) {
          $scope.data_value = data.all;
        });
        break;
      case "equipment":
        console.log("We chose: "+ value);
        getDataService(null, EQUIPMENTURL).then(function(data) {
          $scope.data_value = data.equipment;
        });
        break;
    }
    console.log($scope.data_value);
  }
});
