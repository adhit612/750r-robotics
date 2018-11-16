var imgs = document.getElementsByClassName('headshot');

for (let img of imgs) {
	img.onclick = function() { showBio(img.alt); };
}

function showBio(txt) {
	window.alert(txt);
}