import {app} from '../app';

app.directive('communicationStack', function(systemConstants) {
  return {
    restict: 'E',
    templateUrl: 'communicationStack/communicationStack.html',
    scope: {
      communicateCandidate: '=',
      currentCommunicateInput: '='
    },
    link: function(scope, element) {
      scope.systemConstants = systemConstants;
      scope.communicationStack = [];
      scope.$watch('communicateCandidate', function(communicateCandidate) {
        if(communicateCandidate && communicateCandidate.messageType === 'newCommunicate') {
          scope.communicationStack.push(communicateCandidate);
        }
      });

      scope.$watch('currentCommunicateInput', function(communicateCandidate) {
        if(!communicateCandidate) {
          return;
        }
        scope.communicationStack.shift();
      });

    }

  }
});