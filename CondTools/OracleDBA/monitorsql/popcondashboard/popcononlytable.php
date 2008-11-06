<script language="php">
  $svr = (empty($_GET['svr'])? "cms_orcoff_prod": strtolower($_GET['svr']));
  $ok = (empty($_GET['ok']) || $_GET['ok'] != "yes"? "no": "yes");
  $ordfld = (empty($_REQUEST['ordfld'])? "logid": $_REQUEST['ordfld']);
  $ordway = (empty($_REQUEST['ordway'])? "desc": $_REQUEST['ordway']);
  $fltr = (empty($_REQUEST['fltr'])? $usrtxt: $_REQUEST['fltr']);
  set_time_limit(0);
  require("../private/support.inc");
  if($conn = @ocilogon($usr, $pwd, $tns)) {
    ociexecute(ociparse($conn, "alter session set nls_sort = 'BINARY_AI'"));
    ociexecute($stmt = ociparse($conn, "select column_name from sys.user_tab_columns where table_name = '".strtoupper($tbl = "cond_log_view")."'"));
    if(ocifetchstatement($stmt, $results) > 0) {
      $all_fields = array_map("strtolower", array_diff($results['COLUMN_NAME'], array("ACTIVITYGRAPH")));
      $good_fields = array('logid', 'exectime', 'iovtag', 'payloadname', 'payloadindex', 'destinationdb', 'usertext', 'execmessage');
      $usrtxt = "* type here a filter string *";
      $sql = "select ";
      foreach($all_fields as $f) if(in_array($f, $good_fields) || !empty($_REQUEST[$f])) $sql .= $f . ", ";
      $sql = substr(str_replace("exectime", "sys_extract_utc(cast(exectime as timestamp with time zone)) as exectime", $sql), 0, -2) . " from $usr.$tbl where ";
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
        $tmp = "";
        while(list($key, $val) = each($results)) if($ok != "yes" || $key != "EXECMESSAGE") $tmp .= $key.";;;";
        print(ereg_replace(";;;$", ";!;\n", $tmp));
        reset($results);
        for($i = 0; $i < $nrows; $i++) {
          $tmp = "";
          if($results['EXECMESSAGE'][$i] == "OK" || $ok != "yes") {
            foreach($results as $key => $val) if($ok != "yes" || $key != "EXECMESSAGE") $tmp .= ($key == "PAYLOADNAME"? html_entity_decode($val[$i]): $val[$i]).";;;";
            print(ereg_replace(";;;$", ";!;\n", $tmp));
          }
        }
      }
    }
    ocifreestatement($stmt);
    ocilogoff($conn);
  }
  elseif(strtolower($svr) != "servername") oraconnecterror();
</script>