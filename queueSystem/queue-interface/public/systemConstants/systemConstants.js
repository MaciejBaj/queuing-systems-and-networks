import {app} from '../app';

app.constant('systemConstants', {
  QUEUES : {
    0: 'FIFO',
    1: 'LIFO'
  },

  EVENT_NAME : {
    0: 'BLOCK',
    1: 'MACHINE',
    2: 'QUEUE'
  },

  DIRECTION : {
    0: 'OUT',
    1: 'IN'
  }
});