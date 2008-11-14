<html lang="en">

<head>
  <meta http-equiv="content-type" content="text/html; charset=iso-8859-15">
  <meta name="author" content="Pietro M. Picca, Pietro.Picca@cern.ch">
  <title>
    <?php print(($title = "Details for $det subdetector on ".strtoupper($svr))."\n"); ?>
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
  ociexecute($stmt = ociparse($conn, $sel == "now"? "select object, type, occupancy, status, tablespace, last_modified from $scm.det_details where owner = '$det'": "select object, type, occupancy, status, tablespace, last_modified from $scm.hs_det_details_$tc where id_snapshot = '$sel' and owner = '$det' order by type, object"));
  $nrows = ocifetchstatement($stmt, $results);
  if($nrows > 0) {
    $ptrn = "";
    print("<table class=\"p2k\">\n");
    print("  <tr>\n");
    print("    <th class=\"p2k\">\n      ID\n    </th>\n");
    while(list($key, $val) = each($results)) print("    <th class=\"p2k\">\n      ".str_replace("_", " ", $key)."\n    </th>\n");
    print "  </tr>\n";
    for($i = 0; $i < $nrows; $i++) {
      reset($results);
      print("  <tr>\n");
      print("    <td class=\"rg\">\n      ".($i + 1).".\n    </td>\n");
      while(list($key, $val) = each($results)) {
        if($key == "OCCUPANCY") print("    <td class=\"rg\">\n      ".(($t = $val[$i]) == 0? "Not determinable": format_number($t, 2))."\n    </td>\n");
        elseif($key == "TYPE") {
          if($ptrn == $val[$i]) print("    <td class=\"p2k\">\n      $ptrn\n    </td>\n");
          else print("    <td class=\"gr\">\n      ".($ptrn = $val[$i])."\n    </td>\n");
        }
        else print("    <td class=\"p2k\">\n      $val[$i]\n    </td>\n");
      }
      print("  </tr>\n");
    }
    print("</table>\n");
  }
  else print("<p class=\"bo\">\n  No object owned by $det subdetector!\n</p>\n");
  ocifreestatement($stmt);
  ocilogoff($conn);
</script>

<p class="bo">
  A date printed in red means that the corresponding object has not been modified for the latest year. The first object of each type is marked in green.
  <br>
  This page has been dynamically generated on
  <?php print(newdate(time(), "eng")." at ".date("G:i", time())."."); ?>
  For any suggestion or comment, please write to
  <?php print("<a href=\"mailto:Pietro M. Picca <Pietro.Picca@cern.ch>?subject=$title\" class=\"p2k\">Pietro.Picca@cern.ch</a>"); ?>.
</p>

<button onClick="self.close();">Close me</button>

</body>

</html>