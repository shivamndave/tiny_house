var mapController = angular.module('appController'),
  ROOMURL = "api/rooms";

mapController.controller('MapController', function($scope,getDataService) {
  getDataService(null, ROOMURL).then(function(data){
    $scope.test = data.rooms;
    $scope.room_colors = ['blue-circle-button btn hvr-grow','green-circle-button btn hvr-grow','purple-circle-button btn hvr-grow','red-circle-button btn hvr-grow','orange-circle-button btn hvr-grow'];
  });
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
