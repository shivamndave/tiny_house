var settingsRoomsController = angular.module('appController'),
    settingsEquipmentController = angular.module('appController'),
    settingsSensorsController = angular.module('appController'),
    settingsActuatorsController = angular.module('appController'),
    SENSORSURL = "api/sensors",
    ROOMSURL = "api/rooms",
    ACTUATORSURL = "api/actuators",
    EQUIPMENTURL = "api/equipment";

settingsRoomsController.controller('SettingsRoomsController', function($scope, $routeParams, getDataService) {
    getDataService($routeParams.id, ROOMSURL).then(function(data) {
        currentData = data.room;
        console.log(currentData);
        $scope.fieldrid = currentData.id;
        $scope.fieldrname = currentData.name;
        $scope.fieldrtype = currentData.type;
        $scope.fieldrinfo = currentData.info;
    });
});

settingsEquipmentController.controller('SettingsEquipmentController', function($scope, $routeParams, getDataService) {
    getDataService($routeParams.id, EQUIPMENTURL).then(function(data) {
        currentData = data.equipment;
        console.log(currentData);
        $scope.fieldeid = currentData.id;
        $scope.fieldename = currentData.name;
        $scope.fieldelocation = currentData.location;
        $scope.fieldeinfo = currentData.info;
        $scope.fieldmacaddress = currentData.mac_address;
        $scope.fieldroomselected = currentData.room_id;
    }).then(getDataService(null, ROOMSURL).then(function(roomdata) {
        $scope.fieldrooms = roomdata.rooms;
    }));

    $scope.equipment_edit = function(fieldeid, fieldename, fieldeinfo, fieldelocation, fieldmacaddress, fieldroomselected) {
        console.log(fieldeid + ", " + fieldename + ", " + fieldeinfo + ", " + fieldelocation + ", " + fieldmacaddress + ", " + fieldroomselected)
    }
});

settingsSensorsController.controller('SettingsSensorsController', function($scope, $routeParams, getDataService) {
    getDataService($routeParams.id, SENSORSURL).then(function(data) {
        currentData = data.sensor.sensor_info;
        console.log(currentData);
        $scope.fieldsid = currentData.id;
        $scope.fieldsuid = currentData.uid;
        $scope.fieldsname = currentData.name;
        $scope.fieldsinfo = currentData.info;
        $scope.fieldsunit = currentData.unit;
        $scope.fieldslongunit = currentData.longunit;
        $scope.fieldequipmentselected = currentData.equipment_id;
    }).then(getDataService(null, EQUIPMENTURL).then(function(equipmentdata) {
        $scope.fieldequipment = equipmentdata.equipment;
    }));

    $scope.sensor_edit = function(fieldsid, fieldsuid, fieldsname, fieldsinfo, fieldsunit, fieldslongunit, fieldequipmentselected) {
        console.log(fieldsid + ", " +fieldsuid + ", " +fieldsname + ", " +fieldsinfo + ", " +fieldsunit + ", " +fieldslongunit + ", " +fieldequipmentselected)
    }
});

settingsActuatorsController.controller('SettingsActuatorsController', function($scope, $routeParams, getDataService) {
    getDataService($routeParams.id, ACTUATORSURL).then(function(data) {
        currentData = data.actuator.actuator_info;
        console.log(currentData);
        $scope.fieldaid = currentData.id;
        $scope.fieldaname = currentData.name;
        $scope.fieldainfo = currentData.info;
        $scope.fieldsensorselected = currentData.sensor_id;
        $scope.fieldequipmentselected = currentData.equipment_id;
    }).then(getDataService(null, SENSORSURL).then(function(sensordata) {
        $scope.fieldsensors = sensordata.all;
    })).then(getDataService(null, EQUIPMENTURL).then(function(equipmentdata) {
        $scope.fieldequipment = equipmentdata.equipment;
    }));

    $scope.actuator_edit = function(fieldaid, fieldaname, fieldainfo, fieldequipmentselected, fieldsensorselected) {
        console.log(fieldaid + ", " + fieldaname + ", " +  fieldainfo + ", " + fieldequipmentselected + ", " + fieldsensorselected)
    }
});
