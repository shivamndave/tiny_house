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
   otherwise({
      redirectTo: '/home'
   });
}]);
