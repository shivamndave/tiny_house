var app = angular.module("app", ['ngRoute', 'appController']);
app.config(['$routeProvider', function($routeProvider) {
   $routeProvider.

   when('/home', {
      templateUrl: 'static/home/templates/home.html',
      controller: 'HomeController'
   }).
   // when('/commands', {
   //    templateUrl: 'static/commands/templates/commands_page.html',
   //    controller: 'CommandsController'
   // }).
   when('/maps', {
      templateUrl: 'static/maps/templates/Maps.html',
      controller: 'MapController'
   }).
   when('/sensors', {
     templateUrl: 'static/sensors/templates/sensPage.html',
     controller: 'SensorController'
   }).
   when('/sensors/:sensorid', {
      templateUrl: 'static/sensors/templates/sensor.html',
      controller: 'SpecSensorController'
   }).
   otherwise({
      redirectTo: '/home'
   });
}]);
