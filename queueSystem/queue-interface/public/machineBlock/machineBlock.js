import {app} from '../app';
import 'systemConstants/systemConstants';

app.directive('machineBlock', function(systemConstants) {
  return {
    restict: 'E',
    templateUrl: 'machineBlock/machineBlock.html',
    scope: {
      model: '=',
      currentCommunicateInput: '='
    },
    link: function(scope, element) {
      var actions = {
        MACHINE: {
          IN: function () {
            scope.model.channelFree = false;
          },
          OUT: function () {
            scope.model.channelFree = true ;
          }
        },
        QUEUE: {
          IN: function () {
            var toBeTakenIndex =  _.findIndex( scope.model.queuePlaces, {free: true});
            scope.model.queuePlaces[toBeTakenIndex] = {free: false};
            scope.$applyAsync();
          },
          OUT: function () {
            var nextCommunicateIndex = scope.model.queueType === 0 ?
              _.findIndex( scope.model.queuePlaces, {free: false}) :
              _.findLastIndex( scope.model.queuePlaces, {free: false});

            scope.model.queuePlaces[nextCommunicateIndex] = {free: true};
            scope.$applyAsync();
          }
        }
      };
      scope.$watch('currentCommunicateInput', function(communicate) {
        if(communicate && communicate.destination === scope.model.id && communicate.eventName !== 0) {
          actions[systemConstants.EVENT_NAME[communicate.eventName]][systemConstants.DIRECTION[communicate.direction]]();
        }
      });
    }
  }
});