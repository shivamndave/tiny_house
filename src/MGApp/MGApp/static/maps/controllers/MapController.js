var mapController = angular.module('appController'),
  ROOMURL = "api/rooms";
//This is just a controller for our maps.html page
mapController.service('getDataService',function($http){
  return getDataMethod;

  function getDataMethod(){
    var request = $http({
        method: 'GET',
        url: ROOMURL
    }).then(function successCallback(response) {
       console.log('data')
       console.log(response.data)

       return response.data.rooms;
    });

    return request;
  }
});

mapController.controller('MapController', function($scope,getDataService) {
  getDataService().then(function(data){
    // $scope.sens = [];
    // angular.forEach(data, function(room, index) {
    //   angular.forEach(room.sensors, function(sens, index) {
    //     $scope.sens.push(sens)
    //   });
    // });
    $scope.test = data;
    console.log(data);
    $scope.room_colors = ['blue-circle-button btn hvr-grow','green-circle-button btn hvr-grow','purple-circle-button btn hvr-grow','red-circle-button btn hvr-grow','orange-circle-button btn hvr-grow'];
  });
  //$scope.test = room_dat;
});


mapController.directive('dirButton', function(){
  return {
    restrict: 'E',
    scope: {
      display: '@'
    },
    link: link,
    templateUrl: 'static/maps/templates/DirectoryBtn.html'
  }
    function link(scope,element,attrs) {
      //console.log(attrs.display.sensors);
      scope.$watch(attrs.display, function(data){
        scope.room_colors = ['blue-circle-button btn hvr-grow','green-circle-button btn hvr-grow','purple-circle-button btn hvr-grow','red-circle-button btn hvr-grow','orange-circle-button btn hvr-grow'];
        scope.sensors = data.sensors;
        console.log(data.sensors);
        scope.room = data.name;
        scope.type = data.type;
        //scope.room_color = room_colors[data.type]
      });}
});
