var MaximCCD = require("./maximjs").MaximCCD
var fs = require('fs');
var async = require('async');

var maximCCD = new MaximCCD();


var defaultPath = "C:/Users/philippe/tmp";

exports.Expose = function (options,callback) {
	if (!options.ImagePath) {
		today = new Date();
		dayPath = defaultPath+"/"+today.getFullYear()+"-"+(today.getMonth()+1)+"-"+today.getDate();
		if (!fs.existsSync(dayPath))
			fs.mkdirSync(dayPath);
		options.ImagePath = dayPath;
	}
	if (options.Repeate)
		nbImage = options.Repeate;
	else
		nbImage=1;
	
function repeater(i,callback) {
	if (i < nbImage) {
		options.ImageIndex = i
		maximCCD.CCDExpose(options,function (err,result) {
			if (err)
				callback(err);
			else {
				console.log("Image :"+i+" done :",result);
				repeater(i+1,callback);
			}
		});
	}
	else
		callback(null);
}
repeater(0,callback);
}

exports.ExposeRGB = function (options,callback) {

async.series({
    Red: function(call){
		options.Filter = 0;
        exports.Expose(options,call);
    },
    Green: function(call){
		options.Filter = 1;
        exports.Expose(options,call);
    },
	Blue: function(call){
	options.Filter = 2;
    exports.Expose(options,call);
    },
},
function(err, results) {
    if (err)
		callback(err)
	else
		callback(null,"ExposeRGB ok");
});

}
