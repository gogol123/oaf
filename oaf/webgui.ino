
void JsonGetCapteur(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  
 if (type == WebServer::POST)
  {
    server.httpFail();
    return;
  }

  server.httpSuccess("application/json");
  
  if (type == WebServer::HEAD)
    return;
  server << "{";
  server << " \"ToitFerme\" : \"" << CapteurToitFermer << "\",\n"; 
  server << " \"ToitOuvert\" : \"" << CapteurToitOuvert << "\",\n"; 
  server << " \"TympanOuvert\" : \"" << CapteurTympanOuvert << "\",\n"; 
  server << " \"TympanFerme\" : \"" << CapteurTympanFermer << "\",\n"; 
  server << " \"TympanIntermediaire\" : \"" << CapteurTympanIntermediaire << "\",\n"; 
  server << " \"Telescope\" : \"" << CapteurTelescope << "\",\n"; 
  server << " \"UserAction\" : \"" << UserAction << "\",\n"; 
  server << " \"CurrentState\" : \"" << CurrentState << "\",\n"; 
  server << " \"OuvertureTympan\" : \"" << digitalRead(relaisOuvertureTympan) << "\",\n"; 
  server << " \"FermetureTympan\" : \"" << digitalRead(relaisFermetureTympan) << "\",\n"; 
  server << " \"OuvertureToit\" : \"" << digitalRead(relaisOuvertureToit) << "\",\n"; 
  server << " \"FermetureToit\" : \"" << digitalRead(relaisFermetureToit) << "\",\n"; 
  server << " \"TelescopeTpl2Connected\" : \"" << TelescopeTpl2Connected << "\"\n"; 

  server << "} \n ";

}





#define NAMELEN 32
#define VALUELEN 32



