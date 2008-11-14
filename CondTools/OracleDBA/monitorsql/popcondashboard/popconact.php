<html lang="en">

<head>
  <meta http-equiv="content-type" content="text/html; charset=iso-8859-15">
  <meta name="author" content="Pietro M. Picca, Pietro.Picca@cern.ch">
  <title>
    PopCon dashboard
  </title>
  <script language="JavaScript" src="calendar.js"></script>
  <script language="JavaScript">
    <!--
    String.prototype.trim = function() {
      return this.replace(/^\s+|\s+$/g, "");
    }
    function selall(e) {
      for(var i = 0; i < e.length && e[i].type == "checkbox"; i++) e[i].checked = true;
    }
    function deselall(e) {
      for(var i = 0; i < e.length && e[i].type == "checkbox"; i++) e[i].checked = false;
    }
    function invsel(e) {
      for(var i = 0; i < e.length && e[i].type == "checkbox"; i++) e[i].checked--;
    }
    function check() {
      if(f.datesel[1].checked && (!isnumeric(f.days.value) || f.days.value.length == 0)) {
        alert('The number of days must be a numeric value!');
        return false;
      }
      var sdate = new Date(swpdate(f.startdate.value));
      var edate = new Date(swpdate(f.enddate.value));
      if(f.datesel[2].checked && sdate > edate) {
        alert('Starting date must precede ending date!');
        return false;
      }
      var today = new Date();
      if(f.datesel[2].checked && edate > today) {
        alert('Don\'t look to any PopCon activity in the future!');
        return false;
      }
      return true;
    }
    function isnumeric(sText) {
      var validchars = "0123456789";
      var isnumber = true;
      for(var i = 0; i < sText.length && isnumber == true; i++) if(validchars.indexOf(sText.charAt(i)) == -1) isnumber = false;
      return isnumber;
    }
    function swpdate(d) {
      return d.substring(3, 6) + d.substring(0, 3) + d.substring(6, 10);
    }
    function order(of, ow) {
      if(check()) {
        f.ordfld.value = of;
        f.ordway.value = ow; 
        f.submit();
      }
    }
    function see(dest, tc) {
      if(tc == "hourly") sizes = 'width=820, height=500';
      else sizes = 'width=1000, height=475';
      var nwin = window.open(dest, tc, 'resizable=yes, menubar=no, toolbar=yes, scrollbars=yes, locations=no, status=yes, ' + sizes);
      nwin.focus();
    }
    // -->
  </script>
  <style type="text/css">
    @import url(cmspietro.css);
  </style>
</head>

<body onLoad="window.defaultStatus = document.title;">

