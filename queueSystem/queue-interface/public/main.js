import angular from 'angular';
import 'queueSystem';
import 'communicationStack/communicationStack';
import 'configurationPanel/configurationPanel';
import 'machineBlock/machineBlock';
import 'currentCommunicate/currentCommunicate';
import 'lineChart/lineChart';
import 'systemConstants/systemConstants';
import 'systemWatch/systemWatch';
import {app} from './app';

angular.element(document).ready(function () {
  angular.bootstrap(document.body, [app.name], {});
});
