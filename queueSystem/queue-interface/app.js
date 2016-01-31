var PORT = 4005;

var express = require('express');
var app = express();
var server = require('http').createServer(app);
var bodyParser = require('body-parser');

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(express.static('public'));

var exec = require('child_process').exec;


app.route('/communicates').post(function(req, res) {
    console.log(req.body.parameters);
    exec('../komunikaty ' + req.body.parameters, {maxBuffer: 1024 * 10000}, function(error, stdout, stderr) {
        if (error !== null) {
            res.status(500).send(error);
        }
        else {
            res.status(200).send(JSON.parse(stdout.replace(/'/g, '"')));
        }
    });
});

server.listen(PORT, function () {
    console.log('Express server listening on %d', PORT);
});

// Expose app
exports = module.exports = app;
