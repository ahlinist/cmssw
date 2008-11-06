<html lang="en">

<head>
  <meta http-equiv="content-type" content="text/html; charset=iso-8859-15">
  <meta name="author" content="Pietro M. Picca, Pietro.Picca@cern.ch">
  <title>
    <?php print(($title = (strlen(trim($caption)) > 0? $caption: "PopCon activity recorded on ".strtoupper($svr = (empty($_GET['svr'])? "cms_orcoff_prod": strtolower($_GET['svr'])))))."\n"); ?>
  </title>
  <script language="javascript">
    function see(dest, tc) {
      if(tc == "hourly") sizes = 'width=820, height=475';
      else sizes = 'width=1000, height=425';
      var nwin = window.open(dest, tc, 'resizable=yes, menubar=no, toolbar=yes, scrollbars=yes, locations=no, status=yes, ' + sizes);
      nwin.focus();
    }
  </script>
  <style type="text/css">
    @import url(cmspietro.css);
  </style>
</head>

<body onLoad="window.defaultStatus = document.title;">

<script language="php">
  set_time_limit(0);
  require("../private/support.inc");
  print("<p class=\"aq\">\n  $title\n</p>\n\n");
  print("<p>\n  <button onClick=\"javascript:see('popcongraph.php?tc=hourly&svr=$svr&app=old&yts=5&rts=10', 'hourly');\">Show activity during the last 24 hours</button>\n");
  print("  <button onClick=\"javascript:see('popcongraph.php?tc=daily&svr=$svr&app=old&yts=5&rts=10', 'daily');\">Show activity during the last 30 days</button>\n</p>\n\n<br>\n\n");
  if($conn = @ocilogon($usr, $pwd, $tns)) {
    ociexecute($stmt = ociparse($conn, "select payload_name, connection_string, to_char(execution_time, 'dd/mm/yyyy hh24:mi:ss') as execution_time, status from $usr.p_con_web_show"));
    if(($nrows = ocifetchstatement($stmt, $results)) > 0) {
      print("<table class=\"p2k\">\n");
      print("  <tr>\n");
      print("    <th class=\"p2k\">\n      ID\n    </th>\n");
      while(list($key, $val) = each($results)) print("    <th class=\"p2k\">\n      <nobr>".str_replace("_", " ", $key)."</nobr>\n    </th>\n");
      print("    <th class=\"p2k\">\n      ACTIVITY\n    </th>\n");
      print "  </tr>\n";
      for($i = 0; $i < $nrows; $i++) {
        print("  <tr valign=\"center\">\n");
        print("    <td class=\"rg\">\n      ".($i + 1).".\n    </td>\n");
        print("    <td class=\"p2k\">\n      <b>".$results['PAYLOAD_NAME'][$i]."</b>\n    </td>\n");
        print("    <td class=\"p2k\">\n      ".$results['CONNECTION_STRING'][$i]."\n    </td>\n");
        print("    <td class=\"rg\">\n      ".$results['EXECUTION_TIME'][$i]."\n    </td>\n");
        print("    <td class=\"p2k\">\n      ".$results['STATUS'][$i]."\n    </td>\n");
        print("    <td class=\"p2k\">\n      <nobr>\n        <a href=\"javascript:see('popcongraph.php?tc=hourly&svr=$svr&app=old&yts=5&rts=10&det=".($acnt = strtolower(substr($acnt = strrchr($results['CONNECTION_STRING'][$i], "//"), 1)))."', 'hourly');\" class=\"p2k\">Hourly</a>\n        <a href=\"javascript:see('popcongraph.php?tc=daily&svr=$svr&app=old&yts=5&rts=10&det=$acnt', 'daily');\" class=\"p2k\">Daily</a>\n      <nobr>\n    </td>\n");
        print("  </tr>\n");
      }
      print("</table>\n");
    }
    else print("<p class=\"bo\">\n  No PopCon activity recorded!\n</p>\n");
    ocifreestatement($stmt);
    ocilogoff($conn);
  }
  else oraconnecterror();
</script>

<p class="bo">
  This is the old PopCon web log interface. Click
  <a href="popconact.php" class="p2k">here</a>
  to point to the new one.
</p>

<p class="bo">
  This page has been dynamically generated on
  <?php print(newdate(time(), "eng")." at ".date("G:i", time()).", Central European time."); ?>
  Created on January 29<sup>th</sup>, 2008, its source code has been last updated on October 24<sup>th</sup>, 2008.
  <br>
  For any suggestion or comment, please write to
  <?php print("<a href=\"mailto:Pietro M. Picca <Pietro.Picca@cern.ch>?subject=$title\" class=\"p2k\">Pietro.Picca@cern.ch</a>"); ?>.
</p>

</body>

</html>