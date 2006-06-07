CREATE OR REPLACE PACKAGE fill_dummy IS
procedure fill_pedestals;
procedure fill_gains;
procedure fill_crosstalk;
procedure fill_noisematrix;
end;
/
show errors

create or replace package body fill_dummy is
norm number := 2147483648;
TYPE table_type IS TABLE OF number INDEX BY BINARY_INTEGER;
chambers table_type;

procedure fill_pedestals is
run number;
layer number;
i number;
j number;
k number;
i_record_id number;
i_map_id number;
i_map_index number;
mean_ped number;
mean_rms number;
rndm number;
i_ped number;
i_rms number;
BEGIN
select max(record_id) into i_record_id from pedestals;
if i_record_id is null then
 i_record_id:=0;
end if;
select max(map_id) into i_map_id from pedestals_map;
if i_map_id is null then
 i_map_id:=0;
end if;
for run in 1..5 loop
 i_record_id:=i_record_id+1;
 insert into pedestals values (i_record_id,run,sysdate,sysdate);
 i_map_index:=0;
 for i in 1..18 loop
  select dbms_random.random into rndm from dual;
  mean_ped:=597+rndm/norm*21;
  select dbms_random.random into rndm from dual;
  mean_rms:=2.07+rndm/norm*0.24;
  for j in 1..6 loop
   i_map_id:=i_map_id+1;
   i_map_index:=i_map_index+1;
   layer:=chambers(i)+j;
   insert into pedestals_map values (i_map_id,i_record_id,i_map_index,layer);
   for k in 1..80 loop
    select dbms_random.random into rndm from dual;
    i_ped:=mean_ped+rndm/norm*56;
    i_rms:=mean_rms+rndm/norm*0.35;
    insert into pedestals_data values(i_map_id,k,i_ped,i_rms);
   end loop;
  end loop;
 end loop;
end loop;
END fill_pedestals;

procedure fill_gains is
run number;
layer number;
i number;
j number;
k number;
i_record_id number;
i_map_id number;
i_map_index number;
mean_gain_slope number;
mean_gain_intercept number;
mean_gain_chi2 number;
rndm number;
i_gain_slope number;
i_gain_intercept number;
i_gain_chi2 number;
BEGIN
select max(record_id) into i_record_id from gains;
if i_record_id is null then
 i_record_id:=0;
end if;
select max(map_id) into i_map_id from gains_map;
if i_map_id is null then
 i_map_id:=0;
end if;
for run in 1..5 loop
 i_record_id:=i_record_id+1;
 insert into gains values (i_record_id,run,sysdate,sysdate);
 i_map_index:=0;
 for i in 1..18 loop
  select dbms_random.random into rndm from dual;
  mean_gain_slope:=7.31+rndm/norm*0.68;
  select dbms_random.random into rndm from dual;
  mean_gain_intercept:=-13.2+rndm/norm*8.8;
  select dbms_random.random into rndm from dual;
  mean_gain_chi2:=2.17+rndm/norm*0.76;
  for j in 1..6 loop
   i_map_id:=i_map_id+1;
   i_map_index:=i_map_index+1;
   layer:=chambers(i)+j;
--   DBMS_OUTPUT.PUT_LINE (i_map_id||' '||i_record_id||' '||i_map_index||
--    ' '||layer);
   insert into gains_map values (i_map_id,i_record_id,i_map_index,layer);
   for k in 1..80 loop
    select dbms_random.random into rndm from dual;
    i_gain_slope:=mean_gain_slope+rndm/norm*0.58;
    i_gain_intercept:=mean_gain_intercept+rndm/norm*11.8;
    i_gain_chi2:=mean_gain_chi2*(1+rndm/norm*0.8);
    insert into gains_data values(i_map_id,k,i_gain_slope,i_gain_intercept,
    i_gain_chi2);
   end loop;
  end loop;
 end loop;
end loop;
END fill_gains;

