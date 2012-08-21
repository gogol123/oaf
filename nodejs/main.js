
var async = require('async');
var exec = require('child_process').exec

var roof = require('./roof.js');
var telescope = require('./tpl2.js');
var ccd = require('./ccd.js');

var SkypeUser = "philippelang";


function Error(err) {
	console.log("Error occur :"+err);
	child = exec('"C:/Program Files (x86)/Skype/Phone/skype.exe" /callto:"'+SkypeUser);
}

//open roof
/*
roof.Open("OuvertureP",function (err){
	if (err) {
		Error(err);
	}
});
*/


//telescope.NTMConnect();
//var statusId=setInterval(telescope.getNTMStatus,2000);
//telescope.slew(8.1,-3.0);
//setTimeout(exports.NTMDisconnect,20000);


var options = {
	Repeate : 3,
	ExposureTime : 5,
	Filter : 0,
	ObjectName : "M27",
//	Binning : 2,
	ImagePath	 : "C:/Users/plang/temp",
};

async.series([ 
	//Expose the sequence with Filter 0 (Red)
	function(callback) { ccd.Expose(options,callback)},
	//Expose the sequence with Filter 1 (Gren)
	function(callback) { options.Filter = 1;ccd.Expose(options,callback)},
	// Close roof
	function(callback) {roof.Close(callback);}
	],
	function(err,result){
	if (err)
		Error(err);
	else
		console.log("OK"+result);
	});

	
	