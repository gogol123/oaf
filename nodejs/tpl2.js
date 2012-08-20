var net = require('net');

var ntm;

var CmdCompletePattern= new RegExp("10 COMMAND COMPLETE","m");

var HAPattern = new RegExp			("10 DATA INLINE HA.CURRPOS=(-?[0-9]*.[0-9]*)","m");
var DECPattern = new RegExp			("10 DATA INLINE DEC.CURRPOS=(-?[0-9]*.[0-9]*)","m");
var PowerStatePattern = new RegExp	("10 DATA INLINE CABINET.POWER_STATE=([0-9]*.[0-9]*)","m");
var ReferendedPattern = new RegExp	("10 DATA INLINE CABINET.REFERENCED=([0-9]*.[0-9]*)","m");
var GlobalStatusPattern = new RegExp("10 DATA INLINE CABINET.STATUS.GLOBAL=([0-9]*.[0-9]*)","m");
var TrackPattern = new RegExp		("10 DATA INLINE POINTING.TRACK=([0-9]*.[0-9]*)","m");
var HAMotionPattern = new RegExp	("10 DATA INLINE HA.MOTION_STATE=([0-9]*)","m");
var DECMotionPattern = new RegExp	("10 DATA INLINE DEC.MOTION_STATE=([0-9]*)","m");

var ntmAnwser = "";


var TelescopStatus = {
	Power : 0,
	Referenced : 0,
	Globalstatus : 0,
	CurrHA : 0.0,
	CurrDec: 0.0,
	Track : 0,
	HAMotionstate : 0,
	HAMotionstate : 0,

};
	

getNTMStatus = function () {

	
	console.log ("get telescope status");
	ntmAnwser = "";
	ntm.write('10 GET CABINET.POWER_STATE;CABINET.REFERENCED;CABINET.STATUS.GLOBAL;POINTING.TRACK;HA.CURRPOS;HA.MOTION_STATE;DEC.CURRPOS;DEC.TARGETPOS;DEC.MOTION_STATE\n');
		

}

NTMConnect = function () {

 ntm = net.connect(65432,'192.168.200.195', function(){
   ntm.on('data', function(data) {
   ntmAnwser+=data.toString();
	if (data.toString().search(CmdCompletePattern) !=-1){
	//	console.log(ntmAnwser);
	 TelescopStatus.CurrHA   = parseFloat(ntmAnwser.match(HAPattern)[1]);
	 TelescopStatus.CurrDec  = parseFloat(ntmAnwser.match(DECPattern)[1]);
	 TelescopStatus.Power= parseFloat(ntmAnwser.match(PowerStatePattern)[1]);
	 TelescopStatus.Referenced= parseFloat(ntmAnwser.match(ReferendedPattern)[1]);
	 TelescopStatus.Globalstatus= parseFloat(ntmAnwser.match(GlobalStatusPattern)[1]);
	 TelescopStatus.Track= parseFloat(ntmAnwser.match(TrackPattern)[1]);
	 TelescopStatus.HAMotionstate= parseFloat(ntmAnwser.match(HAMotionPattern)[1]);
	 TelescopStatus.DECMotionstate= parseFloat(ntmAnwser.match(DECMotionPattern)[1]);
	 console.log(TelescopStatus);
	 ntmAnwser = "";
	 }
   });

   ntm.on('error', function(err) {
     console.log('error:', err.message);
	 
   });

  ntm.write('01 SET SERVER.CONNECTION.EVENTMASK=0\n');
});
}
NTMDisconnect = function() {
	ntm.end('DISCONNECT');
	clearInterval(statusId);
}


NTMConnect();
var statusId=setInterval(getNTMStatus,2000);
setTimeout(NTMDisconnect,20000);


