var sensorController = angular.module('appController'),
  SENSURL = "api/sensors/47";
//This is just a controller for our maps.html page
sensorController.service('getDataService',function($http){
  return getDataMethod;

  function getDataMethod(){
    var request = $http({
        method: 'GET',
        url: SENSURL
    }).then(function successCallback(response) {
       //console.log('data')
       //console.log(response.data)

       return response.data.rooms;
    });

    return request;
  }
});

sensorController.controller('SensorController', function($scope,getDataService) {
  getDataService().then(function(data){
  console.log(data);
});
});


sensorController.directive('sensPage', function(){
  return {
    restrict: 'E',
    scope: {
      display: '@',
    },
    link: link,
    templateUrl: 'static/sensors/templates/sensor.html'
  }
    function link(scope,element,attrs) {
      //console.log(attrs.display.sensors);
      scope.$watch(attrs.display, function(data){
        scope.id = data.sensor_info.id;
        scope.info = data.sensor_info.info;
        scope.longunit = data.sensor_info.longunit;
        scope.name = data.sensor_info.name;
        scope.status = data.sensor_info.status;
        scope.unit = data.sensor_info.unit;
        //scope.room_color = room_colors[data.type]
      });}
});
