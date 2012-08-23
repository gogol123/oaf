
var async = require('async');
var exec = require('child_process').exec

var roof = require('./roof.js');
var telescope = require('./tpl2.js');
var ccd = require('./ccd.js');
var Location = require("./maximjs").Location


var osenbach = new Location(47.59,7.1244)
var SkypeUser = "philippelang";
var close = false;

function Error(err) {
	console.log("Error occur :"+err);
	child = exec('"C:/Program Files (x86)/Skype/Phone/skype.exe" /callto:"'+SkypeUser);
}

	

//check meteo every 30s 
var CounterMeteo=0;
MeteoTaskId = setInterval(roof.getMeteo,30000,function(err,result){
	if (err)
		console.log("Error occur");
	else{
		console.log("Meteo is :");
		//console.log(result);
		if ( (result.SkyTemp > 1.0) || (result.Rain > 2100)){
			console.log ("condition pour femeture meteo:"+CounterMeteo);
			CounterMeteo++;
			if (CounterMeteo > 10)
				closaAll();
		}
		else
			CounterMeteo=0;
		}
	});
	
//check telescope status every 10s


telescope.NTMConnect();
TelescopeTaskId = setInterval(telescope.getNTMStatus,10000,function(err,result){
	if (err)
		console.log("Error occur");
	else{
		//console.log("Telescope :");
		//console.log(result);
		;
		}
	});	
	
function closaAll() {
console.log("CLOSE ALL");
clearInterval(MeteoTaskId);
close = true;
async.series([ 
	// park telescope
	function(callback) {telescope.park(callback);},
	// close roof
	function(callback) {roof.Close(callback);},
	//Stop all the watcher (meteo and NTM)
	function(callback) { clearInterval(TelescopeTaskId);telescope.NTMDisconnect();callback(null);}
	],
	function(err,result){
	if (err)
		Error(err);
	else
		console.log("DONE !"+result);
		process.exit();
	});
}	


var M57 ={
	ra : {
		h : 18.0,
		mn: 53.0,
		s : 35.0,
	},
	dec: {
	d : 33.0,
	mn : 1.0,
	s   : 47.0,
	},
}

	
var ra = telescope.hms_to_deg(M57.ra);
var dec = telescope.dms_to_deg(M57.dec);

var twilight = osenbach.Twilight();
console.log("Twilight start : "+twilight.start);
console.log("Twilight end : "+twilight.end);


var M57transit = osenbach.TransitTime(ra,dec);
console.log("M27 transit  : "+M57transit.transit)
console.log(osenbach.EqtoHz(ra,dec));
	

		
		
var ImageOptions = {
	Repeate : 1,
	ExposureTime : 60,
	Filter : 3,
	ObjectName : "M57",
//	Binning : 2,
//	ImagePath	 : "C:/Users/philippe/tmp",
};


async.series([ 
	//Open roof
//	function(callback) { roof.Open("OuvertureP",callback);},
	// Start telescop
//	function(callback) {  telescope.powerOn(callback);},
	//slew to target
	function(callback) { telescope.slew("18h53m35s","33d01m40s",osenbach,callback);},
	//Expose the luminance sequence with Filter 3
	function(callback) { ccd.Expose(ImageOptions,callback);},
	//Expose the RGB sequence 
	function(callback) { ccd.ExposeRGB(ImageOptions,callback);}
	// close all
	//,function(callback) {closaAll();callback(null)}
	],
	function(err,result){
	if (err)
		Error(err);
	else
		console.log("DONE !"+result);
	});


	