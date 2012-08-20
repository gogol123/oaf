//Object name: HIP 78459 SAO 65024
//Magnitude: 5,39
//Equatorial: RA: 16h 01m 32.637s   Dec: +33°16'23.860"(current)
//Equatorial 2000: RA: 16h 01m 02.463s   Dec: +33°18'02.864"


function hms_to_deg( heq) {
	heq.ra = (heq.ra_h / 24) * 360;
    heq.ra += (heq.ra_mn / 60) * 15;
	
	heq.dec =  heq.dec_d;
    heq.dec += heq.dec_mn / 60;
}

var Location = require("./maximjs").Location;
var MaximCCD = require("./maximjs").MaximCCD;


//test lib nova
var osenbach = new Location(47.59,7.1244)

var twilight = osenbach.Twilight();
console.log("Twilight start : "+twilight.start);
console.log("Twilight end : "+twilight.end);

var Coord = {
	ra_h : 16,
	ra_mn : 01,
	dec_d : 33,
	dec_mn : 16,
	ra :0,
	dec : 0,
	}

	
hms_to_deg(Coord);
var transit = osenbach.TransitTime(Coord.ra,Coord.dec);
console.log("SAO 65024 transit  : "+transit.transit)

var ccd = new MaximCCD();

var options = {
	ExposureTime : 5,
	Filter : 3,
	ObjectName : "SAO_65024",
	Binning : 1,
	ImagePath	 : "C:/Users/philippe/tmp",
};

function repeater(i) {
	if (i < 5) {
		options.ImageIndex = i
		ccd.CCDExpose(options,function (err,result) {
			if (err)
			console.log("Error occur :"+err);
			else {
				console.log("Image :"+i+" done :",result);
				repeater(i+1);
			}
		});
	}
}
repeater(0);


