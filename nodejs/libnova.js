var M27Coord = {
	ra_h : 20,
	ra_mn : 00,
	dec_d : 22,
	dec_mn : 46,
	ra :0,
	dec : 0,
	}


function hms_to_deg( heq) {
	heq.ra = (heq.ra_h / 24) * 360;
    heq.ra += (heq.ra_mn / 60) * 15;
	
	heq.dec =  heq.dec_d;
    heq.dec += heq.dec_mn / 60;
	}



	
	

var Location = require("./maximjs").Location

//test lib nova
var osenbach = new Location(47.59,7.1244)
var tomorrow = new Date();
var  now = new Date();
tomorrow.setTime(tomorrow.getTime() + (1000*3600*24));
var sun = osenbach.SunRiseSet();
console.log(sun.rise)
console.log(sun.transit)
console.log(sun.set)
var sun2 = osenbach.SunRiseSet(tomorrow);
console.log(sun2.rise)
console.log(sun2.transit)
console.log(sun2.set) 

var twilight = osenbach.Twilight();
console.log("Twilight start : "+twilight.start)
console.log("Twilight end : "+twilight.end)


hms_to_deg(M27Coord);
var M27transit = osenbach.TransitTime(M27Coord.ra,M27Coord.dec);
console.log("M27 transit  : "+M27transit.transit)
//test MaximDl
var MaximCCD = require("./maximjs").MaximCCD

var ccd = new MaximCCD();

var options = {
	ExposureTime : 5,
	Filter : 3,
	ObjectName : "M27",
	Binning : 2,
	ImagePath	 : "C:/Users/philippe/tmp",
};


