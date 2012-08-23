
//include 
var async = require('async');
var exec = require('child_process').exec
var roof = require('./roof.js');
var telescope = require('./tpl2.js');
var ccd = require('./ccd.js');
var Location = require("./maximjs").Location
var util =require('./util.js');


//constante
var osenbach = new Location(47.59,7.1244)
var SkypeUser = "philippelang";
var close = false;

// Error function , call skype
function Error(err) {
	console.log("Error occur :"+err);
	child = exec('"C:/Program Files (x86)/Skype/Phone/skype.exe" /callto:"'+SkypeUser);
}

//globale common object 

var Common = {
	_ccd : ccd,
	_telescope : telescope,
	_isClosing : false,
	};
	
//start Meteo watcher, check every 30s 
util.WatchMeteo(30000);	

//start Telescope status watcher every 5s
util.WatchTelescope(5000,Common);
	




var M57 = {
	ra : "18h53m35s",
	dec: "33d01m40s",
	}


var twilight = osenbach.Twilight();
console.log("Twilight start : "+twilight.start);
console.log("Twilight end : "+twilight.end);

var ra = util.hms_to_deg(M57.ra.decodeRa());
console.log(ra);
var dec = util.dms_to_deg(M57.dec.decodeDec())

var M57transit = osenbach.TransitTime(ra,dec);
console.log("M27 transit  : "+M57transit.transit)
console.log(osenbach.EqtoHz(ra,dec));
	

		
		
var ImageOptions = {
	Object : M57,			// only needed in case of SlewExpose function
	Repeate : 2   ,		// to specify a different nb of image betwen L and RGB , if only a value is set all the LRG will take the same 
	ExposureTime : [10,5],
	Filter : 3,				
	ObjectName : "M57",
//	Binning : 2,
//	ImagePath	 : "C:/Users/philippe/tmp",
};




async.series([ 
	//Open roof
//	function(callback) { roof.Open("OuvertureP",callback);},
	// Start telescop
	function(callback) {  telescope.powerOn(callback);},
	//slew and expose LRGB sequence 
	function(callback) { util.SlewExpose(ImageOptions,telescope,ccd,osenbach,callback);},
	//slew to 2nd target
	function(callback) { telescope.slew("18h53m35s","33d01m40s",osenbach,callback);},
	//Expose the Ha sequence with Filter 4
	function(callback) { ImageOptions.Filter=4; ccd.Expose(ImageOptions,callback);},
	// close all
	function(callback) {closaAll();callback(null)}
	],
	function(err,result){
	if (err)
		Error(err);
	else
		console.log("DONE !"+result);
	});


	