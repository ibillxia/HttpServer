# HttpServer  
 A simple realization of Http protocol  

## Usage  
1) Get the source    

```
git clone https://github.com/ibillxia/HttpServer.git
cd HttpServer
```

2) Edit Config
Set the `www` data storage directory:

```
vim common.h
```

Set the variable `server_dir` to `/home/YourName/www` or any other directory you prefer. 

3) Start The Server
First compile the source code, and then run the executable file:

```
make
./xhttp
```

The server is started if it shows the following message, 

```
-HTTP Server [xhttp] started...
```

4) Test a Webpage  
Copy the files in `www` to your `server_dir` setted in the 2nd step,

```
cp ./www/* %server_dir%
```

Open your browser and enter the url `localhost:8080/index.html`. 
Then it will shows the webpage stored on your localhost.

## About HTTP protocol  

This program is a simple HTTP server. It runs only on Linux/Unix plantforms.  
For more about HTTP protocol, please refer to its wiki:   
[Hypertext_Transfer_Protocol](http://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol)

## Lisense  

The MIT License (MIT)  
Copyright (C) 2011-2015 Bill Xia (ibillxia@gmail.com)   
All rights reserved.  


