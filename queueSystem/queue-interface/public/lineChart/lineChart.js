import {app} from '../app';
import c3 from 'c3';

app.directive('lineChart', function($timeout) {
  return {
    restict: 'E',
    template: '<h1>{{messageType}}</h1><br/></h1><div id="{{messageType}}" /><hr/>',
    scope: {
      currentMessage: '=',
      messageType: '@'
    },
    link: function(scope, element) {

      var chart;
      var points = {
        times: ['x'],
        values: [scope.messageType]
      };

      $timeout(function() {
        renderChart();
      });

      function renderChart() {
        chart = c3.generate({
          bindto: '#' + scope.messageType,
          data: {
            x: 'x',
            columns: [

            ]
          }
        });
      }

      scope.$watch('currentMessage', function(communicate) {
        if(communicate && communicate.messageType === scope.messageType) {
          points.times.push(communicate.time);
          points.values.push(communicate.value);

          //chart.unload();
          chart.load({
            columns: [
              points.times, points.values
            ],
            unload: [scope.messageType]
          });
        }
      });
    }
  }
});/**
 * Created by maciej on 31.01.16.
 */
