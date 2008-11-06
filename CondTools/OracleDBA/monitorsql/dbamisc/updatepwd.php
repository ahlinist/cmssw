<script language="php">
  if(!isset($svr)) $svr = "int2r";
  function validate($usr, $pwd) {
    foreach(file("../private/updatepwd.pwd") as $line) {
      parse_str($line, $vars);
      if($usr == $vars['username'] && md5($pwd) == $vars['password']) return true;
    }
    return false;
  }
  function ok($param) {
    return !empty($_REQUEST[$param]);
  }
</script>
<html lang="en">

<head>
  <meta http-equiv="content-type" content="text/html; charset=iso-8859-15">
  <meta name="author" content="Pietro M. Picca, Pietro.Picca@cern.ch">
  <style type="text/css">
    @import url(updatepwd.css);
  </style>
<?php if(!ok("usr") || !ok("pwd")): ?>
  <title>
    Restricted area access
  </title>
</head>

<body onLoad="window.defaultStatus = document.title;">

<form name="login" method="post" action="<?php print(basename($_SERVER['PHP_SELF'])); ?>" onSubmit="return this.usr.value.length != 0 && this.pwd.value.length != 0;">
  <table>
    <tr>
      <td align="left">
        Username:
      </td>
      <td align="right">
        <input type="text" name="usr" maxlength="12">
      </td>
    </tr>
    <tr>
      <td align="left">
        Password:
      </td>
      <td align="right">
        <input type="password" name="pwd" maxlength="12">
      </td>
    </tr>
    <tr>
      <td colspan="2" align="middle">
        <input type="submit" value="Enter" class="bu">
        &nbsp;
        <input type="reset" value="Cancel" class="bu">
      </td>
    </tr>
<?php print("    <input type=\"hidden\" name=\"svr\" value=\"$svr\">\n"); ?>
  </table>
</form>
<script language="php">
  elseif(validate(($usr = $_REQUEST['usr']), ($pwd = $_REQUEST['pwd']))):
</script>
  <title>
    Passwords updating on <?php print(strtoupper($svr)."\n"); ?>
  </title>
</head>

<body onLoad="window.defaultStatus = document.title;">

<script language="php">
  set_time_limit(0);
  print("<p>\n  Executing SQL statements...\n</p>\n");
  $conn = ocilogon($usr, $pwd, $svr);
  require("geoip.inc");
  $gi = geoip_open("geoip.dat", GEOIP_STANDARD);  
    ociexecute(ociparse($conn, "insert into $usr.mon_log (domain, ip, country, browser, kind) values ('".strtolower(gethostbyaddr("$REMOTE_ADDR"))."', '$REMOTE_ADDR', '".geoip_country_name_by_addr($gi, $REMOTE_ADDR)."', '$HTTP_USER_AGENT', 'U')"));
    ocicommit($conn);
  geoip_close($gi);
  ociexecute(ociparse($conn, "begin $usr.gen_pwd; end;"));
  ociexecute($stmt = ociparse($conn, "select * from $usr.cms_cond_logins"));
  $nrows = ocifetchstatement($stmt, $results);
  ocifreestatement($stmt);
  ocilogoff($conn);
  print("<p>\n  Writing XML file...\n</p>\n");
  $xmlfiles = array("r" => array("int2r" => "readOnlyPrep.xml", "int9r" => "readOnlyInt.xml"), "w" => array("int2r" => "readWritePrep.xml", "int9r" => "readWriteInt.xml"));
  $svrnames = array("int2r" => "cms_orcoff_prep", "int9r" => "cms_orcoff_int");
  $myf = array("r" => ($mys = "<?xml version=\"1.0\" ?>\n<connectionlist>\n"), "w" => $mys);
  for($i = 0; $i < $nrows; $i++) {
    $key = (preg_match($ptrn = "/_R$/", $results['USERNAME'][$i]) == 1 ? "r": "w");
    $myf[$key] .= "<connection name=\"oracle://$svrnames[$svr]/".preg_replace($ptrn, "", $results['USERNAME'][$i])."\">\n";
    $myf[$key] .= "  <parameter name=\"user\" value=\"".strtolower($results['USERNAME'][$i])."\" />\n";
    $myf[$key] .= "  <parameter name=\"password\" value=\"".$results['PASSWORD'][$i]."\" />\n";
    $myf[$key] .= "</connection>\n";
  }
  foreach($xmlfiles as $key => $xmlfile) {
    if(file_exists(($path = "/afs/cern.ch/user/p/p2k/xml/").$xmlfile[$svr])) {
      $handle = fopen($path.$xmlfile[$svr], "r");
        while(!feof($handle)) {
          $myl = fgets($handle);
          if(strstr($myl, "?xml") !== false) fgets($handle);
          elseif(strstr($myl, "oracle://$svrnames[$svr]") !== false) for($i = 0; $i < 3; $i++) fgets($handle);
          elseif(rtrim($myl) != "</connectionlist>" && rtrim($myl) != "") $myf[$key] .= rtrim($myl)."\n";
        }
      fclose($handle);
    }
    $myf[$key] .= "</connectionlist>";
    $handle = fopen($path.$xmlfile[$svr], "w");
      fwrite($handle, $myf[$key]);
    fclose($handle);
  }
  print("<p>\n  Sending reminding message...\n</p>\n");
  mail("Pietro M. Picca <Pietro.Picca@cern.ch>, Zhen Xie <Zhen.Xie@cern.ch>", "Oracle connection passwords for $svrnames[$svr] changed", "Dear both,\n\nI am daring to inform you that, from 6 o'clock in the morning of the day after tomorrow on, passwords will not be the same ones.\nYou may want to warn users...\n\nYou will find the new XML files here:\n$path\n\nCheers,\nyour faithful routine!");
  print("<p>\n  Done.\n</p>\n");
</script>
<?php else: ?>
  <title>
    Restricted area access
  </title>
</head>

<body onLoad="window.defaultStatus = document.title;">

<p>
  Supplied data are not correct.
  <br>
  Click
  <?php print("<a href=\"javascript:self.location.href = this.location.href + '?svr=$svr';\">here</a>\n"); ?>
  to try again.
</p>
<?php endif; ?>

</body>

</html>