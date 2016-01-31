import {app} from './app';
import 'systemConstants/systemConstants';
import _ from 'lodash';

app.controller('queueSystem', function ($scope, $http, $timeout, systemConstants) {

  $scope.speed = 1;
  $scope.actualTime = 0;
  $scope.currentCommunicate = null;
  //$scope.machineBlocks = [{
  //  id: 0,
  //  channelFree: true,
  //  queueLength: 2,
  //  queuePlaces: [{free: true}, {free: true}],
  //  queueType: 0
  //}];

  $scope.$watch('configuration.machineBlocks', function(confMachineBlocks) {
    if(!confMachineBlocks) {
      $scope.machineBlocks = [];
      return;
    }

    $scope.machineBlocks = _.map(confMachineBlocks, function(block) {
      return {
        id: block.id,
        channelFree: true,
        queueLength: block.queueCapacity,
        queueType: block.queueType,
        queuePlaces: generateFreeQueuePlaces(block.queueCapacity)
      }
    });

    function generateFreeQueuePlaces(howMany) {
      var places = [];
      for(var i = 0; i < howMany; i++) {
        places.push({free: true});
      }
      return places;
    }
  }, true);

  $scope.start = function() {

    var machineBlocksValues = _.reduce($scope.configuration.machineBlocks, function(result, block) {
      return result.concat(_.values(block));
    }, []);

    machineBlocksValues = machineBlocksValues.concat([$scope.configuration.arrivalDistribution.type, $scope.configuration.arrivalDistribution.parameter]);

    var configurationString = _.reduce(machineBlocksValues, function(result, value) {
      return result += value + ' ';
    }, "");

    console.log(configurationString);
    $http.post('communicates', {parameters: configurationString}).then(function (res) {
      $scope.messages = _.sortBy(res.data, 'time');
      $scope.messages.pop();
      simulateRealtimeSimulation();
    });
  };

  function takeNextMessage(message) {
    $scope.currentMessage = message;
    $scope.actualTime = message.time;
    simulateRealtimeSimulation();
  }

  function simulateRealtimeSimulation() {
    $scope.currentMessage = $scope.messages.shift();
    if(_.isEmpty($scope.currentMessage)) {
      return;
    }
    $timeout(function() {
      takeNextMessage($scope.currentMessage);
    }, 500 / $scope.speed);
  }

});