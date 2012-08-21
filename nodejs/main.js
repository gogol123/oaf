// main program

var exec = require('child_process').exec
var roof = require('./roof.js');
var telescope = require('./tpl2.js');


var SkypeUser = "philippelang";

process.on('uncaughtException', function (err) {
  console.error(err);
  console.log("Node NOT Exiting...");
});


//open roof
/*
roof.Open("OuvertureP",function (err){
	if (err) {
		console.log(err);
		child = exec('"C:/Program Files (x86)/Skype/Phone/skype.exe" /callto:"'+SkypeUser);	
	}
})
);
*/

telescope.NTMConnect();
var statusId=setInterval(telescope.getNTMStatus,2000);
telescope.slew(8.1,-3.0);
//setTimeout(exports.NTMDisconnect,20000);
