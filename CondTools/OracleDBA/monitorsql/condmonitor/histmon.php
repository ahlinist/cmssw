<html lang="en">

<head>
  <meta http-equiv="content-type" content="text/html; charset=iso-8859-15">
  <meta name="author" content="Pietro M. Picca, Pietro.Picca@cern.ch">
  <title>
    <?php print(($title = "List of available snapshots for condition accounts on ".strtoupper($svr))."\n"); ?>
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
  print("<table class=\"p3k\">\n  <tr>\n    <td class=\"p2k\" colspan=\"2\">      1. Current situation\n    </td>\n    <td class=\"p2k\">\n      <button onClick=\"javascript:location.href = 'mon.php?svr=$svr&sel=now';\">show</button>\n    </td>\n");
  $apl = "hist";
  require("../private/support.inc");
  $conn = ocilogon($usr, $pwd, $tns);
  ociexecute($stmt = ociparse($conn, "select distinct id_snapshot, case when to_number(to_char(sysdate, 'HH24')) >= to_number(id_snapshot) then 'Today' else 'Yesterday' end || ' at ' || id_snapshot || ':00' as label from $scm.hs_cond_occupancy_hourly order by case when to_number(to_char(sysdate, 'HH24')) >= to_number(id_snapshot) then to_number(id_snapshot) + 24 else to_number(id_snapshot) end desc"));
  $nrows = ocifetchstatement($stmt, $results);
  print("  <tr>\n    <td class=\"p2k\">\n      2. By hour (last 24 hours)\n    </td>\n    <td class=\"p2k\">\n      <select id=\"hh\">\n");
  for($i = 0; $i < $nrows; $i++) print("        <option value=\"".$results['ID_SNAPSHOT'][$i]."\"".($i == 0? " selected=\"selected\"": "").">\n          ".$results['LABEL'][$i]."\n        </option>\n");
  print("      </select>\n    </td>\n    <td class=\"p2k\">\n      <button onClick=\"javascript:location.href = 'mon.php?svr=$svr&sel=' + document.getElementById('hh').value;\">show</button>\n    </td>\n");
  ocifreestatement($stmt);
  ociexecute($stmt = ociparse($conn, "select distinct id_snapshot, to_char(to_date(id_snapshot, 'YYMMDD'), 'FMDay DD Month YYYY') as label from $scm.hs_cond_occupancy_daily order by id_snapshot desc"));
  $nrows = ocifetchstatement($stmt, $results);
  print("  <tr>\n    <td class=\"p2k\">\n      3. By day\n    </td>\n    <td class=\"p2k\">\n      <select id=\"dd\">\n");
  for($i = 0; $i < $nrows; $i++) print("        <option value=\"".$results['ID_SNAPSHOT'][$i]."\"".($i == 0? " selected=\"selected\"": "").">\n          ".$results['LABEL'][$i]."\n        </option>\n");
  print("      </select>\n    </td>\n    <td class=\"p2k\">\n      <button onClick=\"javascript:location.href = 'mon.php?svr=$svr&sel=' + document.getElementById('dd').value;\">show</button>\n    </td>\n  </tr>\n</table>\n");
  ocifreestatement($stmt);
  ocilogoff($conn);
</script>

<p class="bo">
  This page has been dynamically generated on
  <?php print(newdate(time(), "eng")." at ".date("G:i", time()).", Central European time."); ?>
  Created on November 13<sup>th</sup>, 2007, its source code has been last updated on November 14<sup>th</sup>, 2008.
  <br>
  For any suggestion or comment, please write to
  <?php print("<a href=\"mailto:Pietro M. Picca <Pietro.Picca@cern.ch>?subject=$title\" class=\"p2k\">Pietro.Picca@cern.ch</a>"); ?>.
</p>

</body>

</html>