
function printline(field, lo, hi) {
    if (hi==lo) {
	printf "%s==%d||", field, hi-1;
    }
    else {
	printf "(%s>=%d&&%s<=%d)||", field, lo-1, field, hi-1;
    }
}


BEGIN {
    i = 0;
    initval = -10;
    lo = initval;
    hi = initval;
    field = "nRef2G";
    first = 1;
}

{
    i++;
}

/[( ]411 / {
#/[( ]443 / {
#print i, lo, hi, $0;
    if (lo == initval) {
	lo = i; hi = i;
	next;
    }
    if ((i-1) == hi) {
	hi = i;
	next;
    }
    printline(field, lo, hi);
    lo = i;
    hi = i;
}

END {
    if (hi != initval) printline(field, lo, hi);
    printf "\n";
}