void connectTelecope(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool){
   URLPARAM_RESULT rc;
   char name[NAMELEN];
   int  name_len;
   char value[VALUELEN];
   int value_len;

  if (strlen(url_tail)) {
  
    while (strlen(url_tail)){
        rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
#if DEBUG > 2
        Serial.println(name);
       Serial.println(value );
#endif
      if (rc == URLPARAM_EOS)
        ;
     else {
       if (strcasecmp(name,"Tpl2")==0) {
        if (strcasecmp(value,"true") ==0){
          TelescopeTpl2Connected =tpl2Connect(ntm);
        }
        if (strcasecmp(value,"false") ==0) {
          TelescopeTpl2Connected = false;  
          tpl2Disconnect();
        }
       }
       if (strcasecmp(name,"FerAuto")==0) {
        if (strcasecmp(value,"true") ==0){
          FermetureAuto =true;
        }
        if (strcasecmp(value,"false") ==0) {
          FermetureAuto = false;  
        }
       }
    }
  }
}
}
void setlumiere(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool)
{
   URLPARAM_RESULT rc;
   char name[NAMELEN];
   int  name_len;
   char value[VALUELEN];
   int value_len;

  if (strlen(url_tail)) {
  
    while (strlen(url_tail)){
        rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc == URLPARAM_EOS)
        ;
     else {
      if (strcasecmp(value,"lumiereOn") ==0){
            transmitHomeEasy(true,0); // 0 adress de la lampe
              delay(10);
              transmitHomeEasy(true,0); // 0 adress de la lampe
      }
      if (strcasecmp(value,"lumiereOff") ==0){
        transmitHomeEasy(false,0); // 0 adress de la lampe
              delay(10);
              transmitHomeEasy(false,0); // 0 adress de la lampe
      }
      if (strcasecmp(value,"park")==0 ){
            tpl2Park();
      }
      if (strcasecmp(value,"unpark")==0 ){
 //             UnPark();
      }
       if (strcasecmp(value,"ToitFermeture")==0 ){
            UserAction =ActionFermeture;
            LogOnInternet(1,"fermeture_cardbox","system");
          }
        if (strcasecmp(value,"ToitOuverture")==0 ){
            UserAction=ActionOuverturePartiel;
            LogOnInternet(1,"ouverture_cardbox","system");
          }
        if (strcasecmp(value,"PowerOff")==0 ){
            tpl2Power(0);
          }
     }
   }
  }
  server.httpSuccess();
}

 
void HtmlFermetureAuto(WebServer &server, WebServer::ConnectionType type, char *url, bool)
{
   wdt_reset();   //pat the dog :)

  if (type == WebServer::POST) {
    bool repeat;
    char name[16], value[16];
    meteoValues.DarknessCheck = false ;
    meteoValues.ClarityCheck=false;
    meteoValues.SkyTempCheck=false;
    meteoValues.RainCheck=false;
    meteoValues.HumidityCheck=false;
    do
    {
      /* readPOSTparam returns false when there are no more parameters
       * to read from the input.  We pass in buffers for it to store
       * the name and value strings along with the length of those
       * buffers. */
      repeat = server.readPOSTparam(name, 16, value, 16);
      if (strcasecmp(name,"ObscurityCheck")==0){
        if(strcasecmp(value,"on")==0) 
          meteoValues.DarknessCheck=true;
      }
      if (strcasecmp(name,"ClarityCheck")==0){
        if(strcasecmp(value,"on")==0 )
          meteoValues.ClarityCheck=true;
      }
     if (strcasecmp(name,"SkyTempCheck")==0){
        if(strcasecmp(value,"on") ==0)
          meteoValues.SkyTempCheck=true;
      }
     if (strcasecmp(name,"PluieCheck")==0){
        if(strcasecmp(value,"on")==0 )
          meteoValues.RainCheck=true;
      }
     if (strcasecmp(name,"HumCheck")==0){
        if(strcasecmp(value,"on")==0 )
          meteoValues.HumidityCheck=true;
      }      
  
       
// value

      if (strcasecmp(name,"ObscurityValue")==0){
        meteoValues.DarknessTheshold = atof(value);
      }
      if (strcasecmp(name,"ClarityValue")==0){
        meteoValues.ClarityThreshold = atof(value);
      }
     if (strcasecmp(name,"SkyTempValue")==0){
        meteoValues.SkyTempThreShold = atof(value);
      }
     if (strcasecmp(name,"PluieValue")==0){
        meteoValues.RainThreshold = atoi(value);
      }
     if (strcasecmp(name,"HumValue")==0){
        meteoValues.HumidityThresold = atof(value);
      }      
      
      }while (repeat);
    }
    

    saveMeteoConfig();

    HtmlCmd(server,type,url,false);
}  


