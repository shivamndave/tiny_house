var app = angular.module("app", ['ngRoute', 'appController']);
app.config(['$routeProvider', function($routeProvider) {
   $routeProvider.

   when('/home', {
      templateUrl: 'static/home/templates/home.html',
      controller: 'HomeController'
   }).
   when('/onboard', {
      templateUrl: 'static/onboard/templates/onboard_page.html',
      controller: 'OnboardController'
   }).
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
   when('/settings', {
      templateUrl: 'static/settings/templates/settings.html',
      controller: 'SettingsController'
   }).
   when('/settings/rooms/:id', {
      templateUrl: 'static/settings/templates/settings_rooms.html',
      controller: 'SettingsRoomsController'
   }).
   when('/settings/equipment/:id', {
      templateUrl: 'static/settings/templates/settings_equipment.html',
      controller: 'SettingsEquipmentController'
   }).
   when('/settings/sensors/:id', {
      templateUrl: 'static/settings/templates/settings_sensors.html',
      controller: 'SettingsSensorsController'
   }).
   when('/settings/actuators/:id', {
      templateUrl: 'static/settings/templates/settings_actuators.html',
      controller: 'SettingsActuatorsController'
   }).
   otherwise({
      redirectTo: '/home'
   });
}]);
