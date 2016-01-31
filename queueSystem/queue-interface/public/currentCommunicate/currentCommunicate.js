import {app} from '../app';

app.directive('currentCommunicate', function(systemConstants) {
  return {
    restict: 'E',
    templateUrl: './currentCommunicate/currentCommunicate.html',
    scope: {
      communicateCandidate: '=',
      currentCommunicateInput: '='
    },
    link: function(scope, element) {
      scope.systemConstants = systemConstants;

      scope.$watch('communicateCandidate', function(communicateCandidate) {
        if(communicateCandidate && communicateCandidate.messageType === 'currentCommunicate') {
          scope.currentCommunicateInput = communicateCandidate;
        }
      });
    }

  }
});