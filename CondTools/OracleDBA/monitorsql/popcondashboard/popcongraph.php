<script language="php">
  set_time_limit(0);
  $tc = (strtolower($_GET['tc']) == "daily"? "daily": "hourly");
  $svr = (empty($_GET['svr'])? "cms_orcoff_prod": strtolower($_GET['svr']));
  $svrlog = (in_array(strtolower($_GET['svrlog']), array("int9r", "cms_orcoff_int", ""))? "int9r": "int2r");
  $app = (empty($_GET['app'])? "new": strtolower($_GET['app']));
  $thresholds = array("yts" => empty($_GET['yts'])? 30: $_GET['yts'], "rts" => empty($_GET['rts'])? 100: $_GET['rts']);
  $apl = "popcon";
  require(($path="../private/")."support.inc");
  if($conn = @ocilogon($usr, $pwd, $tns)) {
    if(empty($_GET['det'])) $sqlstr = "select ".($fld = ($tc == "hourly"? "hour": "day")).", sum(frequency) as frequency from $usr.p_con_hits_$tc"."_$app group by $fld";
    else $sqlstr = "select ".($tc == "hourly"? "hour": "day").", frequency from $usr.p_con_hits_$tc"."_$app where account = '".strtoupper($_GET['det'])."'";
    ociexecute($stmt = ociparse($conn, $sqlstr));
    if(($nrows = ocifetchstatement($stmt, $graphValues)) > 0) {
      $sfactor = 1.2;
      $std = 30;
      $offset = 3;
      $did = abs(intval($_GET['did']));
      $did = (empty($did)? 3: min($did, 5));
      $xtopoffset = 48;
      $ytopoffset = 24;
      $ybottomoffset = ($tc == "hourly"? 77: 29);
      $quad = abs(intval($_GET['quad']));
      $quad = (empty($quad)? 10: min($quad, 100));
      $imgHeight = $std * $quad;
      $mval = 0;
      $graphValues = array_values($graphValues);
      foreach($graphValues[0] as $key => $val) {
        $newValues[$graphValues[0][$key]] = $graphValues[1][$key];
        if($graphValues[1][$key] > $mval) $mval = $graphValues[1][$key];
      }
      $showValues = array_values(fill_missing($newValues));
      $newValues = fill_missing(array_map("sizeHeight", $newValues));
      $graphValues = array_values($newValues);
      $imgWidth = count($graphValues) * $std;
      header("Content-type: image/png");
      if($_GET['vw'] == "grp" && empty($_GET['det'])) {
        ociexecute($stmt2 = ociparse($conn, "select distinct account from $usr.p_con_hits_$tc"."_$app order by account"));
        $mrows = ocifetchstatement($stmt2, $detValues);
        $detValues = array_values($detValues);
        $yf = ceil($mrows / $did) * 25 + 23;
        $image = imagecreatetruecolor($xtopoffset + $imgWidth + 1, $ytopoffset + $imgHeight + $ybottomoffset + $yf);
        $tmpa = array(array("r" => 0, "g" => 0, "b" => 0), array("r" => 255, "g" => 255, "b" => 255));
        $safecounter = 0;
        do {
          $tmpe = array("r" => rand(0, 255), "g" => rand(0, 255), "b" => rand(0, 255));
          $flag = true;
          for($i = 0; $i < count($tmpa); $i++) if(sqrt(($tmpe['r'] - $tmpa[$i]['r']) * ($tmpe['r'] - $tmpa[$i]['r']) + ($tmpe['g'] - $tmpa[$i]['g']) * ($tmpe['g'] - $tmpa[$i]['g']) + ($tmpe['b'] - $tmpa[$i]['b']) * ($tmpe['b'] - $tmpa[$i]['b'])) < 256 * pow($mrows + 2, -1 / 3)) $flag = false;
          if($flag == true) array_push($tmpa, $tmpe);
          if(++$safecounter == 1000) {
            $tmpa = array(array("r" => 0, "g" => 0, "b" => 0), array("r" => 255, "g" => 255, "b" => 255));
            $safecounter = 0;
            mail("Pietro.Picca@cern.ch", "PopCon dashboard graph notice", "The safe counter has reached its limit and it has been reset.");
          }
        }
        while(count($tmpa) - 2 < $mrows);
        $colours = array();
        for($i = 0; $i < $mrows; $i++) $colours[$detValues[0][$i]] = imagecolorallocate($image, $tmpa[$i + 2]['r'], $tmpa[$i + 2]['g'], $tmpa[$i + 2]['b']);
        $detValues = $colours;
        ocifreestatement($stmt2);
      }
      else {
        $yf = (empty($_GET['det'])? ceil(3 / $did) * 25 + 23: 0);
        $image = imagecreatetruecolor($xtopoffset + $imgWidth + 1, $ytopoffset + $imgHeight + $ybottomoffset + $yf);
        $ts = array();
        array_push($ts, array("threshold" => 0, "colour" => imagecolorallocate($image, 25, 25, 64), "label" => "From 0 to ".$thresholds['yts']." hits"));
        array_push($ts, array("threshold" => sizeHeight($thresholds['yts']), "colour" => imagecolorallocate($image, 255, 207, 0), "label" => "From ".($thresholds['yts'] + 1)." to ".$thresholds['rts']." hits"));
        array_push($ts, array("threshold" => sizeHeight($thresholds['rts']), "colour" => imagecolorallocate($image, 220, 14, 60), "label" => "Over ".$thresholds['rts']." hits"));
        array_push($ts, array("threshold" => sizeHeight($mval)));
      }
      imagealphablending($image, true);
      $colorWhite = imagecolorallocate($image, 255, 255, 255);
      imagecolortransparent($image, $colorWhite);
      $colorGrey = imagecolorallocate($image, 191, 191, 191);
      $colorBlack = imagecolorallocate($image, 0, 0, 0);
      imagefilledrectangle($image, 0, 0, $xtopoffset + $imgWidth, $ytopoffset + $imgHeight + $ybottomoffset + $yf, $colorWhite);
      $imagebg = imagecreatefromjpeg("marble.jpg");
      imagesettile($image, $imagebg);
      imagefilledrectangle($image, $xtopoffset, $ytopoffset, $xtopoffset + $imgWidth, $ytopoffset + $imgHeight, IMG_COLOR_TILED);
      imagestring($image, $sz = 5, xpos($title = "PopCon activity during the latest ".($tc == "hourly"? "24 hours": "30 days").(empty($_GET['det'])? "": " for ".strtoupper($_GET['det'])." account"), $sz), 0, $title, $colorBlack);
      $mykeys = array_keys($newValues);
      for($i = 0; $i < $imgWidth / $std; $i++) {
        imageline($image, $xtopoffset + $i * $std, $ytopoffset, $xtopoffset + $i * $std, $ytopoffset + $imgHeight - 1, $colorGrey);
        if($_GET['vw'] == "grp" && empty($_GET['det'])) {
          ociexecute($stmt2 = ociparse($conn, "select account, frequency from $usr.p_con_hits_$tc"."_$app where ".($tc == "hourly"? "hour": "day")." = '".$mykeys[$i]."'"));
          ocifetchstatement($stmt2, $detNValues);
          $ts = array();
          $tl = 0;
          $detNValues = array_values($detNValues);
          $tmp = array();
          for($j = 0; $j < count($detNValues[0]); $j++) $tmp[$detNValues[0][$j]] = $detNValues[1][$j];
          foreach(array_keys($detNValues = $tmp) as $key) {
            array_push($ts, array("threshold" => sizeHeight($tl), "colour" => $detValues[$key], "label" => $key));
            $tl += $detNValues[$key];
          }
          array_push($ts, array("threshold" => sizeHeight($tl)));
          ocifreestatement($stmt2);
        }
        for($j = 0; $j < count($ts); $j++) imagefilledrectangle($image, $xtopoffset + ($i * $std) + $offset, $ytopoffset + $imgHeight - min($ts[$j + 1]['threshold'], $graphValues[$i]), $xtopoffset + ($i + 1) * $std - $offset, $ytopoffset + $imgHeight - min($ts[$j]['threshold'], $graphValues[$i]) - 1, $ts[$j]['colour']);
        $myd = explode(":", $mykeys[$i]);
        if($tc == "hourly") {
          if($myd[2] == date("d", time())) imagestringup($image, 1, $xtopoffset + ($i * $std) + 6, $ytopoffset + $imgHeight + 29, "Today", $colorBlack);
          else imagestringup($image, 1, $xtopoffset + ($i * $std) + 6, $ytopoffset + $imgHeight + 49, "Yesterday", $colorBlack);
          imagestringup($image, 1, $xtopoffset + ($i * $std) + 16, $ytopoffset + $imgHeight + 59, $myd[3].":00-".$myd[3].":59", $colorBlack);
        }
        else imagestring($image, 1, $xtopoffset + ($i * $std) + $offset, $ytopoffset + $imgHeight + $offset + 1, $myd[2]."/".$myd[1], $colorBlack);
      }
      for($i = 0; $i <= $imgHeight / $std; $i++) {
        imageline($image, $xtopoffset, $ytopoffset + $i * $std, $xtopoffset + $imgWidth, $ytopoffset + $i * $std, $colorGrey);
        imagestring($image, 1, $xtopoffset - 24, $ytopoffset + $imgHeight - $offset - ($i + 1) * $std, str_pad(($i + 1) * ceil($mval * $sfactor / $quad), 4, " ", STR_PAD_LEFT), $colorBlack);
      }
      imagestring($image, 1, $xtopoffset - 9, $ytopoffset + $imgHeight - 4, "0", $colorBlack);
      imagestring($image, $sz = 3, xpos($xlabel = "Time", $sz), $ytopoffset + $imgHeight + ($tc == "hourly"? 66: 18), $xlabel, $colorBlack);
      for($i = 0; $i < $imgWidth / $std; $i++) if($showValues[$i] > 0 && $_GET['sl'] != "no") imagestring($image, 2, $xtopoffset + ($i * $std) + round(($std - 5 * strlen($showValues[$i])) / 2), $ytopoffset + $imgHeight - $graphValues[$i] - 16, $showValues[$i], $colorBlack);
      imageline($image, $xtopoffset + $imgWidth, $ytopoffset, $xtopoffset + $imgWidth, $ytopoffset + $imgHeight - 1, $colorGrey);
      imagestringup($image, $sz = 3, 0, ypos($ylabel = "Number of successful transfers", $sz), $ylabel, $colorBlack);
      if(empty($_GET['det'])) {
        $lg = array();
        if($_GET['vw'] == "grp") foreach($detValues as $key => $val) array_push($lg, array("label" => $key, "colour" => $val));
        else {
          array_pop($ts);
          foreach($ts as $key => $val) array_push($lg, array("label" => $val['label'], "colour" => $val['colour']));
        }
        imagerectangle($image, $xtopoffset, $ytopoffset + $imgHeight + $ybottomoffset + 10, $xtopoffset + $imgWidth, $ytopoffset + $imgHeight + $ybottomoffset + $yf - 1, $colorGrey);
        imagefilledrectangle($image, $xtopoffset + 1, $ytopoffset + $imgHeight + $ybottomoffset + 11, $xtopoffset + $imgWidth - 1, $ytopoffset + $imgHeight + $ybottomoffset + $yf - 2, IMG_COLOR_TILED);
        for($i = 0; $i < count($lg); $i ++) {
          imagerectangle($image, $xtopoffset + round(($i % $did) * ($imgWidth / $did)) + 13, $ytopoffset + $imgHeight + $ybottomoffset + floor($i / $did) * 25 + 22, $xtopoffset + round(($i % $did) * ($imgWidth / $did)) + 26, $ytopoffset + $imgHeight + $ybottomoffset + floor($i / $did) * 25 + 35, $colorBlack);
          imagefilledrectangle($image, $xtopoffset + round(($i % $did) * ($imgWidth / $did)) + 14, $ytopoffset + $imgHeight + $ybottomoffset + floor($i / $did) * 25 + 23, $xtopoffset + round(($i % $did) * ($imgWidth / $did)) + 25, $ytopoffset + $imgHeight + $ybottomoffset + floor($i / $did) * 25 + 34, $lg[$i]['colour']);
          imagestring($image, 3, $xtopoffset + round(($i % $did) * ($imgWidth / $did)) + 36, $ytopoffset + $imgHeight + $ybottomoffset + floor($i / $did) * 25 + 22, $lg[$i]['label'], $colorBlack);
        }
      }
      imagepng($image);
      imagedestroy($image);
      imagedestroy($imagebg);
    }
    else print("<p class=\"bo\">\n  No PopCon activity recorded during the latest ".($tc == "hourly"? "24 hours": "30 days").(empty($_GET['det'])? "!": " for ".strtoupper($_GET['det'])." account!\n</p>"));
    ocifreestatement($stmt);
    ocilogoff($conn);
    $svr = $svrlog;
    $apl = "mon";
    require($path."auth.inc");
    $conn = ocilogon($usr, $pwd, $svr);
    require($path."geoip.inc");
    $gi = geoip_open("geoip.dat", GEOIP_STANDARD);
      $stmt = ociparse($conn, "insert into $usr.mon_log (domain, ip, country, browser, kind) values ('".strtolower(gethostbyaddr("$REMOTE_ADDR"))."', '$REMOTE_ADDR', '".geoip_country_name_by_addr($gi, $REMOTE_ADDR)."', '$HTTP_USER_AGENT', 'G') returning to_char(id) into :RID");
      ocibindbyname($stmt, ":RID", $rid, 10);
      ociexecute($stmt);
      ocifreestatement($stmt);
    geoip_close($gi);
    if($_GET['vw'] == "grp" && $nrows > 0) ociexecute(ociparse($conn, "insert into $usr.mon_log_colours (id, attempts, query_string) values($rid, $safecounter, '".$_SERVER['QUERY_STRING']."')"));
    ocicommit($conn);
    ocilogoff($conn);
  }
  else oraconnecterror();
  function xpos($s, $charWidth) {
    return($GLOBALS['xtopoffset'] + round(($GLOBALS['imgWidth'] - ($charWidth + 4) * strlen($s)) / 2));
  }
  function ypos($s, $charWidth) {
    return($GLOBALS['ytopoffset'] + round(($GLOBALS['imgHeight'] + ($charWidth + 4) * strlen($s)) / 2));
  }
  function sizeHeight($y) {
    return(round($y * $GLOBALS['imgHeight'] / ceil($GLOBALS['mval'] * $GLOBALS['sfactor'] / $GLOBALS['quad']) / $GLOBALS['quad']));
  }
  function fill_missing($myarray) {
    $tmp = array_reverse(array_keys($myarray));
    if(ereg("([0-9]{4}):([0-9]{2}):([0-9]{2}):([0-9]{2})", $tmp[0])) for($i = time() - 82800; $i < time() + 3600; $i += 3600) {
      if(!array_key_exists(date("Y:m:d:H", $i), $myarray)) $myarray[date("Y:m:d:H", $i)] = 0;
    }
    else for($i = time() - 2505600; $i < time() + 86400; $i += 86400) {
      if(!array_key_exists(date("Y:m:d", $i), $myarray)) $myarray[date("Y:m:d", $i)] = 0;
    }
    ksort($myarray);  
    return($myarray);
  }
</script>