// main program

var exec = require('child_process').exec
var roof = require('./roof.js');


var SkypeUser = "philippelang";

process.on('uncaughtException', function (err) {
  console.error(err);
  console.log("Node NOT Exiting...");
});


//open roof
roof.Open("OuvertureP",function (err){
	if (err) {
		console.log(err);
		child = exec('"C:/Program Files (x86)/Skype/Phone/skype.exe" /callto:"'+SkypeUser);	
	}
})
);