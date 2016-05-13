var onboardController = angular.module('appController'),
    CLIENTNAME = "test.mosquitto.org",
    CLIENTPORT = 8080,
    CLIENTID   = "id";

onboardController.controller('OnboardController', function($scope) {
  var client = new Paho.MQTT.Client(CLIENTNAME, CLIENTPORT, CLIENTID);

  console.log("Now trying to connect...");
  client.connect({onSuccess:onConnect});

  client.onConnectionLost = onConnectionLost;
  client.onMessageArrived = onMessageArrived;

  $scope.sendMQTTMessage = function(inp) {
    if(inp) {
      message = new Paho.MQTT.Message(inp.toString());
      message.destinationName = "test_thing_we_have";
      client.send(message);
    } else {
      console.log("No String")
    }
  }

  function onConnect() {
    console.log("onConnect");
    client.subscribe("test_thing_we_have");
    console.log("subscribed");
  }

  function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
      console.log("onConnectionLost:"+responseObject.errorMessage);
    }
  }

  function onMessageArrived(message) {
    console.log("onMessageArrived:"+message.payloadString);
    $scope.mqtt_message = message.payloadString;
    $scope.$apply();
  }
});