procedure fill_crosstalk is
run number;
layer number;
i number;
j number;
k number;
i_record_id number;
i_map_id number;
i_map_index number;
mean_xtalk_slope_right number;
mean_xtalk_intercept_right number;
mean_xtalk_chi2_right number;
mean_xtalk_slope_left number;
mean_xtalk_intercept_left number;
mean_xtalk_chi2_left number;
rndm number;
i_xtalk_slope_right number;
i_xtalk_intercept_right number;
i_xtalk_chi2_right number;
i_xtalk_slope_left number;
i_xtalk_intercept_left number;
i_xtalk_chi2_left number;
BEGIN
select max(record_id) into i_record_id from crosstalk;
if i_record_id is null then
 i_record_id:=0;
end if;
select max(map_id) into i_map_id from crosstalk_map;
if i_map_id is null then
 i_map_id:=0;
end if;
for run in 1..5 loop
 i_record_id:=i_record_id+1;
 insert into crosstalk values (i_record_id,run,sysdate,sysdate);
 i_map_index:=0;
 for i in 1..18 loop
  select dbms_random.random into rndm from dual;
  mean_xtalk_slope_right:=0.0009+rndm/norm*0.00005;
  select dbms_random.random into rndm from dual;
  mean_xtalk_intercept_right:=0.01+rndm/norm*0.0005;
  select dbms_random.random into rndm from dual;
  mean_xtalk_chi2_right:=1.9+rndm/norm*0.1;
  select dbms_random.random into rndm from dual;
  mean_xtalk_slope_left:=0.0009+rndm/norm*0.00005;
  select dbms_random.random into rndm from dual;
  mean_xtalk_intercept_left:=0.01+rndm/norm*0.0005;
  select dbms_random.random into rndm from dual;
  mean_xtalk_chi2_left:=1.9+rndm/norm*0.1;
  for j in 1..6 loop
   i_map_id:=i_map_id+1;
   i_map_index:=i_map_index+1;
   layer:=chambers(i)+j;
--   DBMS_OUTPUT.PUT_LINE (i_map_id||' '||i_record_id||' '||i_map_index||
--    ' '||layer);
   insert into crosstalk_map values (i_map_id,i_record_id,i_map_index,layer);
   for k in 1..80 loop
    select dbms_random.random into rndm from dual;
    i_xtalk_slope_right:=mean_xtalk_slope_right+rndm/norm*0.00002;
    select dbms_random.random into rndm from dual;
    i_xtalk_intercept_right:=mean_xtalk_intercept_right+rndm/norm*0.0002;
    select dbms_random.random into rndm from dual;
    i_xtalk_chi2_right:=mean_xtalk_chi2_right+rndm/norm*0.04;
    select dbms_random.random into rndm from dual;
    i_xtalk_slope_left:=mean_xtalk_slope_left+rndm/norm*0.00002;
    select dbms_random.random into rndm from dual;
    i_xtalk_intercept_left:=mean_xtalk_intercept_left+rndm/norm*0.0002;
    select dbms_random.random into rndm from dual;
    i_xtalk_chi2_left:=mean_xtalk_chi2_left*1+rndm/norm*0.04;
    insert into crosstalk_data values(i_map_id,k,i_xtalk_slope_right,
     i_xtalk_intercept_right,i_xtalk_chi2_right,i_xtalk_slope_left,
     i_xtalk_intercept_left,i_xtalk_chi2_left);
   end loop;
  end loop;
 end loop;
end loop;
END fill_crosstalk;

procedure fill_noisematrix is
run number;
layer number;
i number;
j number;
k number;
i_record_id number;
i_map_id number;
i_map_index number;
mean_elem33 number;
mean_elem34 number;
mean_elem35 number;
mean_elem44 number;
mean_elem45 number;
mean_elem46 number;
mean_elem55 number;
mean_elem56 number;
mean_elem57 number;
mean_elem66 number;
mean_elem67 number;
mean_elem77 number;
rndm number;
i_elem33 number;
i_elem34 number;
i_elem35 number;
i_elem44 number;
i_elem45 number;
i_elem46 number;
i_elem55 number;
i_elem56 number;
i_elem57 number;
i_elem66 number;
i_elem67 number;
i_elem77 number;
BEGIN
select max(record_id) into i_record_id from noisematrix;
if i_record_id is null then
 i_record_id:=0;
end if;
select max(map_id) into i_map_id from noisematrix_map;
if i_map_id is null then
 i_map_id:=0;
