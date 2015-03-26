# Introduction #

Information about use Google API, testing platforms, Qt versions.

# Details #

Project **qt-google-shopping** uses Google API for Shopping.

### Google API ###

Project **qt-google-shopping** uses Google API for Shopping.

How it works:
Work with Google-API perfomed by send https-request with using QNetworkAccessManager (see files _shopping\_data\_manager.h_ and _shopping\_data\_manager.cpp_)

API features used in this project:
|search products|
|:--------------|
|show details of product|

File **HowToRegisterYourAppIicationInGoogle** describes how register your own application on Google.

### Tested platforms ###
Project was tested on:
| **OS** | **Qt version** |
|:-------|:---------------|
|Linux 64bit|Qt 4.7.4, 4.8.1|
|Windows 7 64bit|Qt 4.8.0|
|Windows XP SP3 32bit|Qt 4.8.0|


# Various comments #

This application can be compiled on Linux, MacOS, Windows. For Windows you can use either MingW compiler or MSVC compiler.