<script language="php">
  $svr = (empty($_GET['svr'])? "cms_orcoff_prod": strtolower($_GET['svr']));
  $svrlog = (in_array(strtolower($_GET['svrlog']), array("int9r", "cms_orcoff_int", ""))? "int9r": "int2r");
  $ok = (empty($_GET['ok']) || $_GET['ok'] != "yes"? "no": "yes");
  $ordfld = (empty($_REQUEST['ordfld'])? "logid": $_REQUEST['ordfld']);
  $ordway = (empty($_REQUEST['ordway'])? "desc": $_REQUEST['ordway']);
  print("<p class=\"aq\">\n  PopCon recent activity recorded on ".strtoupper($svr)."\n</p>\n\n");
  print("<div id=\"overDiv\"></div>\n\n");
  print("<p class=\"tl\">\n  I. Graphs section\n</p>\n\n");
  print("<table class=\"p2k\">\n  <tr>\n    <td width=\"33%\" class=\"gc\">\n      graph types\n    </td>\n    <td width=\"34%\" class=\"gh\">\n      a. latest 24 hours\n    </td>\n    <td width=\"33%\" class=\"gh\">\n      b. latest 30 days\n    </td>\n  </tr>\n");
  print("  <tr>\n    <td class=\"ga\">\n      1. divided in thresholds\n    </td>\n    <td class=\"gh\">\n      <button onClick=\"javascript:see('popcongraph.php?tc=hourly&svr=$svr&svrlog=$svrlog&app=new', 'hourly');\">show activity</button>\n    </td>\n");
  print("    <td class=\"gh\">\n      <button onClick=\"javascript:see('popcongraph.php?tc=daily&svr=$svr&svrlog=$svrlog&app=new', 'daily');\">show activity</button>\n    </td>\n  </tr>\n  ");
  print("  <tr>\n    <td class=\"ga\">\n      2. grouped by subdetectors\n    </td>\n    <td class=\"gh\">\n      <button onClick=\"javascript:see('popcongraph.php?tc=hourly&svr=$svr&svrlog=$svrlog&app=new&vw=grp', 'hourly');\">show activity</button>\n    </td>\n");
  print("    <td class=\"gh\">\n      <button onClick=\"javascript:see('popcongraph.php?tc=daily&svr=$svr&svrlog=$svrlog&app=new&vw=grp&did=4', 'daily');\">show activity</button>\n    </td>\n  </tr>\n</table>\n\n");
  set_time_limit(0);
  $apl = "popcon";
  require(($path = "../private/")."support.inc");
  if($conn = @ocilogon($usr, $pwd, $tns)) {
    $fo = (empty($_GET['fo'])? "opt": strtolower($_GET['fo']));
    ociexecute(ociparse($conn, "alter session set nls_sort = 'BINARY_AI'"));
    print("<p class=\"tl\">\n  II. Choices section\n</p>\n\n");
    ociexecute($stmt = ociparse($conn, "select column_name from sys.user_tab_columns where table_name = '".strtoupper($tbl = "cond_log_view")."'"));
    if(ocifetchstatement($stmt, $results) > 0) {
      print("<form name=\"$fo\" method=\"get\" action=\"".basename($_SERVER['PHP_SELF']).(empty($_SERVER['QUERY_STRING'])? "": "?".$_SERVER['QUERY_STRING'])."\" onReset=\"alert('Settings are restored to the last refreshing of the present page!');\" onSubmit=\"this.fltr.value = this.fltr.value.trim(); return check();\">\n");
      print("  <table class=\"p2k\">\n    <tr>\n");
      print("      <th width=\"33%\" class=\"p2k\">\n        a. additional columns\n       </th>\n");
      print("      <th width=\"34%\" class=\"p2k\">\n        b. period\n      </th>\n");
      print("      <th width=\"33%\" class=\"p2k\">\n        c. granularity\n      </th>\n");
      print("    </tr>\n    <tr>\n      <td class=\"p2k\">\n");
      $all_fields = array_map("strtolower", $results['COLUMN_NAME']);
      $good_fields = array('exectime', 'iovtag', 'payloadname', 'destinationdb', 'execmessage');
      foreach(array_diff($all_fields, $good_fields) as $f) print("        <input type=\"checkbox\" name=\"$f\"".(empty($_REQUEST[$f])? "": " checked").">\n        $f\n        <br>\n");
      print("        <br>\n");
      print("        <nobr>\n          <input type=\"button\" value=\"select all\" onClick=\"selall(this.form.elements);\">\n");
      print("          <input type=\"button\" value=\"select none\" onClick=\"deselall(this.form.elements);\">\n");
      print("          <input type=\"button\" value=\"invert selection\" onClick=\"invsel(this.form.elements);\">\n        </nobr>\n");
      print("      </td>\n      <td class=\"p2k\">\n");
      print("        <input type=\"radio\" name=\"datesel\" value=\"24h\"".(in_array($_REQUEST['datesel'], array("24h", ""))? " checked": "").">\n");
      print("        latest 24 hours\n        <br>\n");
      print("        <input type=\"radio\" name=\"datesel\" value=\"ndays\"".($_REQUEST['datesel'] == "ndays"? " checked": "").">\n");
      print("        latest \n");
      print("        <input type=\"text\" name=\"days\" size=\"2\" maxlength=\"2\" value=\"".(empty($_REQUEST['days'])? "2": $_REQUEST['days'])."\" onKeyDown=\"this.form.datesel[1].checked = true; prev = this.value;\" onKeyUp=\"if(!isnumeric(this.value)) this.value = prev;\">\n");
      print("        days\n        <br>\n");
      ociexecute($stmt = ociparse($conn, "select to_char(min(exectime), 'DD/MM/YYYY') as mintime, to_char(max(exectime), 'DD/MM/YYYY') as maxtime from $usr.$tbl"));
      ocifetchstatement($stmt, $results);
      print("        <nobr>\n          <input type=\"radio\" name=\"datesel\" value=\"interval\"".($_REQUEST['datesel'] == "interval"? " checked": "").">\n");
      print("          from \n");
      print("          <input type=\"text\" name=\"startdate\" maxlength=\"10\" value=\"".(empty($_REQUEST['startdate'])? $results['MINTIME'][0]: $_REQUEST['startdate'])."\" class=\"dte\" readonly>\n");
      print("          <a href=\"javascript:f.datesel[2].checked = true; show_calendar('opt.startdate');\" onMouseOver=\"window.status = 'JavaScript Date Picker'; overlib('Click here to select starting date.'); return true;\" onMouseOut=\"window.status = document.title; nd(); return true;\"><img src=\"calendar.gif\" width=\"16\" height=\"16\" border=\"0\"></a>\n");
      print("          to\n");
      print("          <input type=\"text\" name=\"enddate\" maxlength=\"10\" value=\"".(empty($_REQUEST['enddate'])? $results['MAXTIME'][0]: $_REQUEST['enddate'])."\" class=\"dte\" readonly>\n");
      print("          <a href=\"javascript:f.datesel[2].checked = true; show_calendar('opt.enddate');\" onMouseOver=\"window.status = 'JavaScript Date Picker'; overlib('Click here to select ending date.'); return true;\" onMouseOut=\"window.status = document.title; nd(); return true;\"><img src=\"calendar.gif\" width=\"16\" height=\"16\" border=\"0\"></a>\n        </nobr>\n");
      print("      </td>\n      <td class=\"p2k\">\n");
      print("        <input type=\"radio\" name=\"sqlsel\" value=\"all\"".($_REQUEST['sqlsel'] != "latest"? " checked": "").">\n");
      print("        extract all records\n        <br>\n");
      print("        <nobr>\n          <input type=\"radio\" name=\"sqlsel\" value=\"latest\"".($_REQUEST['sqlsel'] == "latest"? " checked": "").">\n");
      print("          extract latest record for each IOV tag\n        </nobr>\n");
      print("      </td>\n    </tr>\n    <tr>\n      <td align=\"center\" colspan=\"3\" class=\"p2k\">\n");
      print("        <input type=\"hidden\" name=\"svr\" value=\"$svr\">\n");
      print("        <input type=\"hidden\" name=\"svrlog\" value=\"$svrlog\">\n");
      print("        <input type=\"hidden\" name=\"ok\" value=\"$ok\">\n");
      print("        <input type=\"hidden\" name=\"ordfld\" value=\"$ordfld\">\n");
      print("        <input type=\"hidden\" name=\"ordway\" value=\"$ordway\">\n");
      print("        <input type=\"hidden\" name=\"fo\" value=\"$fo\">\n");
      $usrtxt = "* type here a filter string *";
      print("        <input type=\"text\" name=\"fltr\" value=\"".(empty($_REQUEST['fltr'])? $usrtxt: $_REQUEST['fltr'])."\" class=\"btm\" onFocus=\"if(this.value == '$usrtxt') this.value = '';\" onBlur=\"this.value = this.value.trim(); if(this.value.length == 0) this.value = '$usrtxt';\">\n");
      print("        <input type=\"reset\" value=\"reset settings\" class=\"btm\">\n");
      print("        <input type=\"submit\" value=\"submit query\" class=\"btm\">\n");
      print("      </td>\n    </tr>\n  </table>\n");
      print("</form>\n\n");
      $sql = "select ";
      foreach($all_fields as $f) if(in_array($f, $good_fields) || !empty($_REQUEST[$f])) $sql .= $f . ", ";
      $sql = substr(str_replace("exectime", "to_char(exectime, 'FMMonth, ddth YYYY ') || to_char(exectime, 'HH24:MI:SS') as exectime", $sql), 0, -2) . " from $usr.$tbl where ";
      switch($_REQUEST['datesel']) {
        case "ndays": $sql .= "trunc(sysdate) - trunc(exectime) < " . $_REQUEST['days']; break;
        case "interval": $sql .= "trunc(exectime) between to_date('".$_REQUEST['startdate']."', 'DD/MM/YYYY') and to_date('".$_REQUEST['enddate']."', 'DD/MM/YYYY')"; break;
        default: $sql .= "trunc(sysdate, 'hh24') - trunc(exectime, 'hh24') < 1";
      }
      if($_REQUEST['sqlsel'] == "latest") $sql .= " and rowid in (select max(rowid) keep(dense_rank first order by exectime desc, logid desc) rv from $usr.$tbl group by iovtag)";
      if(!in_array($_REQUEST['fltr'], array("", $usrtxt))) {
        $sql .= " and lower(iovtag || ' ' || payloadname || ' ' || destinationdb) like '%".str_replace("_", "!_", strtolower($_REQUEST['fltr']))."%'";
        if(strpos($_REQUEST['fltr'], "_") !== false) $sql .= " escape '!'";
      }
      ociexecute($stmt = ociparse($conn, $sql = "$sql order by $usr.$tbl.$ordfld $ordway"));
      if(($nrows = ocifetchstatement($stmt, $results)) > 0) {
        print("<p class=\"tl\">\n  III. Results section\n</p>\n\n");
        print("<table class=\"p2k\">\n");
        print("  <tr>\n");
        while(list($key, $val) = each($results)) if($ok != "yes" || $key != "EXECMESSAGE") print("    <th class=\"p2k\">\n      <nobr>".strtolower(str_replace("_", " ", $key))."</nobr>\n      <br>\n      <a href=\"javascript:order('".strtolower($key)."', 'asc');\"><img src=\"arrowup.gif\" border=\"0\"></a><a href=\"javascript:order('".strtolower($key)."', 'desc');\"><img src=\"arrowdown.gif\" border=\"0\"></a>\n    </th>\n");
        print("  </tr>\n");
        reset($results);
        for($i = 0; $i < $nrows; $i++) {
          if($results['EXECMESSAGE'][$i] == "OK" || $ok != "yes") {
            print("  <tr valign=\"center\">\n");
            foreach($results as $key => $val) if($ok != "yes" || $key != "EXECMESSAGE") print("    <td class=\"".(stristr($results['EXECMESSAGE'][$i], 'conditions') !== false? "ug": ($ok != "yes"? "gr": "p2k"))."\">".str_replace("&lt;", "<br>&lt;", $val[$i])."</td>\n");
            print("  </tr>\n");
          }
        }
        print("</table>\n\n");
      }
      else print("<p class=\"bo\">\n  No PopCon activity recorded during the selected period!\n</p>\n\n");
      if($_GET['sq']{0} == "y") print("<p class=\"bo\">\n  The executed statement to query the database is:\n  <br>\n  $sql;\n</p>\n");
    }
    else print("<p class=\"bo\">\n  PopCon log table missing!\n</p>\n");
    ocifreestatement($stmt);
    ocilogoff($conn);
    $svr = $svrlog;
    $apl = "mon";
    require($path."auth.inc");
    $conn = ocilogon($usr, $pwd, $svr);
    require($path."geoip.inc");
    $gi = geoip_open("geoip.dat", GEOIP_STANDARD);  
      ociexecute(ociparse($conn, "insert into $usr.mon_log (domain, ip, country, browser, kind) values ('".strtolower(gethostbyaddr("$REMOTE_ADDR"))."', '$REMOTE_ADDR', '".geoip_country_name_by_addr($gi, $REMOTE_ADDR)."', '$HTTP_USER_AGENT', 'P')"));
      ocicommit($conn);
    geoip_close($gi);
    ocilogoff($conn);
  }
  elseif(strtolower($svr) != "servername") oraconnecterror();
</script>

<p class="bo">
  A Twiki page with a detailed explanation is available
  <a href="https://twiki.cern.ch/twiki/bin/view/CMS/PopConDashBoard" class="p2k" target="_blank">here</a>.
  Tails for all the cronjob script log files are listed in
  <a href="logtails.php" class="p2k">Cronjob Tail Fetcher</a>.
</p>

<p class="bo">
  This page has been dynamically generated on
  <?php print(newdate(time(), "eng")." at ".date("G:i", time()).", Central European time."); ?>
  Created on January 29<sup>th</sup>, 2008, its source code has been last updated on November 13<sup>th</sup>, 2008.
  <br>
  For any suggestion or comment, please write to
  <?php print("<a href=\"mailto:Pietro M. Picca <Pietro.Picca@cern.ch>?subject=$title\" class=\"p2k\">Pietro.Picca@cern.ch</a>"); ?>.
</p>

<script language="JavaScript">
  <!--
  var f = document.forms.<?php print($fo); ?>;
  // -->
</script>

</body>

</html>