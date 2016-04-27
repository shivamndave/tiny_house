var app = angular.module("app", ['ngRoute', 'appController']);
app.config(['$routeProvider', function($routeProvider) {
   $routeProvider.
   
   when('/home', {
      templateUrl: './home/templates/home.html',
      controller: 'HomeController'
   }).
   
   otherwise({
      redirectTo: '/home'
   });
}]);
