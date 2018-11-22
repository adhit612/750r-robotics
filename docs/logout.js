// Initialize Firebase
var config = {
	apiKey: "AIzaSyCjs-YBtyd3D8r2w8qbaRsLY0mFCEFZumQ",
	authDomain: "r-log-in.firebaseapp.com",
	databaseURL: "https://r-log-in.firebaseio.com",
	projectId: "r-log-in",
	storageBucket: "r-log-in.appspot.com",
	messagingSenderId: "847645178492"
};
firebase.initializeApp(config);

function signOut() {
	firebase.auth().signOut();
}

firebase.auth().onAuthStateChanged(function(user) {
	if(user) {
		console.log("SIGN IN");
		var username = user.displayName;
		var photoUrl = user.photoURL;
		
		document.getElementById("username").innerHTML = "Welcome<br/>~ " + username + " ~";
		document.getElementById("pfp").src = photoUrl;
		
	} else {
		console.log("SIGN OUT");
		window.location.href = "../index.html"
	}
});