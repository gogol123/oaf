
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


//check meteo every 30s 
MeteoTaskId = setInterval(roof.getMeteo,30000,function(err,result){
	if (err)
		console.log("Error occur");
	else{
		console.log("Meteo is :");
		console.log(result);
		}
	});
	
//check telescope status every 10s
telescope.NTMConnect();
TelescopeTaskId = setInterval(telescope.getNTMStatus,10000,function(err,result){
	if (err)
		console.log("Error occur");
	else{
		console.log("Telescope :");
		console.log(result);
		}
	});	


var target ={
	ra : {
		h : 19.0,
		mn: 59.0,
		s : 20.0,
	},
	dec: {
	d : 22.0,
	mn : 40.0,
	s   : 30.0,
	},
}

	
var ra = telescope.hms_to_deg(target.ra);
var dec = telescope.dms_to_deg(target.dec);

console.log (ra,dec);



//telescope.slew(8.1,-3.0);
//setTimeout(exports.NTMDisconnect,20000);


var ImageOptions = {
	Repeate : 3,
	ExposureTime : 5,
	Filter : 0,
	ObjectName : "M27",
//	Binning : 2,
	ImagePath	 : "C:/Users/philippe/tmp",
};


async.series([ 
	//Open roof
	function(callback) { roof.Open("OuvertureP",callback)},
	// Start telescop
	function(callback) {  telescope.powerOn();},
	//Expose the sequence with Filter 0 (Red)
	function(callback) { ccd.Expose(ImageOptions,callback)},
	//Expose the sequence with Filter 1 (Gren)
	function(callback) { ImageOptions.Filter = 1;ccd.Expose(ImageOptions,callback)},
	// Close roof
	function(callback) {roof.Close(callback);},
	//Stop all the watcher (meteo and NTM
	function(callback) { clearInterval(MeteoTaskId);clearInterval(TelescopeTaskId)}
	],
	function(err,result){
	if (err)
		Error(err);
	else
		console.log("OK"+result);
	});



	