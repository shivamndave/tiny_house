var settingsRoomsController = angular.module('appController'),
    settingsEquipmentController = angular.module('appController'),
    settingsSensorsController = angular.module('appController'),
    settingsActuatorsController = angular.module('appController'),
    SENSORSURL = "api/sensors",
    ROOMSURL = "api/rooms",
    ACTUATORSURL = "api/actuators",
    EQUIPMENTURL = "api/equipment";

settingsRoomsController.controller('SettingsRoomsController', function($scope, $routeParams, getDataService, postToDB) {
    getDataService($routeParams.id, ROOMSURL).then(function(data) {
        currentData = data.room;
        console.log(currentData);
        $scope.fieldrid = currentData.id;
        $scope.fieldrname = currentData.name;
        $scope.fieldrtype = currentData.type;
        $scope.fieldrinfo = currentData.info;
    });

    $scope.room_edit = function(fieldrid, fieldrname, fieldrtype, fieldrinfo) {
        console.log(fieldrid + ", " + fieldrname + ", " + fieldrtype + ", " + fieldrinfo)
        var postData = {
          "name": fieldrname,
          "type": fieldrtype,
          "info": fieldrinfo,
          "id": parseInt(fieldrid),
        };
        postToDB("/api/rooms/update", postData).then(function(value) {
          console.log("Success - Button disabled")
        }, function(reason) {
          console.log("Rejected SAHN")
        });
    }
});

settingsEquipmentController.controller('SettingsEquipmentController', function($scope, $routeParams, getDataService, postToDB) {
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
        var postData = {
            "room_id": fieldroomselected,
            "name": fieldename,
            "location": fieldelocation,
            "info": fieldeinfo,
            "id": parseInt(fieldeid),
        };
        postToDB("/api/equipment/update", postData).then(function(value) {
          console.log("Success - Button disabled")
        }, function(reason) {
          console.log("Rejected SAHN")
        });
    }
});

settingsSensorsController.controller('SettingsSensorsController', function($scope, $routeParams, getDataService, postToDB) {
    getDataService($routeParams.id, SENSORSURL).then(function(data) {
        currentData = data.sensor.sensor_info;
        console.log(currentData);
        $scope.fieldsid = currentData.id;
        $scope.fieldsuid = currentData.uid;
        $scope.fieldsname = currentData.name;
        $scope.fieldsinfo = currentData.info;
        $scope.fieldsunit = currentData.unit;
        $scope.fieldslongunit = currentData.longunit;
        $scope.fieldequipment = data.sensor.equipment.name;
    })

    $scope.sensor_edit = function(fieldsid, fieldsuid, fieldsname, fieldsinfo, fieldsunit, fieldslongunit, fieldequipment) {
        console.log(fieldsid + ", " +fieldsuid + ", " +fieldsname + ", " +fieldsinfo + ", " +fieldsunit + ", " +fieldslongunit + ", " +fieldequipment)
        var postData = {
            "name": fieldsname,
            "unit": fieldsunit,
            "longunit": fieldslongunit,
            "info": fieldsinfo,
            "id": parseInt(fieldsid),
        };
        postToDB("/api/sensors/update", postData).then(function(value) {
          console.log("Success - Button disabled")
        }, function(reason) {
          console.log("Rejected SAHN")
        });
    }
});

settingsActuatorsController.controller('SettingsActuatorsController', function($scope, $routeParams, getDataService, postToDB) {
    getDataService($routeParams.id, ACTUATORSURL).then(function(data) {
        currentData = data.actuator.actuator_info;
        console.log(currentData);
        $scope.fieldaid = currentData.id;
        $scope.fieldaname = currentData.name;
        $scope.fieldainfo = currentData.info;
        $scope.fieldsensor = data.actuator.sensor_info.name;
        $scope.fieldequipment = data.actuator.equipment.name;
    })

    $scope.actuator_edit = function(fieldaid, fieldaname, fieldainfo, fieldequipment, fieldsensor) {
        console.log(fieldaid + ", " + fieldaname + ", " +  fieldainfo + ", " + fieldequipment + ", " + fieldsensor)

        var postData = {
            "name": fieldaname,
            "info": fieldainfo,
            "id": parseInt(fieldaid),
        };

        postToDB("/api/actuators/update", postData).then(function(value) {
          console.log("Success - Button disabled")
        }, function(reason) {
          console.log("Rejected SAHN")
        });
    }
});
