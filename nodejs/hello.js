var http = require('http');
var querystring = require('querystring');


function getMeteo() {
	var options = {
	  host: '192.168.200.178',
	  path: '/jsonSensor',
	};
	var meteoObject ;
	var body ="";
	var req = http.request(options, function(res) {
	  res.on('data', function (chunk) {
		body += chunk;
		});
	  res.on('end',function() {
		meteoObject = JSON.parse(body);
		console.log(meteoObject.cloudsensor);
	  });
	});
	req.end();
}

function getTime() {
	var CurrentDate = new Date()
	console.log('Current Time :'+CurrentDate.getHours()+":"+CurrentDate.getMinutes()+":"+CurrentDate.getSeconds());
}





setInterval(getMeteo,5000);
//setInterval(getTime,5000);

