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

function signIn() {
	var provider = new firebase.auth.GoogleAuthProvider();
	firebase.auth().signInWithRedirect(provider);
}

firebase.auth().onAuthStateChanged(function(user) {
	if(user) {
		console.log("SIGN IN");
		window.location.href = "team.html";
	} else {
		console.log("SIGN OUT");
	}
});