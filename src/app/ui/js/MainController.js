// Main Controller js file

// Utilizes the AngularJS controller, can model other controllers after this one
function MainController($scope) {
    $scope.projectTitle = "Tiny House Project Dashboard";
    $scope.projectInfo = "Information goes here regarding the project or link";
    $scope.projectContributors = "Name some important people contact info here";
    $scope.projectNotes = "Additional notes";
};

// Injects the controller into the tinyhouseApp $scope
// to be called in the HTML
MainController.$inject = ['$scope'];
angular.module('tinyhouseApp', []).controller('MainController', MainController);