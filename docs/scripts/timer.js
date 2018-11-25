var end = Date.parse("December 1, 2018 07:00:00");

setInterval(function() {
	var now = Date.now();
	
	var timeLeft = end - now;
	
	document.getElementById("d").innerHTML = Math.floor(timeLeft / (1000 * 60 * 60 * 24)) + "d : ";
	document.getElementById("h").innerHTML = Math.floor(timeLeft % (1000 * 60 * 60 * 24) / (1000 * 60 * 60)) + "h : ";
	document.getElementById("m").innerHTML = Math.floor(timeLeft % (1000 * 60 * 60) / (1000 * 60)) + "m : ";
	document.getElementById("s").innerHTML = Math.floor(timeLeft % (1000 * 60) / 1000) + "s";
	
}, 1000);