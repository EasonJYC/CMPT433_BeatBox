"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();
$(document).ready(function() {
	window.setInterval(function() {updateIndex()}, 1000);
	$('#modeNone').click(function(){
		$('#modeid').html('none');
		sendbeatboxCommand("model 0");
	});
	$('#modeRock1').click(function(){
		sendbeatboxCommand("model 1");
		$('#modeid').html('model 1');
	});
	$('#modeRock2').click(function(){
		sendbeatboxCommand("model 2");
		$('#modeid').html('model 2');
	});
    $('#volumeDown').click(function(){
		sendbeatboxCommand("decrease volume");
	});
    $('#volumeUp').click(function(){
		sendbeatboxCommand("increase volume");
    });
    $('#tempoDown').click(function(){
		sendbeatboxCommand("bpm slower");
	});
    $('#tempoUp').click(function(){
		sendbeatboxCommand("bpm faster");
	});
	$('#sample1').click(function(){
		sendbeatboxCommand("base");
		$('#modeid').html('base');
	});
	$('#sample2').click(function(){
		sendbeatboxCommand("hihat");
		$('#modeid').html('hihat');
	});
	$('#sample3').click(function(){
		sendbeatboxCommand("snare");
		$('#modeid').html('snare');

		
	});

	function updateIndex(){
		sendbeatboxCommand("update");
	}
	var volume = 80;
	var tempo = 120;
	socket.on('increaseVolume', function(result){
		if(volume <= 95){
			volume += 5;
		}
		document.getElementById("volumeid").value = volume;
	})
	socket.on('decreaseVolume', function(result){
		if(volume >= 5){
			volume -= 5;
		}
		document.getElementById("volumeid").value = volume;
	})
	socket.on('bpmFaster', function(result){
		if(tempo <= 295){
			tempo += 5;
		}
		document.getElementById("tempoid").value = tempo;
	})
	socket.on('bpmSlower', function(result){
		if(tempo >= 45){
			tempo -= 5;
		}
		document.getElementById("tempoid").value = tempo;
	})
	socket.on('updateIndex',function(result){
		var myArray = result.match(/\d+/g);
		document.getElementById("tempoid").value = myArray[0];
		document.getElementById("volumeid").value = myArray[1];
		var hour = Math.floor(myArray[2]/3600);
		var minute = Math.floor((myArray[2]-3600*hour)/60);
		var seconds = (myArray[2] - 3600*hour - 60*minute);
		var string = hour + ':' + minute + ':' +seconds;
		$('#status').html(string);
	})
	
	
});

function sendbeatboxCommand(message) {
	socket.emit('audioMixer', message);
};