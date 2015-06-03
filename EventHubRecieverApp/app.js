﻿
/**
 * Module dependencies.
 */

var express = require('express');
var routes = require('./routes');
var http = require('http');
var path = require('path');

var app = express();
var config = require('./config');

// all environments
app.set('port', process.env.PORT || 3000);
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.json());
app.use(express.urlencoded());
app.use(express.methodOverride());
app.use(app.router);
app.use(require('stylus').middleware(path.join(__dirname, 'public')));
app.use(express.static(path.join(__dirname, 'public')));

// development only
if ('development' == app.get('env')) {
    app.use(express.errorHandler());
}

app.get('/', routes.index);
app.get('/about', routes.about);
app.get('/contact', routes.contact);
app.get('/sensorData', function (req, res) {
    var temp = req.query.temp;
    var humidity = req.query.humidity;
    var deviceId = req.query.deviceId;

    var client = require('event-hub-client').restClient(
        "esp8266",
        "sensorData",
        "sender",
        "4B5LSN7kdr6ESKBSn6OtVLom0YjvxOVbjKx8PAVvg6Q=");
    console.log("Sending message");
    client.sendMessage('{ "temp": '+ temp +', "humidity": '+ humidity + ', "deviceId": "'+ deviceId + '"}', callback);
    res.send("Message sent");
});

http.createServer(app).listen(app.get('port'), function () {
    console.log('Express server listening on port ' + app.get('port'));
});

function callback(err, response)
{
    console.log("message was sent");
}