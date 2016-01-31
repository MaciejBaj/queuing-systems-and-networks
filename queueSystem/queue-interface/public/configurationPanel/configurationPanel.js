import {app} from '../app';

app.directive('configurationPanel', function() {
  return {
    restict: 'E',
    scope: {
      configuration: '='
    },
    templateUrl: 'configurationPanel/configurationPanel.html',
    link: function(scope) {
      var initialMachineBlock = {
        distribution: 0,
        distributionParameter: 60,
        queueType: 0,
        queueCapacity: 0,
        outputMachineNumber: -1,
        sourceMachineNumber: -1,
        id: 0
      };

      scope.configuration = {
        machineBlocks: [],
        arrivalDistribution: {
          type: 0,
          parameter: 120
        }
      };

      scope.SELECTABLE_OPTIONS = {
        DISTRIBUTION: ['exponential', 'unormalized', 'normalized', 'accurate'],
        QUEUE_TYPE: ['FIFO', 'LIFO']
      };

      scope.addNewMachineBlock = function() {
        initialMachineBlock.id = scope.configuration.machineBlocks.length;
        scope.configuration.machineBlocks.push(_.cloneDeep(initialMachineBlock));
      };
      scope.removeMachineBlock = function() {
        scope.configuration.machineBlocks.pop();
        //_.remove(scope.configuration.machineBlocks, configuration);
      };

      scope.addNewMachineBlock();
    }
  }
});