<html lang="en">

<head>
  <meta http-equiv="content-type" content="text/html; charset=iso-8859-15">
  <meta name="author" content="Pietro M. Picca, Pietro.Picca@cern.ch">
  <meta http-equiv="refresh" content="300;url=<?php print($_SERVER["REQUEST_URI"]); ?>">
  <title>
    <?php print(($title = "PopCon cronjob tail fetcher")."\n"); ?>
  </title>
  <style type="text/css">
    @import url(cmspietro.css);
  </style>
  <script language="JavaScript">
    <!--
    var shortHTML = new Array();
    var ltails = new Array();
    function expand(obj) {
      shortHTML[obj.id] = obj.innerHTML;
      if(obj.id in ltails) obj.innerHTML = ltails[obj.id];
      else obj.innerHTML = document.getElementById(obj.id.replace("stail", "ltail")).innerHTML;
      
    }
    function shrink(obj) {
      ltails[obj.id] = obj.innerHTML;
      if(obj.id in shortHTML) obj.innerHTML = shortHTML[obj.id];
      else obj.innerHTML = document.getElementById(obj.id.replace("ltail", "stail")).innerHTML;
    }
    function convert(d) {
      var dd = new Date(d);
      var months = new Array("January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December");
      var rc = "<i>UTC</i>: " + months[dd.getUTCMonth()] + " " + dd.getUTCDate();
      switch(dd.getUTCDate()) {
        case 1: case 21: case 31: rc += "<sup>st</sup>"; break;
        case 2: case 22: rc += "<sup>nd</sup>"; break;
        case 3: case 23: rc += "<sup>rd</sup>"; break;
        default: rc += "<sup>th</sup>";
      }
      rc += ", " + dd.getUTCFullYear() + " " + dd.getUTCHours() + ":";
      if(dd.getUTCMinutes() < 10) rc += "0";
      rc += dd.getUTCMinutes() + ":";
      if(dd.getUTCSeconds() < 10) rc += "0";
      rc += dd.getUTCSeconds() + " - <i>Local time</i>: " + months[dd.getMonth()] + " " + dd.getDate();
      switch(dd.getDate()) {
        case 1: case 21: case 31: rc += "<sup>st</sup>"; break;
        case 2: case 22: rc += "<sup>nd</sup>"; break;
        case 3: case 23: rc += "<sup>rd</sup>"; break;
        default: rc += "<sup>th</sup>";
      }
      rc += ", " + dd.getFullYear() + " " + dd.getHours() + ":";
      if(dd.getMinutes() < 10) rc += "0";
      rc += dd.getMinutes() + ":";
      if(dd.getSeconds() < 10) rc += "0";
      return(rc + dd.getSeconds());
    }
    // -->
  </script>
</head>

<body onLoad="window.defaultStatus = document.title;">

<script language="php">
  set_time_limit(0);
  $svr = (empty($_GET['svr'])? "cms_orcoff_prod": strtolower($_GET['svr']));
  require("../private/support.inc");
  print("<p class=\"aq\">\n  $title\n</p>\n\n");
  if($conn = @ocilogon($usr, $pwd, $tns)) {
    ociexecute($stmt = ociparse($conn, "select filename, 86400000 * (cast(sys_extract_utc(crontime) as date) - to_date('01-01-1970','DD-MM-YYYY')) as crontime, short_tail, long_tail from $usr.logtails order by crontime desc"));
    if(($nrows = ocifetchstatement($stmt, $results)) > 0) {
      print("<table class=\"p2k\">\n");
      print("  <tr>\n");
      print("    <th class=\"pl\" align=\"right\">\n      id\n    </th>\n");
      print("    <th class=\"pl\" align=\"left\">\n      file name + timestamp + tail\n    </th>\n");
      print("  </tr>\n");
      for($i = 0; $i < $nrows; $i++) {
        print("  <tr valign=\"center\">\n");
        print("    <td class=\"p2k\" rowspan=\"3\" align=\"right\">\n      ".($i + 1).".\n    </td>\n");
        print("    <td class=\"p2k\">\n      <b>\n        ".$results['FILENAME'][$i]."\n      </b>\n    </td>\n");
        print("  </tr>\n  <tr>\n");
        print("    <td class=\"p2k\">\n      <script language=\"JavaScript\">\n        <!--\n        document.write(convert(".$results['CRONTIME'][$i]."));\n        // -->\n      </script>\n    </td>\n");
        print("  </tr>\n  <tr>\n");
        print("    <td class=\"p2k\">\n      <div id=\"stail".str_pad($i + 1, 3, "0", STR_PAD_LEFT)."\"".($flag = ($_GET['mask']{$i} == "1")? " class=\"inv\"": "").">\n        <pre>".$results['SHORT_TAIL'][$i]."</pre>\n        <button onClick=\"expand(this.parentNode);\">\n          expand ".($i + 1)."\n        </button>\n      </div>\n");
        print("      <div id=\"ltail".str_pad($i + 1, 3, "0", STR_PAD_LEFT)."\"".(!$flag? " class=\"inv\"": "").">\n        <textarea readonly=\"readonly\" wrap=\"off\">".$results['LONG_TAIL'][$i]."</textarea>\n        <br>\n        <br>\n        <button onClick=\"shrink(this.parentNode);\">\n          shrink ".($i + 1)."\n        </button>\n      </div>\n    </td>\n");
        print("  </tr>\n");
      }
      print("</table>\n");
    }
    else print("<p class=\"bo\">\n  No PopCon cronjob tails recorded!\n</p>\n");
    ocifreestatement($stmt);
    ocilogoff($conn);
  }
  else oraconnecterror();
</script>

<p class="bo">
  This page has been dynamically generated on
  <?php print(newdate(time(), "eng")." at ".date("G:i", time()).", Central European time."); ?>
  Created on September 20<sup>th</sup>, 2008, its source code has been last updated on October 25<sup>th</sup>, 2008.
  <br>
  For any suggestion or comment, please write to
  <?php print("<a href=\"mailto:Pietro M. Picca <Pietro.Picca@cern.ch>?subject=$title\" class=\"p2k\">Pietro.Picca@cern.ch</a>"); ?>.
</p>

</body>

</html>