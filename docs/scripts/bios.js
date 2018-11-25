var config = {
	apiKey: "AIzaSyCjs-YBtyd3D8r2w8qbaRsLY0mFCEFZumQ",
	authDomain: "r-log-in.firebaseapp.com",
	databaseURL: "https://r-log-in.firebaseio.com",
	projectId: "r-log-in",
	storageBucket: "r-log-in.appspot.com",
	messagingSenderId: "847645178492"
};
firebase.initializeApp(config);

var imgs = document.getElementsByClassName('headshot');

for (let img of imgs) {
	//img.src = "..\\images\\" + img.alt + ".jpg";
	img.onclick = function() { showBio(img.alt); };
}

document.getElementById('modal-close').onclick = function() { closeBio() };

function showBio(txt) {
	document.getElementById('memberBio').style.display = "block";
	const app = firebase.app();
	const db = firebase.firestore();
	const bio = db.collection('bios').doc(txt);
	bio.get().then(doc => {
		const data = doc.data();
		document.getElementById('modal-img').src = "..\\images\\" + txt + ".jpg";
		console.log(document.getElementById('modal-img').src);
		document.getElementById('modal-heading').innerHTML = data.name;
		document.getElementById('modal-text').innerHTML = data.bio;
		document.getElementById('modal-link').href = "mailto:" + data.email;
		document.getElementById('modal-link').innerHTML = data.email;
	});
}

function closeBio() {
	document.getElementById('memberBio').style.display = "none";
}