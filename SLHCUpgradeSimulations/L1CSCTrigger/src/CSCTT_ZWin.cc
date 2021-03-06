#include "../interface/CSCTT_ZWin.h"
#include "math.h"
#include <iostream>

CSCTT_ZWin::CSCTT_ZWin(float eta, int from, int to, float dz)
{
	inDz = false; 
if(from ==0 && to == 1   && eta>=1.15 && eta<1.2 && dz>= 5.58827 && dz<= 6.33347) inDz=true; 
if(from ==0 && to == 1   && eta>=1.2 && eta<1.25 && dz>= 5.6545 && dz<= 6.82444) inDz=true; 
if(from ==0 && to == 1   && eta>=1.25 && eta<1.3 && dz>= 5.99488 && dz<= 7.19104) inDz=true; 
if(from ==0 && to == 1   && eta>=1.3 && eta<1.35 && dz>= 6.46882 && dz<= 7.52082) inDz=true; 
if(from ==0 && to == 1   && eta>=1.35 && eta<1.4 && dz>= 6.75902 && dz<= 8.04519) inDz=true; 
if(from ==0 && to == 1   && eta>=1.4 && eta<1.45 && dz>= 7.16234 && dz<= 8.53865) inDz=true; 
if(from ==0 && to == 1   && eta>=1.45 && eta<1.5 && dz>= 7.51489 && dz<= 8.99682) inDz=true; 
if(from ==0 && to == 1   && eta>=1.5 && eta<1.55 && dz>= 7.9722 && dz<= 9.58596) inDz=true; 
if(from ==0 && to == 1   && eta>=1.55 && eta<1.6 && dz>= 8.45486 && dz<= 10.4726) inDz=true; 
if(from ==0 && to == 1   && eta>=1.6 && eta<1.65 && dz>= 8.88722 && dz<= 10.6475) inDz=true; 
if(from ==0 && to == 1   && eta>=1.65 && eta<1.7 && dz>= 9.54357 && dz<= 11.0178) inDz=true; 
if(from ==0 && to == 1   && eta>=1.7 && eta<1.75 && dz>= 9.98229 && dz<= 11.705) inDz=true; 
if(from ==0 && to == 1   && eta>=1.75 && eta<1.8 && dz>= 10.5943 && dz<= 12.3316) inDz=true; 
if(from ==0 && to == 1   && eta>=1.8 && eta<1.85 && dz>= 11.1943 && dz<= 13.0385) inDz=true; 
if(from ==0 && to == 1   && eta>=1.85 && eta<1.9 && dz>= 11.743 && dz<= 13.8456) inDz=true; 
if(from ==0 && to == 1   && eta>=1.9 && eta<1.95 && dz>= 12.478 && dz<= 14.5257) inDz=true; 
if(from ==0 && to == 1   && eta>=1.95 && eta<2 && dz>= 13.0719 && dz<= 15.1988) inDz=true; 
if(from ==0 && to == 1   && eta>=2 && eta<2.05 && dz>= 13.7026 && dz<= 16.0463) inDz=true; 
if(from ==0 && to == 1   && eta>=2.05 && eta<2.1 && dz>= 14.3546 && dz<= 16.9436) inDz=true; 
if(from ==0 && to == 1   && eta>=2.1 && eta<2.15 && dz>= 15.1896 && dz<= 17.6746) inDz=true; 
if(from ==0 && to == 1   && eta>=2.15 && eta<2.2 && dz>= 16.1744 && dz<= 18.29) inDz=true; 
if(from ==0 && to == 1   && eta>=2.2 && eta<2.25 && dz>= 16.817 && dz<= 19.3893) inDz=true; 
if(from ==0 && to == 1   && eta>=2.25 && eta<2.3 && dz>= 17.7558 && dz<= 20.3954) inDz=true; 
if(from ==0 && to == 1   && eta>=2.3 && eta<2.35 && dz>= 18.8428 && dz<= 21.2302) inDz=true; 
if(from ==0 && to == 1   && eta>=2.35 && eta<2.4 && dz>= 19.5047 && dz<= 22.3988) inDz=true; 
if(from ==0 && to == 2   && eta>=1.2 && eta<1.25 && dz>= 23.0012 && dz<= 26.5863) inDz=true; 
if(from ==0 && to == 2   && eta>=1.25 && eta<1.3 && dz>= 24.2916 && dz<= 28.1087) inDz=true; 
if(from ==0 && to == 2   && eta>=1.3 && eta<1.35 && dz>= 25.7436 && dz<= 29.9162) inDz=true; 
if(from ==0 && to == 2   && eta>=1.35 && eta<1.4 && dz>= 27.2306 && dz<= 31.6294) inDz=true; 
if(from ==0 && to == 2   && eta>=1.4 && eta<1.45 && dz>= 28.9273 && dz<= 33.5265) inDz=true; 
if(from ==0 && to == 2   && eta>=1.45 && eta<1.5 && dz>= 30.5783 && dz<= 35.3053) inDz=true; 
if(from ==0 && to == 2   && eta>=1.5 && eta<1.55 && dz>= 32.1238 && dz<= 37.6693) inDz=true; 
if(from ==0 && to == 2   && eta>=1.55 && eta<1.6 && dz>= 34.0162 && dz<= 41.3884) inDz=true; 
if(from ==0 && to == 2   && eta>=1.6 && eta<1.65 && dz>= 35.71 && dz<= 42.3044) inDz=true; 
if(from ==0 && to == 2   && eta>=1.65 && eta<1.7 && dz>= 38.187 && dz<= 43.8276) inDz=true; 
if(from ==0 && to == 2   && eta>=1.7 && eta<1.75 && dz>= 40.1619 && dz<= 46.2247) inDz=true; 
if(from ==0 && to == 2   && eta>=1.75 && eta<1.8 && dz>= 42.6162 && dz<= 48.9213) inDz=true; 
if(from ==0 && to == 2   && eta>=1.8 && eta<1.85 && dz>= 44.9128 && dz<= 51.5945) inDz=true; 
if(from ==0 && to == 2   && eta>=1.85 && eta<1.9 && dz>= 47.5624 && dz<= 54.5609) inDz=true; 
if(from ==0 && to == 2   && eta>=1.9 && eta<1.95 && dz>= 50.3351 && dz<= 57.2327) inDz=true; 
if(from ==0 && to == 2   && eta>=1.95 && eta<2 && dz>= 52.7063 && dz<= 60.1469) inDz=true; 
if(from ==0 && to == 2   && eta>=2 && eta<2.05 && dz>= 55.7308 && dz<= 63.08) inDz=true; 
if(from ==0 && to == 2   && eta>=2.05 && eta<2.1 && dz>= 58.2248 && dz<= 66.7919) inDz=true; 
if(from ==0 && to == 2   && eta>=2.1 && eta<2.15 && dz>= 61.5113 && dz<= 69.9007) inDz=true; 
if(from ==0 && to == 2   && eta>=2.15 && eta<2.2 && dz>= 64.5467 && dz<= 73.7287) inDz=true; 
if(from ==0 && to == 2   && eta>=2.2 && eta<2.25 && dz>= 67.7734 && dz<= 77.3992) inDz=true; 
if(from ==0 && to == 2   && eta>=2.25 && eta<2.3 && dz>= 71.4496 && dz<= 82.1919) inDz=true; 
if(from ==0 && to == 2   && eta>=2.3 && eta<2.35 && dz>= 75.4494 && dz<= 86.5204) inDz=true; 
if(from ==0 && to == 2   && eta>=2.35 && eta<2.4 && dz>= 79.4826 && dz<= 89.4669) inDz=true; 
if(from ==0 && to == 2   && eta>=2.4 && eta<2.45 && dz>= 85.0861 && dz<= 85.6739) inDz=true; 
if(from ==0 && to == 3   && eta>=1.2 && eta<1.25 && dz>= 28.9226 && dz<= 32.9783) inDz=true; 
if(from ==0 && to == 3   && eta>=1.25 && eta<1.3 && dz>= 30.5553 && dz<= 34.9614) inDz=true; 
if(from ==0 && to == 3   && eta>=1.3 && eta<1.35 && dz>= 32.3714 && dz<= 37.2368) inDz=true; 
if(from ==0 && to == 3   && eta>=1.35 && eta<1.4 && dz>= 34.3418 && dz<= 39.3067) inDz=true; 
if(from ==0 && to == 3   && eta>=1.4 && eta<1.45 && dz>= 36.3322 && dz<= 41.7253) inDz=true; 
if(from ==0 && to == 3   && eta>=1.45 && eta<1.5 && dz>= 38.332 && dz<= 43.9413) inDz=true; 
if(from ==0 && to == 3   && eta>=1.5 && eta<1.55 && dz>= 40.4007 && dz<= 46.9215) inDz=true; 
if(from ==0 && to == 3   && eta>=1.55 && eta<1.6 && dz>= 42.7354 && dz<= 51.5731) inDz=true; 
if(from ==0 && to == 3   && eta>=1.6 && eta<1.65 && dz>= 44.9623 && dz<= 52.5602) inDz=true; 
if(from ==0 && to == 3   && eta>=1.65 && eta<1.7 && dz>= 47.9924 && dz<= 54.4151) inDz=true; 
if(from ==0 && to == 3   && eta>=1.7 && eta<1.75 && dz>= 50.4283 && dz<= 57.5296) inDz=true; 
if(from ==0 && to == 3   && eta>=1.75 && eta<1.8 && dz>= 53.4763 && dz<= 60.7868) inDz=true; 
if(from ==0 && to == 3   && eta>=1.8 && eta<1.85 && dz>= 56.5657 && dz<= 64.0974) inDz=true; 
if(from ==0 && to == 3   && eta>=1.85 && eta<1.9 && dz>= 59.7748 && dz<= 67.8071) inDz=true; 
if(from ==0 && to == 3   && eta>=1.9 && eta<1.95 && dz>= 63.1877 && dz<= 71.1396) inDz=true; 
if(from ==0 && to == 3   && eta>=1.95 && eta<2 && dz>= 66.3291 && dz<= 74.4822) inDz=true; 
if(from ==0 && to == 3   && eta>=2 && eta<2.05 && dz>= 69.9258 && dz<= 78.3281) inDz=true; 
if(from ==0 && to == 3   && eta>=2.05 && eta<2.1 && dz>= 72.9313 && dz<= 82.6708) inDz=true; 
if(from ==0 && to == 3   && eta>=2.1 && eta<2.15 && dz>= 76.9386 && dz<= 86.2033) inDz=true; 
if(from ==0 && to == 3   && eta>=2.15 && eta<2.2 && dz>= 81.3007 && dz<= 90.8335) inDz=true; 
if(from ==0 && to == 3   && eta>=2.2 && eta<2.25 && dz>= 85.4524 && dz<= 94.7514) inDz=true; 
if(from ==0 && to == 3   && eta>=2.25 && eta<2.3 && dz>= 89.2532 && dz<= 100.153) inDz=true; 
if(from ==0 && to == 3   && eta>=2.3 && eta<2.35 && dz>= 94.9705 && dz<= 104.535) inDz=true; 
if(from ==0 && to == 4   && eta>=1.85 && eta<1.9 && dz>= 98.8764 && dz<= 109.229) inDz=true; 
if(from ==0 && to == 4   && eta>=1.9 && eta<1.95 && dz>= 102.925 && dz<= 114.572) inDz=true; 
if(from ==0 && to == 4   && eta>=1.95 && eta<2 && dz>= 108.35 && dz<= 119.671) inDz=true; 
if(from ==0 && to == 4   && eta>=2 && eta<2.05 && dz>= 113.981 && dz<= 125.837) inDz=true; 
if(from ==0 && to == 4   && eta>=2.05 && eta<2.1 && dz>= 118.997 && dz<= 133.405) inDz=true; 
if(from ==0 && to == 4   && eta>=2.1 && eta<2.15 && dz>= 126.089 && dz<= 135.206) inDz=true; 
if(from ==0 && to == 5   && eta>=1.8 && eta<1.85 && dz>= 106.108 && dz<= 115.492) inDz=true; 
if(from ==0 && to == 5   && eta>=1.85 && eta<1.9 && dz>= 109.496 && dz<= 122.309) inDz=true; 
if(from ==0 && to == 5   && eta>=1.9 && eta<1.95 && dz>= 115.677 && dz<= 128.578) inDz=true; 
if(from ==0 && to == 5   && eta>=1.95 && eta<2 && dz>= 121.792 && dz<= 134.524) inDz=true; 
if(from ==0 && to == 5   && eta>=2 && eta<2.05 && dz>= 128.221 && dz<= 141.164) inDz=true; 
if(from ==0 && to == 5   && eta>=2.05 && eta<2.1 && dz>= 134.19 && dz<= 143.683) inDz=true; 
if(from ==0 && to == 6   && eta>=1.65 && eta<1.7 && dz>= 120.089 && dz<= 130.253) inDz=true; 
if(from ==0 && to == 6   && eta>=1.7 && eta<1.75 && dz>= 123.779 && dz<= 137.606) inDz=true; 
if(from ==0 && to == 6   && eta>=1.75 && eta<1.8 && dz>= 131.014 && dz<= 145.196) inDz=true; 
if(from ==0 && to == 6   && eta>=1.8 && eta<1.85 && dz>= 137.972 && dz<= 153.584) inDz=true; 
if(from ==0 && to == 6   && eta>=1.85 && eta<1.9 && dz>= 145.955 && dz<= 161.497) inDz=true; 
if(from ==0 && to == 6   && eta>=1.9 && eta<1.95 && dz>= 154.454 && dz<= 164.368) inDz=true; 
if(from ==0 && to == 7   && eta>=1.55 && eta<1.6 && dz>= 122.974 && dz<= 133.983) inDz=true; 
if(from ==0 && to == 7   && eta>=1.6 && eta<1.65 && dz>= 124.949 && dz<= 133.162) inDz=true; 
if(from ==0 && to == 7   && eta>=1.65 && eta<1.7 && dz>= 127.215 && dz<= 141.339) inDz=true; 
if(from ==0 && to == 7   && eta>=1.7 && eta<1.75 && dz>= 133.818 && dz<= 148.949) inDz=true; 
if(from ==0 && to == 7   && eta>=1.75 && eta<1.8 && dz>= 141.803 && dz<= 157.213) inDz=true; 
if(from ==0 && to == 7   && eta>=1.8 && eta<1.85 && dz>= 149.124 && dz<= 166.311) inDz=true; 
if(from ==0 && to == 7   && eta>=1.85 && eta<1.9 && dz>= 158.58 && dz<= 168.913) inDz=true; 
if(from ==0 && to == 8   && eta>=1.2 && eta<1.25 && dz>= 97.3675 && dz<= 109.199) inDz=true; 
if(from ==0 && to == 8   && eta>=1.25 && eta<1.3 && dz>= 103.027 && dz<= 115.031) inDz=true; 
if(from ==0 && to == 8   && eta>=1.3 && eta<1.35 && dz>= 109.189 && dz<= 122.821) inDz=true; 
if(from ==0 && to == 8   && eta>=1.35 && eta<1.4 && dz>= 115.822 && dz<= 129.565) inDz=true; 
if(from ==0 && to == 8   && eta>=1.4 && eta<1.45 && dz>= 122.706 && dz<= 137.163) inDz=true; 
if(from ==0 && to == 8   && eta>=1.45 && eta<1.5 && dz>= 130.24 && dz<= 144.099) inDz=true; 
if(from ==0 && to == 8   && eta>=1.5 && eta<1.55 && dz>= 136.356 && dz<= 154.557) inDz=true; 
if(from ==0 && to == 8   && eta>=1.55 && eta<1.6 && dz>= 143.262 && dz<= 170.637) inDz=true; 
if(from ==0 && to == 8   && eta>=1.6 && eta<1.65 && dz>= 155.204 && dz<= 171.039) inDz=true; 
if(from ==0 && to == 8   && eta>=1.65 && eta<1.7 && dz>= 161.435 && dz<= 179.434) inDz=true; 
if(from ==0 && to == 8   && eta>=1.7 && eta<1.75 && dz>= 170.279 && dz<= 183.053) inDz=true; 
if(from ==0 && to == 9   && eta>=1.15 && eta<1.2 && dz>= 98.594 && dz<= 109.413) inDz=true; 
if(from ==0 && to == 9   && eta>=1.2 && eta<1.25 && dz>= 102.546 && dz<= 114.982) inDz=true; 
if(from ==0 && to == 9   && eta>=1.25 && eta<1.3 && dz>= 108.334 && dz<= 121.119) inDz=true; 
if(from ==0 && to == 9   && eta>=1.3 && eta<1.35 && dz>= 114.88 && dz<= 129.325) inDz=true; 
if(from ==0 && to == 9   && eta>=1.35 && eta<1.4 && dz>= 121.84 && dz<= 136.41) inDz=true; 
if(from ==0 && to == 9   && eta>=1.4 && eta<1.45 && dz>= 129.101 && dz<= 144.423) inDz=true; 
if(from ==0 && to == 9   && eta>=1.45 && eta<1.5 && dz>= 136.855 && dz<= 151.927) inDz=true; 
if(from ==0 && to == 9   && eta>=1.5 && eta<1.55 && dz>= 143.41 && dz<= 162.755) inDz=true; 
if(from ==0 && to == 9   && eta>=1.55 && eta<1.6 && dz>= 150.966 && dz<= 179.028) inDz=true; 
if(from ==0 && to == 9   && eta>=1.6 && eta<1.65 && dz>= 158.616 && dz<= 182.707) inDz=true; 
if(from ==0 && to == 9   && eta>=1.65 && eta<1.7 && dz>= 170.135 && dz<= 186.03) inDz=true; 
if(from ==1 && to == 2   && eta>=1.2 && eta<1.25 && dz>= 17.0949 && dz<= 20.1116) inDz=true; 
if(from ==1 && to == 2   && eta>=1.25 && eta<1.3 && dz>= 18.0108 && dz<= 21.3241) inDz=true; 
if(from ==1 && to == 2   && eta>=1.3 && eta<1.35 && dz>= 19.089 && dz<= 22.6226) inDz=true; 
if(from ==1 && to == 2   && eta>=1.35 && eta<1.4 && dz>= 20.2206 && dz<= 23.9215) inDz=true; 
if(from ==1 && to == 2   && eta>=1.4 && eta<1.45 && dz>= 21.4505 && dz<= 25.3832) inDz=true; 
if(from ==1 && to == 2   && eta>=1.45 && eta<1.5 && dz>= 22.6804 && dz<= 26.7689) inDz=true; 
if(from ==1 && to == 2   && eta>=1.5 && eta<1.55 && dz>= 23.8348 && dz<= 28.4989) inDz=true; 
if(from ==1 && to == 2   && eta>=1.55 && eta<1.6 && dz>= 25.2474 && dz<= 31.1991) inDz=true; 
if(from ==1 && to == 2   && eta>=1.6 && eta<1.65 && dz>= 26.2986 && dz<= 32.0613) inDz=true; 
if(from ==1 && to == 2   && eta>=1.65 && eta<1.7 && dz>= 28.3458 && dz<= 33.1876) inDz=true; 
if(from ==1 && to == 2   && eta>=1.7 && eta<1.75 && dz>= 29.7628 && dz<= 35.0509) inDz=true; 
if(from ==1 && to == 2   && eta>=1.75 && eta<1.8 && dz>= 31.5009 && dz<= 37.0855) inDz=true; 
if(from ==1 && to == 2   && eta>=1.8 && eta<1.85 && dz>= 33.1965 && dz<= 39.0946) inDz=true; 
if(from ==1 && to == 2   && eta>=1.85 && eta<1.9 && dz>= 35.2812 && dz<= 41.3796) inDz=true; 
if(from ==1 && to == 2   && eta>=1.9 && eta<1.95 && dz>= 37.2555 && dz<= 43.3573) inDz=true; 
if(from ==1 && to == 2   && eta>=1.95 && eta<2 && dz>= 38.9174 && dz<= 45.5636) inDz=true; 
if(from ==1 && to == 2   && eta>=2 && eta<2.05 && dz>= 41.1678 && dz<= 47.7602) inDz=true; 
if(from ==1 && to == 2   && eta>=2.05 && eta<2.1 && dz>= 43.0847 && dz<= 50.4865) inDz=true; 
if(from ==1 && to == 2   && eta>=2.1 && eta<2.15 && dz>= 45.4068 && dz<= 53.0256) inDz=true; 
if(from ==1 && to == 2   && eta>=2.15 && eta<2.2 && dz>= 47.7075 && dz<= 55.7202) inDz=true; 
if(from ==1 && to == 2   && eta>=2.2 && eta<2.25 && dz>= 50.4421 && dz<= 57.9534) inDz=true; 
if(from ==1 && to == 2   && eta>=2.25 && eta<2.3 && dz>= 52.6984 && dz<= 61.573) inDz=true; 
if(from ==1 && to == 2   && eta>=2.3 && eta<2.35 && dz>= 55.5995 && dz<= 65.0779) inDz=true; 
if(from ==1 && to == 2   && eta>=2.35 && eta<2.4 && dz>= 59.2679 && dz<= 67.5029) inDz=true; 
if(from ==1 && to == 3   && eta>=1.15 && eta<1.2 && dz>= 22.87 && dz<= 25.6729) inDz=true; 
if(from ==1 && to == 3   && eta>=1.2 && eta<1.25 && dz>= 23.0131 && dz<= 26.5438) inDz=true; 
if(from ==1 && to == 3   && eta>=1.25 && eta<1.3 && dz>= 24.2839 && dz<= 28.1302) inDz=true; 
if(from ==1 && to == 3   && eta>=1.3 && eta<1.35 && dz>= 25.6984 && dz<= 29.9343) inDz=true; 
if(from ==1 && to == 3   && eta>=1.35 && eta<1.4 && dz>= 27.4117 && dz<= 31.5356) inDz=true; 
if(from ==1 && to == 3   && eta>=1.4 && eta<1.45 && dz>= 28.8979 && dz<= 33.556) inDz=true; 
if(from ==1 && to == 3   && eta>=1.45 && eta<1.5 && dz>= 30.5795 && dz<= 35.3229) inDz=true; 
if(from ==1 && to == 3   && eta>=1.5 && eta<1.55 && dz>= 32.1971 && dz<= 37.6413) inDz=true; 
if(from ==1 && to == 3   && eta>=1.55 && eta<1.6 && dz>= 34.0161 && dz<= 41.3327) inDz=true; 
if(from ==1 && to == 3   && eta>=1.6 && eta<1.65 && dz>= 35.5089 && dz<= 42.2936) inDz=true; 
if(from ==1 && to == 3   && eta>=1.65 && eta<1.7 && dz>= 38.2274 && dz<= 43.7488) inDz=true; 
if(from ==1 && to == 3   && eta>=1.7 && eta<1.75 && dz>= 40.0789 && dz<= 46.3467) inDz=true; 
if(from ==1 && to == 3   && eta>=1.75 && eta<1.8 && dz>= 42.4342 && dz<= 48.9149) inDz=true; 
if(from ==1 && to == 3   && eta>=1.8 && eta<1.85 && dz>= 44.8621 && dz<= 51.6052) inDz=true; 
if(from ==1 && to == 3   && eta>=1.85 && eta<1.9 && dz>= 47.5694 && dz<= 54.6032) inDz=true; 
if(from ==1 && to == 3   && eta>=1.9 && eta<1.95 && dz>= 50.2474 && dz<= 57.1669) inDz=true; 
if(from ==1 && to == 3   && eta>=1.95 && eta<2 && dz>= 52.7002 && dz<= 60.161) inDz=true; 
if(from ==1 && to == 3   && eta>=2 && eta<2.05 && dz>= 55.6173 && dz<= 62.8833) inDz=true; 
if(from ==1 && to == 3   && eta>=2.05 && eta<2.1 && dz>= 57.9619 && dz<= 66.6055) inDz=true; 
if(from ==1 && to == 3   && eta>=2.1 && eta<2.15 && dz>= 61.2666 && dz<= 69.458) inDz=true; 
if(from ==1 && to == 3   && eta>=2.15 && eta<2.2 && dz>= 64.6057 && dz<= 73.3897) inDz=true; 
if(from ==1 && to == 3   && eta>=2.2 && eta<2.25 && dz>= 68.1733 && dz<= 76.3131) inDz=true; 
if(from ==1 && to == 3   && eta>=2.25 && eta<2.3 && dz>= 71.2101 && dz<= 80.4851) inDz=true; 
if(from ==1 && to == 3   && eta>=2.3 && eta<2.35 && dz>= 75.4838 && dz<= 84.1556) inDz=true; 
if(from ==1 && to == 4   && eta>=1.85 && eta<1.9 && dz>= 87.2899 && dz<= 95.7952) inDz=true; 
if(from ==1 && to == 4   && eta>=1.9 && eta<1.95 && dz>= 89.8982 && dz<= 100.43) inDz=true; 
if(from ==1 && to == 4   && eta>=1.95 && eta<2 && dz>= 94.6659 && dz<= 104.903) inDz=true; 
if(from ==1 && to == 4   && eta>=2 && eta<2.05 && dz>= 99.3965 && dz<= 110.294) inDz=true; 
if(from ==1 && to == 4   && eta>=2.05 && eta<2.1 && dz>= 103.639 && dz<= 117.006) inDz=true; 
if(from ==1 && to == 4   && eta>=2.1 && eta<2.15 && dz>= 110.171 && dz<= 118.76) inDz=true; 
if(from ==1 && to == 5   && eta>=1.8 && eta<1.85 && dz>= 94.1101 && dz<= 103.071) inDz=true; 
if(from ==1 && to == 5   && eta>=1.85 && eta<1.9 && dz>= 97.2867 && dz<= 108.849) inDz=true; 
if(from ==1 && to == 5   && eta>=1.9 && eta<1.95 && dz>= 102.635 && dz<= 114.479) inDz=true; 
if(from ==1 && to == 5   && eta>=1.95 && eta<2 && dz>= 107.948 && dz<= 119.851) inDz=true; 
if(from ==1 && to == 5   && eta>=2 && eta<2.05 && dz>= 113.636 && dz<= 125.786) inDz=true; 
if(from ==1 && to == 5   && eta>=2.05 && eta<2.1 && dz>= 119.188 && dz<= 127.873) inDz=true; 
if(from ==1 && to == 6   && eta>=1.65 && eta<1.7 && dz>= 110.197 && dz<= 119.545) inDz=true; 
if(from ==1 && to == 6   && eta>=1.7 && eta<1.75 && dz>= 113.291 && dz<= 126.459) inDz=true; 
if(from ==1 && to == 6   && eta>=1.75 && eta<1.8 && dz>= 119.951 && dz<= 133.202) inDz=true; 
if(from ==1 && to == 6   && eta>=1.8 && eta<1.85 && dz>= 126.272 && dz<= 140.864) inDz=true; 
if(from ==1 && to == 6   && eta>=1.85 && eta<1.9 && dz>= 134.049 && dz<= 148.159) inDz=true; 
if(from ==1 && to == 6   && eta>=1.9 && eta<1.95 && dz>= 140.716 && dz<= 151.254) inDz=true; 
if(from ==1 && to == 7   && eta>=1.55 && eta<1.6 && dz>= 114.537 && dz<= 123.525) inDz=true; 
if(from ==1 && to == 7   && eta>=1.6 && eta<1.65 && dz>= 115.653 && dz<= 122.857) inDz=true; 
if(from ==1 && to == 7   && eta>=1.65 && eta<1.7 && dz>= 117.403 && dz<= 130.613) inDz=true; 
if(from ==1 && to == 7   && eta>=1.7 && eta<1.75 && dz>= 123.321 && dz<= 137.751) inDz=true; 
if(from ==1 && to == 7   && eta>=1.75 && eta<1.8 && dz>= 130.753 && dz<= 145.238) inDz=true; 
if(from ==1 && to == 7   && eta>=1.8 && eta<1.85 && dz>= 137.543 && dz<= 153.602) inDz=true; 
if(from ==1 && to == 7   && eta>=1.85 && eta<1.9 && dz>= 146.351 && dz<= 156.308) inDz=true; 
if(from ==1 && to == 8   && eta>=1.2 && eta<1.25 && dz>= 91.8277 && dz<= 102.624) inDz=true; 
if(from ==1 && to == 8   && eta>=1.25 && eta<1.3 && dz>= 96.7512 && dz<= 108.31) inDz=true; 
if(from ==1 && to == 8   && eta>=1.3 && eta<1.35 && dz>= 102.63 && dz<= 115.363) inDz=true; 
if(from ==1 && to == 8   && eta>=1.35 && eta<1.4 && dz>= 108.834 && dz<= 121.914) inDz=true; 
if(from ==1 && to == 8   && eta>=1.4 && eta<1.45 && dz>= 115.144 && dz<= 129.042) inDz=true; 
if(from ==1 && to == 8   && eta>=1.45 && eta<1.5 && dz>= 122.382 && dz<= 135.475) inDz=true; 
if(from ==1 && to == 8   && eta>=1.5 && eta<1.55 && dz>= 128.247 && dz<= 145.187) inDz=true; 
if(from ==1 && to == 8   && eta>=1.55 && eta<1.6 && dz>= 134.727 && dz<= 160.144) inDz=true; 
if(from ==1 && to == 8   && eta>=1.6 && eta<1.65 && dz>= 145.102 && dz<= 161.212) inDz=true; 
if(from ==1 && to == 8   && eta>=1.65 && eta<1.7 && dz>= 151.818 && dz<= 168.637) inDz=true; 
if(from ==1 && to == 8   && eta>=1.7 && eta<1.75 && dz>= 160.17 && dz<= 171.927) inDz=true; 
if(from ==1 && to == 9   && eta>=1.15 && eta<1.2 && dz>= 92.8015 && dz<= 102.943) inDz=true; 
if(from ==1 && to == 9   && eta>=1.2 && eta<1.25 && dz>= 97.1091 && dz<= 108.271) inDz=true; 
if(from ==1 && to == 9   && eta>=1.25 && eta<1.3 && dz>= 102.145 && dz<= 114.415) inDz=true; 
if(from ==1 && to == 9   && eta>=1.3 && eta<1.35 && dz>= 108.364 && dz<= 121.883) inDz=true; 
if(from ==1 && to == 9   && eta>=1.35 && eta<1.4 && dz>= 114.846 && dz<= 128.757) inDz=true; 
if(from ==1 && to == 9   && eta>=1.4 && eta<1.45 && dz>= 121.588 && dz<= 136.295) inDz=true; 
if(from ==1 && to == 9   && eta>=1.45 && eta<1.5 && dz>= 129.07 && dz<= 143.229) inDz=true; 
if(from ==1 && to == 9   && eta>=1.5 && eta<1.55 && dz>= 135.4 && dz<= 153.114) inDz=true; 
if(from ==1 && to == 9   && eta>=1.55 && eta<1.6 && dz>= 142.351 && dz<= 168.698) inDz=true; 
if(from ==1 && to == 9   && eta>=1.6 && eta<1.65 && dz>= 148.665 && dz<= 172.66) inDz=true; 
if(from ==1 && to == 9   && eta>=1.65 && eta<1.7 && dz>= 160.5 && dz<= 175.394) inDz=true; 
if(from ==2 && to == 3   && eta>=1.2 && eta<1.25 && dz>= 5.48823 && dz<= 6.92562) inDz=true; 
if(from ==2 && to == 3   && eta>=1.25 && eta<1.3 && dz>= 5.77493 && dz<= 7.36946) inDz=true; 
if(from ==2 && to == 3   && eta>=1.3 && eta<1.35 && dz>= 6.17609 && dz<= 7.76986) inDz=true; 
if(from ==2 && to == 3   && eta>=1.35 && eta<1.4 && dz>= 6.49119 && dz<= 8.26029) inDz=true; 
if(from ==2 && to == 3   && eta>=1.4 && eta<1.45 && dz>= 6.92805 && dz<= 8.73805) inDz=true; 
if(from ==2 && to == 3   && eta>=1.45 && eta<1.5 && dz>= 7.2133 && dz<= 9.25402) inDz=true; 
if(from ==2 && to == 3   && eta>=1.5 && eta<1.55 && dz>= 7.66984 && dz<= 9.84934) inDz=true; 
if(from ==2 && to == 3   && eta>=1.55 && eta<1.6 && dz>= 8.13441 && dz<= 10.7206) inDz=true; 
if(from ==2 && to == 3   && eta>=1.6 && eta<1.65 && dz>= 8.52229 && dz<= 10.9594) inDz=true; 
if(from ==2 && to == 3   && eta>=1.65 && eta<1.7 && dz>= 9.09586 && dz<= 11.4467) inDz=true; 
if(from ==2 && to == 3   && eta>=1.7 && eta<1.75 && dz>= 9.58661 && dz<= 12.0252) inDz=true; 
if(from ==2 && to == 3   && eta>=1.75 && eta<1.8 && dz>= 10.204 && dz<= 12.6965) inDz=true; 
if(from ==2 && to == 3   && eta>=1.8 && eta<1.85 && dz>= 10.6856 && dz<= 13.4961) inDz=true; 
if(from ==2 && to == 3   && eta>=1.85 && eta<1.9 && dz>= 11.2679 && dz<= 14.3006) inDz=true; 
if(from ==2 && to == 3   && eta>=1.9 && eta<1.95 && dz>= 11.8951 && dz<= 14.9647) inDz=true; 
if(from ==2 && to == 3   && eta>=1.95 && eta<2 && dz>= 12.711 && dz<= 15.5763) inDz=true; 
if(from ==2 && to == 3   && eta>=2 && eta<2.05 && dz>= 13.2515 && dz<= 16.4521) inDz=true; 
if(from ==2 && to == 3   && eta>=2.05 && eta<2.1 && dz>= 13.929 && dz<= 17.3178) inDz=true; 
if(from ==2 && to == 3   && eta>=2.1 && eta<2.15 && dz>= 14.5546 && dz<= 18.184) inDz=true; 
if(from ==2 && to == 3   && eta>=2.15 && eta<2.2 && dz>= 15.2695 && dz<= 19.1784) inDz=true; 
if(from ==2 && to == 3   && eta>=2.2 && eta<2.25 && dz>= 16.0706 && dz<= 20.0406) inDz=true; 
if(from ==2 && to == 3   && eta>=2.25 && eta<2.3 && dz>= 17.1119 && dz<= 20.8754) inDz=true; 
if(from ==2 && to == 3   && eta>=2.3 && eta<2.35 && dz>= 18.0319 && dz<= 21.8539) inDz=true; 
if(from ==2 && to == 4   && eta>=1.85 && eta<1.9 && dz>= 49.4329 && dz<= 55.7017) inDz=true; 
if(from ==2 && to == 4   && eta>=1.9 && eta<1.95 && dz>= 51.2803 && dz<= 58.3673) inDz=true; 
if(from ==2 && to == 4   && eta>=1.95 && eta<2 && dz>= 54.1504 && dz<= 61.1615) inDz=true; 
if(from ==2 && to == 4   && eta>=2 && eta<2.05 && dz>= 56.8241 && dz<= 64.1469) inDz=true; 
if(from ==2 && to == 4   && eta>=2.05 && eta<2.1 && dz>= 59.5764 && dz<= 67.9088) inDz=true; 
if(from ==2 && to == 4   && eta>=2.1 && eta<2.15 && dz>= 62.6597 && dz<= 69.4651) inDz=true; 
if(from ==2 && to == 5   && eta>=1.8 && eta<1.85 && dz>= 58.7479 && dz<= 65.1456) inDz=true; 
if(from ==2 && to == 5   && eta>=1.85 && eta<1.9 && dz>= 60.9237 && dz<= 68.7662) inDz=true; 
if(from ==2 && to == 5   && eta>=1.9 && eta<1.95 && dz>= 64.1387 && dz<= 72.4197) inDz=true; 
if(from ==2 && to == 5   && eta>=1.95 && eta<2 && dz>= 67.6817 && dz<= 75.8626) inDz=true; 
if(from ==2 && to == 5   && eta>=2 && eta<2.05 && dz>= 71.0876 && dz<= 79.5019) inDz=true; 
if(from ==2 && to == 5   && eta>=2.05 && eta<2.1 && dz>= 73.9352 && dz<= 81.2528) inDz=true; 
if(from ==2 && to == 6   && eta>=1.65 && eta<1.7 && dz>= 80.1431 && dz<= 87.2624) inDz=true; 
if(from ==2 && to == 6   && eta>=1.7 && eta<1.75 && dz>= 82.5623 && dz<= 92.3236) inDz=true; 
if(from ==2 && to == 6   && eta>=1.75 && eta<1.8 && dz>= 87.5106 && dz<= 97.1701) inDz=true; 
if(from ==2 && to == 6   && eta>=1.8 && eta<1.85 && dz>= 92.1214 && dz<= 102.89) inDz=true; 
if(from ==2 && to == 6   && eta>=1.85 && eta<1.9 && dz>= 97.4128 && dz<= 108.179) inDz=true; 
if(from ==2 && to == 6   && eta>=1.9 && eta<1.95 && dz>= 101.97 && dz<= 110.98) inDz=true; 
if(from ==2 && to == 7   && eta>=1.55 && eta<1.6 && dz>= 85.624 && dz<= 92.9423) inDz=true; 
if(from ==2 && to == 7   && eta>=1.6 && eta<1.65 && dz>= 86.2719 && dz<= 92.7227) inDz=true; 
if(from ==2 && to == 7   && eta>=1.65 && eta<1.7 && dz>= 88.1904 && dz<= 98.2399) inDz=true; 
if(from ==2 && to == 7   && eta>=1.7 && eta<1.75 && dz>= 92.7658 && dz<= 103.6) inDz=true; 
if(from ==2 && to == 7   && eta>=1.75 && eta<1.8 && dz>= 98.2886 && dz<= 109.183) inDz=true; 
if(from ==2 && to == 7   && eta>=1.8 && eta<1.85 && dz>= 103.411 && dz<= 115.51) inDz=true; 
if(from ==2 && to == 7   && eta>=1.85 && eta<1.9 && dz>= 110.052 && dz<= 117.3) inDz=true; 
if(from ==2 && to == 8   && eta>=1.15 && eta<1.2 && dz>= 69.2666 && dz<= 77.2191) inDz=true; 
if(from ==2 && to == 8   && eta>=1.2 && eta<1.25 && dz>= 74.0462 && dz<= 82.9951) inDz=true; 
if(from ==2 && to == 8   && eta>=1.25 && eta<1.3 && dz>= 78.2787 && dz<= 87.6993) inDz=true; 
if(from ==2 && to == 8   && eta>=1.3 && eta<1.35 && dz>= 82.8325 && dz<= 93.3708) inDz=true; 
if(from ==2 && to == 8   && eta>=1.35 && eta<1.4 && dz>= 87.9312 && dz<= 98.5697) inDz=true; 
if(from ==2 && to == 8   && eta>=1.4 && eta<1.45 && dz>= 93.2596 && dz<= 104.148) inDz=true; 
if(from ==2 && to == 8   && eta>=1.45 && eta<1.5 && dz>= 98.7858 && dz<= 109.626) inDz=true; 
if(from ==2 && to == 8   && eta>=1.5 && eta<1.55 && dz>= 103.447 && dz<= 117.411) inDz=true; 
if(from ==2 && to == 8   && eta>=1.55 && eta<1.6 && dz>= 108.909 && dz<= 129.562) inDz=true; 
if(from ==2 && to == 8   && eta>=1.6 && eta<1.65 && dz>= 117.043 && dz<= 130.431) inDz=true; 
if(from ==2 && to == 8   && eta>=1.65 && eta<1.7 && dz>= 122.781 && dz<= 136.285) inDz=true; 
if(from ==2 && to == 8   && eta>=1.7 && eta<1.75 && dz>= 129.748 && dz<= 138.796) inDz=true; 
if(from ==2 && to == 9   && eta>=1.15 && eta<1.2 && dz>= 74.688 && dz<= 83.416) inDz=true; 
if(from ==2 && to == 9   && eta>=1.2 && eta<1.25 && dz>= 79.4004 && dz<= 88.6539) inDz=true; 
if(from ==2 && to == 9   && eta>=1.25 && eta<1.3 && dz>= 83.6784 && dz<= 93.8495) inDz=true; 
if(from ==2 && to == 9   && eta>=1.3 && eta<1.35 && dz>= 88.6096 && dz<= 99.8503) inDz=true; 
if(from ==2 && to == 9   && eta>=1.35 && eta<1.4 && dz>= 93.9934 && dz<= 105.381) inDz=true; 
if(from ==2 && to == 9   && eta>=1.4 && eta<1.45 && dz>= 99.7445 && dz<= 111.362) inDz=true; 
if(from ==2 && to == 9   && eta>=1.45 && eta<1.5 && dz>= 105.341 && dz<= 117.427) inDz=true; 
if(from ==2 && to == 9   && eta>=1.5 && eta<1.55 && dz>= 110.612 && dz<= 125.396) inDz=true; 
if(from ==2 && to == 9   && eta>=1.55 && eta<1.6 && dz>= 116.449 && dz<= 138.227) inDz=true; 
if(from ==2 && to == 9   && eta>=1.6 && eta<1.65 && dz>= 122.049 && dz<= 141.017) inDz=true; 
if(from ==2 && to == 9   && eta>=1.65 && eta<1.7 && dz>= 131.352 && dz<= 143.449) inDz=true; 
if(from ==3 && to == 4   && eta>=1.85 && eta<1.9 && dz>= 37.0162 && dz<= 42.4689) inDz=true; 
if(from ==3 && to == 4   && eta>=1.9 && eta<1.95 && dz>= 38.2118 && dz<= 44.468) inDz=true; 
if(from ==3 && to == 4   && eta>=1.95 && eta<2 && dz>= 40.4045 && dz<= 46.4807) inDz=true; 
if(from ==3 && to == 4   && eta>=2 && eta<2.05 && dz>= 42.297 && dz<= 48.9094) inDz=true; 
if(from ==3 && to == 4   && eta>=2.05 && eta<2.1 && dz>= 44.4401 && dz<= 51.5988) inDz=true; 
if(from ==3 && to == 4   && eta>=2.1 && eta<2.15 && dz>= 46.7511 && dz<= 52.9937) inDz=true; 
if(from ==3 && to == 5   && eta>=1.8 && eta<1.85 && dz>= 46.7397 && dz<= 52.7095) inDz=true; 
if(from ==3 && to == 5   && eta>=1.85 && eta<1.9 && dz>= 48.6416 && dz<= 55.417) inDz=true; 
if(from ==3 && to == 5   && eta>=1.9 && eta<1.95 && dz>= 51.0387 && dz<= 58.4301) inDz=true; 
if(from ==3 && to == 5   && eta>=1.95 && eta<2 && dz>= 54.0338 && dz<= 61.1352) inDz=true; 
if(from ==3 && to == 5   && eta>=2 && eta<2.05 && dz>= 56.6023 && dz<= 64.2986) inDz=true; 
if(from ==3 && to == 5   && eta>=2.05 && eta<2.1 && dz>= 59.3545 && dz<= 65.383) inDz=true; 
if(from ==3 && to == 6   && eta>=1.65 && eta<1.7 && dz>= 70.2025 && dz<= 76.4122) inDz=true; 
if(from ==3 && to == 6   && eta>=1.7 && eta<1.75 && dz>= 72.1524 && dz<= 81.1149) inDz=true; 
if(from ==3 && to == 6   && eta>=1.75 && eta<1.8 && dz>= 76.501 && dz<= 85.0379) inDz=true; 
if(from ==3 && to == 6   && eta>=1.8 && eta<1.85 && dz>= 80.4897 && dz<= 90.2569) inDz=true; 
if(from ==3 && to == 6   && eta>=1.85 && eta<1.9 && dz>= 85.3216 && dz<= 94.9824) inDz=true; 
if(from ==3 && to == 6   && eta>=1.9 && eta<1.95 && dz>= 89.0257 && dz<= 97.5482) inDz=true; 
if(from ==3 && to == 7   && eta>=1.55 && eta<1.6 && dz>= 76.6054 && dz<= 82.8625) inDz=true; 
if(from ==3 && to == 7   && eta>=1.6 && eta<1.65 && dz>= 76.9377 && dz<= 82.5999) inDz=true; 
if(from ==3 && to == 7   && eta>=1.65 && eta<1.7 && dz>= 78.3668 && dz<= 87.3553) inDz=true; 
if(from ==3 && to == 7   && eta>=1.7 && eta<1.75 && dz>= 82.3682 && dz<= 92.3572) inDz=true; 
if(from ==3 && to == 7   && eta>=1.75 && eta<1.8 && dz>= 87.2754 && dz<= 97.0631) inDz=true; 
if(from ==3 && to == 7   && eta>=1.8 && eta<1.85 && dz>= 91.8986 && dz<= 102.944) inDz=true; 
if(from ==3 && to == 7   && eta>=1.85 && eta<1.9 && dz>= 97.8089 && dz<= 104.495) inDz=true; 
if(from ==3 && to == 8   && eta>=1.2 && eta<1.25 && dz>= 68.1656 && dz<= 76.3526) inDz=true; 
if(from ==3 && to == 8   && eta>=1.25 && eta<1.3 && dz>= 72.0144 && dz<= 80.6488) inDz=true; 
if(from ==3 && to == 8   && eta>=1.3 && eta<1.35 && dz>= 76.1825 && dz<= 86.001) inDz=true; 
if(from ==3 && to == 8   && eta>=1.35 && eta<1.4 && dz>= 81.0442 && dz<= 90.7588) inDz=true; 
if(from ==3 && to == 8   && eta>=1.4 && eta<1.45 && dz>= 85.746 && dz<= 95.8694) inDz=true; 
if(from ==3 && to == 8   && eta>=1.45 && eta<1.5 && dz>= 90.9326 && dz<= 100.939) inDz=true; 
if(from ==3 && to == 8   && eta>=1.5 && eta<1.55 && dz>= 95.2243 && dz<= 108.136) inDz=true; 
if(from ==3 && to == 8   && eta>=1.55 && eta<1.6 && dz>= 100.295 && dz<= 119.4) inDz=true; 
if(from ==3 && to == 8   && eta>=1.6 && eta<1.65 && dz>= 108.094 && dz<= 119.918) inDz=true; 
if(from ==3 && to == 8   && eta>=1.65 && eta<1.7 && dz>= 112.988 && dz<= 125.35) inDz=true; 
if(from ==3 && to == 8   && eta>=1.7 && eta<1.75 && dz>= 119.361 && dz<= 127.669) inDz=true; 
if(from ==3 && to == 9   && eta>=1.15 && eta<1.2 && dz>= 70.3416 && dz<= 76.9815) inDz=true; 
if(from ==3 && to == 9   && eta>=1.2 && eta<1.25 && dz>= 73.3643 && dz<= 82.0875) inDz=true; 
if(from ==3 && to == 9   && eta>=1.25 && eta<1.3 && dz>= 77.3735 && dz<= 86.812) inDz=true; 
if(from ==3 && to == 9   && eta>=1.3 && eta<1.35 && dz>= 81.8593 && dz<= 92.5471) inDz=true; 
if(from ==3 && to == 9   && eta>=1.35 && eta<1.4 && dz>= 87.156 && dz<= 97.51) inDz=true; 
if(from ==3 && to == 9   && eta>=1.4 && eta<1.45 && dz>= 92.2342 && dz<= 103.057) inDz=true; 
if(from ==3 && to == 9   && eta>=1.45 && eta<1.5 && dz>= 97.5442 && dz<= 108.718) inDz=true; 
if(from ==3 && to == 9   && eta>=1.5 && eta<1.55 && dz>= 102.366 && dz<= 116.248) inDz=true; 
if(from ==3 && to == 9   && eta>=1.55 && eta<1.6 && dz>= 107.805 && dz<= 128.117) inDz=true; 
if(from ==3 && to == 9   && eta>=1.6 && eta<1.65 && dz>= 113.413 && dz<= 130.302) inDz=true; 
if(from ==3 && to == 9   && eta>=1.65 && eta<1.7 && dz>= 121.581 && dz<= 132.95) inDz=true; 
	
}

