<html lang="en">

<head>
  <meta http-equiv="content-type" content="text/html; charset=iso-8859-15">
  <meta name="author" content="Pietro M. Picca, Pietro.Picca@cern.ch">
  <title>
    <?php print(($title = "Monitor tool for ".strtoupper($svr = (empty($_GET['svr'])? "cmsr": $_GET['svr']))." developers objects")."\n"); ?>
  </title>
  <script language="javascript">
    function see(dest) {
      var nwin = window.open(dest, 'show', 'resizable=yes, menubar=no, toolbar=yes, scrollbars=yes, locations=no, status=yes');
      nwin.focus();
    }
  </script>
  <style type="text/css">
    @import url(cmspietro.css);
  </style>
</head>

<body onLoad="window.defaultStatus = document.title;">

<p class="aq">
  <?php print("$title\n"); ?>
</p>

<script language="php">
  set_time_limit(0);
  $sel = (ereg("^[0-9]{2}$|^[0-9]{6}$", $_GET['sel'])? $_GET['sel']: "now");
  $tc = (strlen($sel) == 6? "daily": "hourly");
  $apl = "mon";
  require("../private/support.inc");
  if($log == "yes") {
    $conn = ocilogon($usr, $pwd, $tns);
    require("../private/geoip.inc");
    $gi = geoip_open("geoip.dat", GEOIP_STANDARD);
      ociexecute(ociparse($conn, "insert into $usr.mon_log (domain, ip, country, browser) values ('".strtolower(gethostbyaddr("$REMOTE_ADDR"))."', '$REMOTE_ADDR', '".geoip_country_name_by_addr($gi, $REMOTE_ADDR)."', '$HTTP_USER_AGENT')"));
      ocicommit($conn);
    geoip_close($gi);
    ocilogoff($conn);
  }
  $svr = (empty($_GET['svr'])? "cmsr": $_GET['svr']);
  $apl = ($sel == "now"? "mon": "hist");
  require("../private/auth.inc");
  $conn = ocilogon($usr, $pwd, $tns);
  ociexecute($stmt = ociparse($conn, $sel == "now"? "select * from $scm.cond_occupancy": "select * from $scm.hs_cond_occupancy_$tc where id_snapshot = '$sel' order by username"));
  $nrows = ocifetchstatement($stmt, $results);
  print("<table class=\"p2k\">\n");
  print("  <tr>\n");
  print("    <th class=\"p2k\">\n      USERNAME\n    </th>\n");
  print("    <th class=\"p2k\">\n      ACTIVITY\n    </th>\n");
  print("    <th class=\"p2k\">\n      TABLESPACE\n    </th>\n");
  print("    <th class=\"p2k\">\n      OCCUPANCY\n    </th>\n");
  print("    <th class=\"p2k\">\n      QUOTA\n    </th>\n");
  print "  </tr>\n";
  for($i = 0; $i < $nrows; $i++) {
    print("  <tr>\n");
    print("    <td class=\"p2k\">\n      <a href=\"javascript:see('detdetails.php?svr=$svr&sel=$sel&det=".($t = $results['USERNAME'][$i])."');\" class=\"p2k\">$t</a>\n    </td>\n");
    print("    <td class=\"md\">\n      ".(ereg("_R$", $t)? "-": "<a href=\"javascript:see('detactivity.php?svr=$svr&sel=$sel&det=$t');\" class=\"p2k\"><img src=\"show.gif\" border=\"0\"></a>")."\n    </td>\n");
    print("    <td class=\"p2k\">\n      ".$results['TABLESPACE'][$i]."\n    </td>\n");
    print("    <td class=\"rg\">\n      ".format_number($results['OCCUPANCY'][$i], 2)."\n    </td>\n");
    print("    <td class=\"rg\">\n      ".(($q = $results['QUOTA'][$i]) == -1? "Unlimited space": ($q == 0? "No space": format_number($q, 2)))."\n    </td>\n");
    print("  </tr>\n");
  }
  print("</table>\n");
  ocifreestatement($stmt);
  ocilogoff($conn);
</script>

<p class="bo">
  By clicking on the username, a page showing all the object contained in the relative schema will be opened.
  <br>
  This page has been dynamically generated on
  <?php print(newdate(time(), "eng")." at ".date("G:i", time()).", Central European time."); ?>
  Created on August 3<sup>rd</sup>, 2007, its source code has been last updated on November 14<sup>th</sup>, 2008.
  <br>
  For any suggestion or comment, please write to
  <?php print("<a href=\"mailto:Pietro M. Picca <Pietro.Picca@cern.ch>?subject=$title\" class=\"p2k\">Pietro.Picca@cern.ch</a>"); ?>.
</p>

</body>

</html>