void HtmlCmd(WebServer &server, WebServer::ConnectionType type, char *url, bool)
{
   wdt_reset();   //pat the dog :)

  if (type == WebServer::POST) {
    bool repeat;
    char name[16], value[16];
    do
    {
      /* readPOSTparam returns false when there are no more parameters
       * to read from the input.  We pass in buffers for it to store
       * the name and value strings along with the length of those
       * buffers. */
      repeat = server.readPOSTparam(name, 16, value, 16);
      if (strcasecmp(name,"action") ==0) {
          if (strcasecmp(value,"lumiereOff")==0) {
              transmitHomeEasy(false,0); // 0 adress de la lampe
              delay(10);
              transmitHomeEasy(false,0); // 0 adress de la lampe
          }
          if (strcasecmp(value,"lumiereOn")==0 ){
              transmitHomeEasy(true,0); // 0 adress de la lampe
              delay(10);
              transmitHomeEasy(true,0); // 0 adress de la lampe
          }
          if (strcasecmp(value,"PC1Off")==0){
              transmitHomeEasy(false,1); // 1 adress de PC1
              delay(10);
              transmitHomeEasy(false,1); // 1 adress de PC1
          }
          if (strcasecmp(value,"PC1On")==0 ){
              transmitHomeEasy(true,1); // 1 adress de la PC1
              delay(10);
               transmitHomeEasy(true,1); // 1 adress de la PC1
          }
           if (strcasecmp(value,"PC2Off")==0){
              transmitHomeEasy(false,2); // 0 adress de la PC2
              delay(10);
              transmitHomeEasy(false,2); // 0 adress de la PC2
           }
          if (strcasecmp(value,"PC2On")==0 ){
              transmitHomeEasy(true,2); // 0 adress de la PC2
              delay(10);
              transmitHomeEasy(true,2); // 0 adress de la PC2
          }

          if (strcasecmp(value,"ouvertureT") ==0){
            UserAction =ActionOuvertureTotal;
            LogOnInternet(1,"ouverture_total_internet","system");
          }
          if (strcasecmp(value,"ouvertureP")==0 ){
            UserAction =ActionOuverturePartiel;
            LogOnInternet(1,"ouverture_partielle_internet","system");
          }
         if (strcasecmp(value,"ouvertureA")==0 ){
            UserAction =ActionOuvertureAeration;
            LogOnInternet(1,"ouverture_aeartion_internet","system");
          }
          if (strcasecmp(value,"fermeture") ==0  ){
            UserAction =ActionFermeture;
            LogOnInternet(1,"fermeture_internet","system");
          }
         if (strcasecmp(value,"stop") ==0  ){
            UserAction =ActionStop;
            CurrentState = StateArretUrgence;
            LogOnInternet(1,"stop_internet","system");

         }
         if (strcasecmp(value,"park") ==0  ){
              tpl2Park();     
          }
         
          if (strcasecmp(value,"unpark") ==0  ){
    //          UnPark();
        }
           if (strcasecmp(value,"poweroff")==0){
             tpl2Power(false);
           }
        }  
    } while (repeat);
    

    // after procesing the POST data, tell the web browser to reload
    // the page using a GET method. 
    server.httpSeeOther(PREFIX);
  

    return;
  }

  /* for a GET or HEAD, send the standard "it's all OK headers" */
 if (true) { //server.checkCredentials(AdminCredential)) {
  server.httpSuccess();
  /* we don't output the body for a HEAD request */
  if (type == WebServer::GET)
  {

    /* store the HTML in program memory using the P macro */
    P(message0) = 
    "<!DOCTYPE html>"
"<html><head>"
"  <script src='http://ajax.googleapis.com/ajax/libs/jquery/1.5/jquery.min.js'></script>"
"  <script src='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8/jquery-ui.min.js'></script>"
"  <link rel='stylesheet' href='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.18/themes/redmond/jquery-ui.css' />"
"  <style type='text/css'>"
"  	#schema {width:340px;height:200px;background-color:#BDBDBD; }"
"  	#moteur {width:340px;height:50px;background-color:#BDBDBD; }"
"  	.capteur, .etatMoteur { width:30px;height:30px;opacity:0.6;background-color:#6E6E6E; }"
"	.textClass { width:60px;height:30px;font-size:150%; }"
"	#statusLabel { width:160px; font-weight:bold}"
"  </style>"
"  <script>"
"       var statusTable = ['Toit fermé ','Tympan Ouverture intermedaire','Tympan ouverture totale','',"
"                           'Tympan fermeture','Toit fermeture','Toit ouverture','Toit ouvert','Arret Urgence',"
"                            'Tympan fermeture','Park Téléscope','Toit ouverture','Toit aeration'];"
"	function updateSchema(){"
"	    $.getJSON('/json/capteur',function(data){"
"	            $('#Telescope').css('background-color',((data.Telescope==0)?'red':'green'));"
"	            $('#ToitFerme').css('background-color',((data.ToitFerme==1)?'green':'#6E6E6E'));"
"	            $('#ToitOuvert').css('background-color',((data.ToitOuvert==1)?'green':'#6E6E6E'));"
"	            $('#TympanOuvert').css('background-color',((data.TympanOuvert==1)?'green':'#6E6E6E'));"
"	            $('#TympanFerme').css('background-color',((data.TympanFerme==1)?'green':'#6E6E6E'));"
"	            $('#TympanInter').css('background-color',((data.TympanIntermediaire==1)?'green':'#6E6E6E'));"
"	            $('#OuvertureTympan').css('background-color',((data.OuvertureTympan==1)?'red':'#6E6E6E'));"
"	            $('#FermetureTympan').css('background-color',((data.FermetureTympan==1)?'red':'#6E6E6E'));"
"	            $('#OuvertureToit').css('background-color',((data.OuvertureToit==1)?'red':'#6E6E6E'));"
"	            $('#FermetureToit').css('background-color',((data.FermetureToit==1)?'red':'#6E6E6E'));"
"                   $('#statusLabel').text(statusTable[data.CurrentState]);"
;
P(message1)=
"                   $('#poweroff').button(data.Telescope==1?'enable':'disable');"
"                   $('#ConnectTpl2').attr('checked',data.TelescopeTpl2Connected==1);"
"                   if (data.CurrentState==8) "
"                        $('#statusLabel').css('color','red');"
"                   else"
"                        $('#statusLabel').css('color','black');"
"                   if (data.TelescopeTpl2Connected==0) {"
"                          $('#fermeture,#ouvertureT,#ouvertureA,#ouvertureP').button('disable');"
"                  } else {"
"                      if ( data.Telescope==0 )"
"                           $('#ouvertureT,#ouvertureP,#ouvertureA').button('disable');"
"	                if(data.CurrentState==0) { "
"	            	    $('#fermeture').button('disable');"
"	            	    $('#ouvertureT,#ouvertureP,#ouvertureA').button('enable');"
"		        } else if (data.CurrentState==7) { "
"	          	    $('#fermeture').button('enable');"
"	            	    $('#ouvertureT,#ouvertureP,#ouvertureA').button('disable');"
"	        	} else if (data.CurrentState==12) {"
"                            $('#ouvertureT,#ouvertureP,#fermeture').button('enable');"
"                        }"
"	        	else  "
"	          	    $('#fermeture,#ouvertureT,#ouvertureP,#ouvertureA').button('disable');"
"                   if (data.CurrentState==8 || $('#security').is(':checked'))  "
"            	        $('#fermeture,#ouvertureT,#ouvertureP,#ouvertureA').button('enable');"
"                 }"
"	    });"
"	};";
P(message2)=
"  $(document).ready(function() {"
"    $('#tabs').tabs();"
"    $('button').button();"
"	$( '#lumiere,#PC1,#PC2').buttonset();"
"	$('#ToitFerme').position({my:'left top',at:'left top',of:'#schema'});"
"	$('#ToitOuvert').position({my:'right top',at:'right top',of:'#schema'});"
"	$('#TympanOuvert').position({my:'left bottom',at:'left bottom',of:'#schema'});"
"	$('#TympanIntermediaire').position({my:'left top',at:'left top',offset: '0 70',of:'#schema'});"
"	$('#TympanFerme').position({my:'left top',at:'left top',offset: '0 35',of:'#schema'});"
"	$('#Telescope').position({of:'#schema'});	"
"	$('#ToitLabel').position({my:'center top',at:'center top',of:'#schema'});"
"	$('#TympanLabel').position({my:'left top',at:'left center',offset :'0 20',of:'#schema'});"
"	$('#TelescopeLabel').position({offset:'0 35',of:'#schema'});"
"	$('#statusLabel').position({my:'left bottom',at:'left bottom',offset:'150 -20',of:'#schema'});"
"	$('#OuvertureTympan').position({my:'left',at :'left ',of :'#moteur'});"
"	$('#FermetureTympan').position({my:'left',at :'left ',offset:'60 0',of :'#moteur'});"
"	$('#OuvertureToit').position({my:'right',at :'right ',offset:'-60 0',of :'#moteur'});"
"	$('#FermetureToit').position({my:'right',at :'right ',of :'#moteur'});"  
"       if ($('#FermetureAuto').is(':checked'))"
"         $('#tableau').show();"
"       else"
"         $('#tableau').hide();"
"	$('button,#PC1On,#PC1Off,#PC2On,#PC2Off,#lumiereOff,#lumiereOn,#park,#unpark' ).click(function() {	"
"		var id = $(this).attr('id'); "
"		$.post('/', { action: id } );"
"	});"
"	$('#ConnectTpl2').click(function(){"
"               var status = $('#ConnectTpl2').is(':checked');"
"		$.get('/connectTelecsope',{Tpl2:status});"
"	});"
"	$('#FermetureAuto').click(function(){"
"               var status = $('#FermetureAuto').is(':checked');"
"		$.get('/connectTelecsope',{FerAuto:status});"
"              if (status) "
"			$('#tableau').show();"
"		else"
"			$('#tableau').hide();"
"	});"
"  });";
P(message3) = 
"setInterval('updateSchema();',1000); "
"  </script>"
"</head><body style='font-size:62.5%;'>"
"<div id='tabs'>"
"    <ul>"
"        <li><a href='#fragment-2'><span>Toit</span></a></li>"
"	 <li><a href='#fragment-0'><span>Telescope</span></a></li>"
"	 <li><a href='#fragment-1'><span>Fermeture auto</span></a></li>"
"        <li><a href='#fragment-3'><span>Lumiere</span></a></li>"
"        <li><a href='#fragment-4'><span>PC</span></a></li>"
"    </ul>"
"    <div id='fragment-2'>"
"    	<div id ='schema'>"
"    	    <div class = 'capteur' id= 'ToitFerme'> </div>"
"    		<div class = 'capteur' id= 'ToitOuvert'></div> "
"    		<div class = 'capteur' id= 'TympanOuvert'></div>"
"    		<div class = 'capteur' id= 'TympanInter'></div>"
"    		<div class = 'capteur' id= 'TympanFerme'></div>"
"    		<div class = 'capteur' id= 'Telescope'></div>"
"    		<div class = 'textClass' id ='ToitLabel'> Toit </div>"
"    		<div class = 'textClass' id ='TympanLabel'> Tympan </div>"
"    		<div class = 'textClass' id ='TelescopeLabel'> Telescope </div>"
"		<div class = 'textClass' id ='statusLabel'> status </div>"
"    	</div>"
"    	<b>Etat Moteurs</b>"
"    	<div id = 'moteur'>"
"    		<div class = 'etatMoteur' id ='OuvertureTympan'> O</div>"
"    		<div class = 'etatMoteur' id ='FermetureTympan'> F</div>"
"    		<div class = 'etatMoteur' id ='OuvertureToit'> O</div>"
"    		<div class = 'etatMoteur' id ='FermetureToit'> F</div>"
"    	</div>"
"		<button id ='ouvertureT'> Ouverture total</button>"
"		<button id= 'ouvertureP'> Ouverture partielle</button>"
"		<button id= 'ouvertureA'> Ouverture aeration</button>"
"		<br><button id='fermeture'> Fermeture</button>"
"		<button id='stop'> Stop!</button>"
"    </div>";
P(message4)=
"    <div id='fragment-3'>"
"		<div id='lumiere'> "
"		Lumiere 1  <input type='radio' name = 'lumiere' id='lumiereOn'  /><label for='lumiereOn'>Allume</label>"
"		<input type='radio' name = 'lumiere' id='lumiereOff' checked='checked' /><label for='lumiereOff'>Eteindre</label>"
"		</div>   "
"	</div>"
"    <div id='fragment-4'>"
"		<div  id='PC1'>"
"			PC astelco  <input type='radio' name = 'PC1' id='PC1On'  /><label for='PC1On'>Allume</label>"
"			<input type='radio'  name = 'PC1' id='PC1Off' checked='checked' /><label for='PC1Off'>Eteindre</label>"
"		</div>   "
"		<div id='PC2'>"
"			PC aquisition  <input type='radio' name = 'PC2'id='PC2On'  /><label for='PC2On'>Allume</label>"
"			<input type='radio' name = 'PC2' id='PC2Off' checked='checked' /><label for='PC2Off'>Eteindre</label>"
"		</div>   "
"        ByPass Security : <input type ='checkbox', id = 'security'>"
"	</div>"
"	<div id='fragment-0'>"
"        <div>"
"			Connect TPL2 : <input type ='checkbox' id = 'ConnectTpl2'";
P(message5) = 
"		 </div>"
"		<button id='park'> park </button>"
"		<button id='poweroff'> NTM power off </button>"
"       </div>"
"    <div id='fragment-1'>"
"        <div>"
"			<input type ='checkbox'  id = 'FermetureAuto' ";
P(message51)=
">Fermeture auto"
"			<div id = 'tableau' >&nbsp &nbsp &nbsp &nbsp Si une des conditions est vrai => Park du telescope et fermeture du toit<br><br>"
"			<form method='post' action ='/FermetureAuto' ><table>"
"				<tr><td><input type ='checkbox'  name = 'ObscurityCheck' ";
P(message6)=
">Obscurite</td><td><</td><td><input name=ObscurityValue size=3 value='";
P(message7)=
"'/>mag/arcsec2</td></tr>"
"				<tr><td><input type ='checkbox'  name = 'ClarityCheck' ";
P(message8)=
">Clarity</td><td>></td><td><input name=ClarityValue size=3 value= '";
P(message9)=
"'/>degres</td></tr>"
"				<tr><td><input type ='checkbox'  name = 'SkyTempCheck'";
P(message10)=
">SkyTemp</td><td>></td><td><input name=SkyTempValue size=3 value = '";
P(message11)=
"'/>degres</td></tr>"
"				<tr><td><input type ='checkbox'  name = 'PluieCheck' ";
P(message12)=
">Pluie</td><td>></td><td><input name=PluieValue size=3 value='";
P(message13)=
"'/></td></tr>"
"				<tr><td><input type ='checkbox'  name = 'HumCheck'";
P(message14)=
">Humidite</td><td>></td><td><input name=HumValue size=3 value ='";
P(message15)=
"'/>%</td></tr>"
"				<tr colspan=3><td ><button id='valider' type='submit'> Valider </button></td></tr>"
"			</table> </form></div>"
"	 </div>"
"       </div>"
"</div></body></html>"
;
    server.printP(message0);
    wdt_reset();   //pat the dog :)
    server.printP(message1);
    wdt_reset();   //pat the dog :)
    server.printP(message2);
    wdt_reset();   //pat the dog :)
    server.printP(message3);
    wdt_reset();   //pat the dog :)
    server.printP(message4);
    if (TelescopeTpl2Connected)
      server.println("checked='checked' >");
    else
      server.println(">");
     server.printP(message5);
    if (FermetureAuto)
      server.print("checked='checked'");
     server.printP(message51);
    if (meteoValues.DarknessCheck)
      server.print("checked='checked'");
    server.printP(message6);
    server.print(meteoValues.DarknessTheshold);
    server.printP(message7);
    if (meteoValues.ClarityCheck)
      server.print("checked='checked'");
    server.printP(message8);
    server.print(meteoValues.ClarityThreshold);
    server.printP(message9);
    if (meteoValues.SkyTempCheck)
      server.print("checked='checked'");
    server.printP(message10);
    server.print(meteoValues.SkyTempThreShold);
    server.printP(message11);
    if (meteoValues.RainCheck)
      server.print("checked='checked'");
    server.printP(message12);
    server.print(meteoValues.RainThreshold);
    server.printP(message13);
    if (meteoValues.HumidityCheck)
      server.print("checked='checked'");
    server.printP(message14);
    server.print(meteoValues.HumidityThresold);
    server.printP(message15);

  }
}

else
{
    /* send a 401 error back causing the web browser to prompt the user for credentials */
    server.httpUnauthorized();
  }
}

