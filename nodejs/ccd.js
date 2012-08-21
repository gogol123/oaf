var MaximCCD = require("./maximjs").MaximCCD
var maximCCD = new MaximCCD();


exports.Expose = function (options,callback) {
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