import {app} from '../app';

app.directive('systemWatch', function() {
  return {
    restict: 'E',
    template: '<h2 class="system-watch"> czas symulacji: {{actualTime}} </h2>',
    link: function(scope) {

    }
  }
});