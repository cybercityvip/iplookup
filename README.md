# iplookup
Simple IP lookup tool written in C

# Installation 
  
Install dependencies
- debian
   <hr>

  ```apt install gcc git```
    <hr>
- arch linux
  
  ```pacman -S gcc git```
    <hr>
- openbsd
  
The GNU C compiler executable is called egcc when it's installed from the gcc package. There should also be a gcc executable in /usr/bin (the gcc of the base system, at least on amd64).

  ```pkg_add -v gcc git```
  <hr>

Install iplookup
  
  ```git clone https://github.com/cybercityvip/iplookup.git```
  
  ```cd iplookup```
    
  ```gcc -o iplookup ip.c $(pkg-config --libs --cflags libcurl json-c)```

  <hr>
syntax

```iplookup 127.0.0.1```
  <hr>

api used 

https://ipapi.co
  <hr>

<img src="https://cybercity.vip/archive/ip.png">

