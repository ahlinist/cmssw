
function formatDate(d)
{
	var twoDigit = function (q) { return ((q / 10) | 0).toString() + (q % 10).toString(); };
	var month=["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"];

	var date = twoDigit(d.getHours()) + ":" + twoDigit(d.getMinutes());
	date += ' ' + month[d.getMonth()];
	date += ' ' + twoDigit(d.getDate());
	return date;
};

  function decOnly(i) {
    var t = i.value;
    if(t.length>0) {
        t = t.replace(/[^\d\.]+/g, ''); 
    }
    var s = t.split('.');
    if(s.length>1) {
        s[1] = s[0] + '.' + s[1];   
        s.shift(s);
    }
    i.value = s.join('');
  }

  function intOnly(i) {
    if(i.value.length>0) {
        i.value = i.value.replace(/[^\d]+/g, ''); 
    }
  }

  function trim(s) {
    if (!s) return s;
    s = s.replace(/^[ \t\n\r]*/g, '');
    s = s.replace(/[ \t\n\r]*$/g, '');
    return s;
  }

  function normalizePersonName(n) {
	if (!n)
		return n;
    n = trim(n);
    var names = "";
    while (n.indexOf(" ") >= 0) {
      names += trim(n.substring(0, n.indexOf(" "))).substring(0, 1) + ". ";
      n = trim(n.substring(n.indexOf(" ")));
    }
    return names.toUpperCase() .toUpperCase()+ n.substring(0, 1).toUpperCase() + n.substring(1).toLowerCase();
  }

  function formatNumber(nStr) {
    nStr += '';
    x = nStr.split('.');
    x1 = x[0];
    x2 = x.length > 1 ? '.' + x[1] : '';
    var rgx = /(\d+)(\d{3})/;
    while (rgx.test(x1)) {
      x1 = x1.replace(rgx, '$1' + ',' + '$2');
    }
    return x1 + x2;
  }

