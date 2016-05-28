var mapController = angular.module('appController'),
  ROOMURL = "api/rooms";

mapController.controller('MapController', function($scope, getDataService) {
  getDataService(null, ROOMURL).then(function(data){
    $scope.test = data.rooms;
    $scope.room_colors = ['blue-circle-button btn hvr-grow','green-circle-button btn hvr-grow','purple-circle-button btn hvr-grow','red-circle-button btn hvr-grow','orange-circle-button btn hvr-grow'];
    console.log(data.rooms.length);
    var sys = arbor.ParticleSystem(1000, 400,1);
    sys.parameters({gravity:true});
    sys.renderer = Renderer("#viewport") ;
    // for(var i=0;i<data.rooms.length ;i++) {
    //   var data = {
    //     nodes:{
    //       animals:{'color':'red','shape':'dot','label':'Animals'},
    //       dog:{'color':'green','shape':'dot','label':'dog'},
    //       cat:{'color':'blue','shape':'dot','label':'cat'}
    //     },
    //     edges:{
    //       animals:{ dog:{}, cat:{} }}
    //     };
    //
    // }

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
        // var sys = arbor.ParticleSystem(1000, 400,1);
        // sys.parameters({gravity:true});
        // sys.renderer = Renderer("#viewport") ;
        // var a = data.name;
        // console.log(a);
        // var data = {
        //   nodes:{
        //     animals:{'color':'red','shape':'dot','label':data.name },
        //     dog:{'color':'green','shape':'dot','label':'dog'},
        //     cat:{'color':'blue','shape':'dot','label':'cat'}
        //   },
        //   edges:{
        //     animals:{ dog:{}, cat:{} }
        //   }
        // };
        // sys.graft(data);
      });}

});