end if;
for run in 1..5 loop
 i_record_id:=i_record_id+1;
 insert into noisematrix values (i_record_id,run,sysdate,sysdate);
 i_map_index:=0;
 for i in 1..18 loop
  select dbms_random.random into rndm from dual;
  mean_elem33:=8.+rndm/norm*2.;
  select dbms_random.random into rndm from dual;
  mean_elem34:=4.+rndm/norm*1.;
  select dbms_random.random into rndm from dual;
  mean_elem35:=3.+rndm/norm*1.;
  select dbms_random.random into rndm from dual;
  mean_elem44:=9.+rndm/norm*2.;
  select dbms_random.random into rndm from dual;
  mean_elem45:=3.+rndm/norm*0.8;
  select dbms_random.random into rndm from dual;
  mean_elem46:=2.+rndm/norm*0.5;
  select dbms_random.random into rndm from dual;
  mean_elem55:=10.+rndm/norm*2.;
  select dbms_random.random into rndm from dual;
  mean_elem56:=4.+rndm/norm*1.;
  select dbms_random.random into rndm from dual;
  mean_elem57:=2.3+rndm/norm*0.7;
  select dbms_random.random into rndm from dual;
  mean_elem66:=10.5+rndm/norm*2.2;
  select dbms_random.random into rndm from dual;
  mean_elem67:=3.3+rndm/norm*0.7;
  select dbms_random.random into rndm from dual;
  mean_elem77:=9.3+rndm/norm*2.5;
  for j in 1..6 loop
   i_map_id:=i_map_id+1;
   i_map_index:=i_map_index+1;
   layer:=chambers(i)+j;
--   DBMS_OUTPUT.PUT_LINE (i_map_id||' '||i_record_id||' '||i_map_index||
--    ' '||layer);
   insert into noisematrix_map values (i_map_id,i_record_id,i_map_index,layer);
   for k in 1..80 loop
    select dbms_random.random into rndm from dual;
    i_elem33:=mean_elem33+rndm/norm*0.5;
    select dbms_random.random into rndm from dual;
    i_elem34:=mean_elem34+rndm/norm*0.4;
    select dbms_random.random into rndm from dual;
    i_elem35:=mean_elem35+rndm/norm*0.3;
    select dbms_random.random into rndm from dual;
    i_elem44:=mean_elem44+rndm/norm*0.6;
    select dbms_random.random into rndm from dual;
    i_elem45:=mean_elem45+rndm/norm*0.4;
    select dbms_random.random into rndm from dual;
    i_elem46:=mean_elem46+rndm/norm*0.2;
    select dbms_random.random into rndm from dual;
    i_elem55:=mean_elem55+rndm/norm*0.6;
    select dbms_random.random into rndm from dual;
    i_elem56:=mean_elem56+rndm/norm*0.5;
    select dbms_random.random into rndm from dual;
    i_elem57:=mean_elem57+rndm/norm*0.3;
    select dbms_random.random into rndm from dual;
    i_elem66:=mean_elem66+rndm/norm*0.7;
    select dbms_random.random into rndm from dual;
    i_elem67:=mean_elem67+rndm/norm*0.4;
    select dbms_random.random into rndm from dual;
    i_elem77:=mean_elem77+rndm/norm*0.7;
    insert into noisematrix_data values(i_map_id,k,i_elem33,i_elem34,
     i_elem35,i_elem44,i_elem45,i_elem46,i_elem55,i_elem56,i_elem57,
     i_elem66,i_elem67,i_elem77);
   end loop;
  end loop;
 end loop;
end loop;
END fill_noisematrix;

BEGIN
chambers(1):= 220121140;
chambers(2):= 220121150;
chambers(3):= 220121160;
chambers(4):= 220121270;
chambers(5):= 220121280;
chambers(6):= 220121290;
chambers(7):= 220121300;
chambers(8):= 220121310;
chambers(9):= 220121320;
chambers(10):= 220131140;
chambers(11):= 220131150;
chambers(12):= 220131160;
chambers(13):= 220131270;
chambers(14):= 220131280;
chambers(15):= 220131290;
chambers(16):= 220131300;
chambers(17):= 220131310;
chambers(18):= 220131320;
end;
/
show errors
