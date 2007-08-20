<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<%@page import="java.io.PrintWriter"%>
<%@page import="java.io.ByteArrayOutputStream"%>
<%@page import="confdb.converter.DbProperties"%>
<%@page import="confdb.db.ConfDBSetups"%>
<%@page import="confdb.converter.Converter"%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>HLT config</title>

<script type="text/javascript" src="../js/yui/yahoo/yahoo.js"></script>
<script type="text/javascript" src="../js/yui/utilities/utilities.js"></script>
<script type="text/javascript" src="../js/yui/event/event.js"></script>

<style type="text/css">

body {
	margin:0;
	padding:0;
<%
  String background = request.getParameter( "bgcolor" );
  if ( background != null )
	  out.println( "background:#" + background + ";" );
%>
}

</style>


<script type="text/javascript">

function signalReady()
{
  if ( parent &&  parent.iframeReady )
    parent.iframeReady();
}

//YAHOO.util.Event.onDOMReady( signalReady );

 </script>

</head>

<body onload="signalReady()">
<pre><code>
<%
  try {
	Converter converter = Converter.getConverter();
	int configKey = Integer.parseInt( request.getParameter( "configKey" ) );
	String index = request.getParameter( "dbIndex" );
	if ( index != null )
	{
		int dbIndex = Integer.parseInt( index );
		ConfDBSetups dbs = new ConfDBSetups();
	    DbProperties dbProperties = new DbProperties( dbs, dbIndex, "convertme!" );
    	dbProperties.setDbUser( "cms_hlt_reader" );
    	converter.setDbProperties( dbProperties );
	}

	String confStr = converter.readConfiguration(configKey);
	if ( confStr == null )
	  out.print( "ERROR!\nconfig " + configKey + " not found!" );
	else
	  out.print( confStr );
  } catch ( Exception e ) {
	  out.print( "ERROR!\n\n" ); 
	  ByteArrayOutputStream buffer = new ByteArrayOutputStream();
	  PrintWriter writer = new PrintWriter( buffer );
	  e.printStackTrace( writer );
	  writer.close();
	  out.println( buffer.toString() );
  }
%>
</code></pre>
</body>
</html>

