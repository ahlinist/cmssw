<html lang="en">

<head>
  <meta http-equiv="content-type" content="text/html; charset=iso-8859-15">
  <meta name="author" content="Pietro M. Picca, Pietro.Picca@cern.ch">
  <title>
    <?php print(($title = "Recent activity for $det subdetector on ".strtoupper($svr))."\n"); ?>
  </title>
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
  $svr = (empty($_GET['svr'])? "cmsr": $_GET['svr']);
  $apl = ($sel == "now"? "mon": "hist");
  require("../private/support.inc");
  $conn = ocilogon($usr, $pwd, $tns);
  ociexecute($stmt = ociparse($conn, $sel == "now"? "select what as statement, to_char(when, 'FMDay DD Month YYYY') || to_char(when, ' HH24:MI') as \"DATE & TIME\" from $scm.server_workload where who = '$det'": "select what as statement, to_char(when, 'FMDay DD Month YYYY') || to_char(when, ' HH24:MI') as \"DATE & TIME\" from $scm.hs_server_workload_$tc where id_snapshot = '$sel' and who = '$det' order by when desc"));
  $nrows = ocifetchstatement($stmt, $results);
  if($nrows > 0) {
    print("<table class=\"p2k\">\n");
    print("  <tr>\n");
    print("    <th class=\"p2k\">\n      ID\n    </th>\n");
    while(list($key, $val) = each($results)) print("    <th class=\"p2k\">\n      <nobr>\n        ".str_replace("_", " ", $key)."\n      </nobr>      \n    </th>\n");
    print "  </tr>\n";
    for($i = 0; $i < $nrows; $i++) {
      reset($results);
      print("  <tr>\n");
      print("    <td class=\"rg\">\n      ".($i + 1).".\n    </td>\n");
      while(list($key, $val) = each($results)) {
        print("    <td class=\"p2k\">\n      $val[$i]\n    </td>\n");
      }
      print("  </tr>\n");
    }
    print("</table>\n");
  }
  else print("<p class=\"bo\">\n  No activity recently recorded for $det subdetector!\n</p>\n");
  ocifreestatement($stmt);
  ocilogoff($conn);
</script>

<p class="bo">
  Here you will find only recently executed SQL statements, still present in the server memory in the moment when the snapshot was taken.
  <br>
  This page has been dynamically generated on
  <?php print(newdate(time(), "eng")." at ".date("G:i", time())."."); ?>
  For any suggestion or comment, please write to
  <?php print("<a href=\"mailto:Pietro M. Picca <Pietro.Picca@cern.ch>?subject=$title\" class=\"p2k\">Pietro.Picca@cern.ch</a>"); ?>.
</p>

<button onClick="self.close();">Close me</button>

</body>

</html>