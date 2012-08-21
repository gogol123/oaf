
var http = require('http');
var querystring = require('querystring');


//Etat Toit 

var statusTable = [
	'Toit ferme',
	'Tympan Ouverture intermedaire',
	'Tympan ouverture totale',
	'Tympan fermeture1',
    'Tympan fermeture',
	'Toit fermeture',
	'Toit ouverture',
	'Toit ouvert',
	'Arret Urgence',
    'Tympan fermeture',
	'Park Telescope',
	'Toit ouverture',
	'Toit aeration',
	'Tympan Ouverture intermedaire'
	];
	
exports.getStatus = function (callback) {
	var options = {
	  host: '192.168.200.177',
	  path: '/json/capteur',
	};
	var ToitStatus ;
	var body ="";
	var req = http.request(options, function(res) {
	  res.setEncoding('utf8');

	  res.on('response', function (chunk) {
		body += chunk;
		});
	  res.on('end',function() {
		ToitStatus = JSON.parse(body);
		callback(null,statusTable[ToitStatus.CurrentState]);
		//console.log("ToitStatus="+statusTable[ToitStatus.CurrentState]);
	  });
   });
  req.on('error',function(err){
	   req.abort();
	   callback( err);
	   });
  req.end();
}


  function HandelRoof(action) {
 	var post_data = querystring.stringify({
      		'action' : action,
  	});
	var options = {
	  host: '192.168.200.177',
	  path: '/',
	  method : 'POST',
 	  headers: {
          	'Content-Type': 'application/x-www-form-urlencoded',
          	'Content-Length': post_data.length
      		}
	};
	
	var body ="";
	var req = http.request(options, function(res) {
	  res.setEncoding('utf8');
	});
	req.write(post_data);
	req.end();
}



exports.Open = function(action,callback) {
	function isRoofOpen(callback) {
	exports.getStatus(function(err,result){
		if (err) 
			callback (err); 
		else 
			if ( (action == "OuvertureA" && result != "Toit aeration") || 
				 ( (action == "OuvertureP" ||  action == "OuvertureT" )&& result != "Toit ouvert")) {		
			}
	});
	}
						
	setTimeout(isRoofOpen , 80000,callback);
	exports.getStatus(function(err,result){
		if (err) 
			callback (err); 
		else {
			if (result != "Toit ouvert")
				HandelRoof(action);
		}			
	});
}


exports.Close = function(callback) {
	function isRoofClosed(callback) {
	exports.Status(function(err,result){
		if (err) 
			throw err
		else 
			if ( result != "Toit ferme") {
				callback (new Error("Error occur in opening roof : ROOF IS NOT CLOSED!")); 
			}
	});
	}
	setTimeout(isRoofClosed , 80000,callback);	
	exports.getStatus(function(err,result){
		if (err) 
			callback(err);
		else {
			if (result != "Toit ferme")
				HandelRoof("Fermeture");
		}			
	});
}



