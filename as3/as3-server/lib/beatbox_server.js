"use strict";
/*
 * Respond to commands over a websocket to relay UDP commands to a local program
 */

var socketio = require('socket.io');
var io;

var dgram = require('dgram');

exports.listen = function(server) {
	io = socketio.listen(server);
	io.set('log level 1');
	
	io.sockets.on('connection', function(socket) {
		handleCommand(socket);
	});
};

function handleCommand(socket) {
	// Pased string of comamnd to relay
	socket.on('audioMixer', function(data) {
		console.log('audioMixer command: ' + data);
		// Info for connecting to the local process via UDP
		var PORT = 12345;
		var HOST = '192.168.7.2';
		var buffer = new Buffer(data);

		var client = dgram.createSocket('udp4');
		client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes) {
		    if (err) 
		    	throw err;
		    console.log('UDP message sent to ' + HOST +':'+ PORT);
		});
		
		client.on('listening', function () {
		    var address = client.address();
		    console.log('UDP Client: listening on ' + address.address + ":" + address.port);
		});
		// Handle an incoming message over the UDP from the local application.
		client.on('message', function (message, remote) {
		    console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);
		    
			// var reply = message.toString('utf8');
			var reply = message.toString().trim();
			if(reply.indexOf('increase volume') > -1){
				socket.emit('increaseVolume', reply);
			}
			else if(reply === 'decrease volume'){
				socket.emit('decreaseVolume', reply);
			}
			else if(reply === 'decrease bpm'){
				socket.emit('bpmSlower', reply);
			}
			else if(reply === 'increase bpm'){
				socket.emit('bpmFaster', reply);
			}
			else if(reply.indexOf('updating') > -1){
				socket.emit('updateIndex',reply);
			}

		    client.close();

		});
		client.on("UDP Client: close", function() {
		    console.log("closed");
		});
		client.on("UDP Client: error", function(err) {
		    console.log("error: ",err);
		});
	});
};