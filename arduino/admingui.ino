
void HtmlAdminCmd(WebServer &server, WebServer::ConnectionType type, char *url, bool)
{
  

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
      if (strcasecmp(name,"actionManu") ==0) {
          if (strcasecmp(value,"TympanOuverture")==0) {
              Serial << "Ouverture Manuel Tympan \n";
              digitalWrite(relaisOuvertureTympan,HIGH);
              delay(1000);
              digitalWrite(relaisOuvertureTympan,LOW);
          }
          if (strcasecmp(value,"TympanFermeture")==0 ){
              Serial << "Fermeture Manuel Tympan \n";
              digitalWrite(relaisFermetureTympan,HIGH);
              delay(1000);
              digitalWrite(relaisFermetureTympan,LOW);
 
          }
          if (strcasecmp(value,"ToitOuverture")==0){
              Serial << "Ouverture Manuel Toit \n";
              digitalWrite(relaisOuvertureToit,HIGH);
              delay(1000);
              digitalWrite(relaisOuvertureToit,LOW);
 
          }
          if (strcasecmp(value,"ToitFermeture")==0 ){
              Serial << "Fermeture Manuel Toit \n";
               digitalWrite(relaisFermetureToit,HIGH);
              delay(1000);
              digitalWrite(relaisFermetureToit,LOW);
 
          }
          if (strcasecmp(value,"Fermeture")==0 ){
              Serial << "Fermeture complete sans park \n";
               UserAction=ActionFemetureSansPark;
          }
      }
    } while (repeat);
    
    // after procesing the POST data, tell the web browser to reload
    // the page using a GET method. 
    server.httpSeeOther(PREFIX);
  

    return;
  }

  /* for a GET or HEAD, send the standard "it's all OK headers" */
 if (server.checkCredentials(AdminCredential)) {
  server.httpSuccess();

  /* we don't output the body for a HEAD request */
  if (type == WebServer::GET)
  {
    /* store the HTML in program memory using the P macro */
    P(message) = 
    "<!DOCTYPE html>"
"<html><head>"
"  <script src='http://ajax.googleapis.com/ajax/libs/jquery/1.5/jquery.min.js'></script>"
"  <script src='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8/jquery-ui.min.js'></script>"
"  <link rel='stylesheet' href='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.18/themes/redmond/jquery-ui.css' />"
"  <script>"
"  $(document).ready(function() {"
"    $('button').button();"
"	$('#TympanOuverture,#TympanFermeture,#ToitOuverture,#ToitFermeture' ).click(function() {	"
"		var id = $(this).attr('id'); "
"		$.post('/admin', { actionManu: id } );"
"	});"
"	$('#Fermeture' ).click(function() {	"
"		var id = $(this).attr('id'); "
"               var answser = confirm('Fermeture sans park du telescope ! Verifier que le teslescope est bien en position park');"
"               if (answser) "
"                  $.post('/admin', { actionManu: id } );"
"	});"
"  });"
"</script>"
"</head><body style='font-size:62.5%;'>"
"	<h1>Action manuelle , impulsion de 1 seconde </h1>"
"    <div >"
"    		<div><button id='TympanOuverture'> Tympan ouverture </button></div>"
"    		<div><button id='TympanFermeture'> Tympan fermeture </button></div>"
"    		<div><button id='ToitOuverture'> Toit ouverture </button></div>"
"    		<div><button id='ToitFermeture'> Toit fermeture </button></div>"
"               <br><br><br><br>"
"    		<div><button id='Fermeture'> Toit fermeture sans park !! </button></div>"
"    </div>"
"</body></html>"
;
    server.printP(message);
  }
}

else
{
    /* send a 401 error back causing the web browser to prompt the user for credentials */
    server.httpUnauthorized();
  }
}
