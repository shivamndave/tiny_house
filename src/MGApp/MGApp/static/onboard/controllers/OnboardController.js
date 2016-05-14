var onboardController = angular.module('appController'),
    CLIENTNAME = "test.mosquitto.org",
    CLIENTPORT = 8080,
    CLIENTID   = "id",
    NEWSENSORTOPIC="/nodediscover/newsensor";


onboardController.controller('OnboardController', function($scope) {
  var client = new Paho.MQTT.Client(CLIENTNAME, CLIENTPORT, CLIENTID);
  $scope.show_form = false;
  $scope.sensordata = null;

  console.log("Now trying to connect...");
  client.connect({onSuccess:onConnect});

  client.onConnectionLost = onConnectionLost;
  client.onMessageArrived = onMessageArrived;

  $scope.sendOnboardMessage = function() {
    message = new Paho.MQTT.Message("Is there a sensor out there");
    message.destinationName = NEWSENSORTOPIC;
    client.send(message);
  }

  function onConnect() {
    console.log("onConnect");
    client.subscribe(NEWSENSORTOPIC);
    console.log("subscribed");
  }

  function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
      console.log("onConnectionLost:" + responseObject.errorMessage);
    }
  }

  function onMessageArrived(message) {
    console.log("onMessageArrived:" + message.payloadString);
    $scope.show_form = true;
    $scope.sensordata = {"string": message.payloadString};
    $scope.$apply();
  }
});

onboardController.directive('onboardForm', function(){
  return {
    restrict: 'E',
    scope: {
      sensordata: '='
    },
    link: link,
    templateUrl: 'static/onboard/templates/onboard_form.html'
  }
    function link(scope,element,attrs) {
      scope.$watchCollection(attrs.sensordata, function(newValue, oldValue){
        if(newValue) {
            console.log(newValue.string);
            scope.test = newValue.string;
        }
      }, true);}
});
