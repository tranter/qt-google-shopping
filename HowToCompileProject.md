# Introduction #

Short guide on compilation


# Details #


This project depends on QJson and Qwt libraries.

You need to get all three projects by:

  * **_git clone https://code.google.com/p/qt-google-shopping/_**
  * **_git clone git://gitorious.org/qjson/qjson.git_**
  * **_svn co https://qwt.svn.sourceforge.net/svnroot/qwt/branches/qwt-6.0_**

All three folders: **qt-google-shopping**, **qjson** and **qwt-6.0** must be in the same directory.

Then you need to compile **qjson** and **qwt** libraries first. We count that **qjson** library is in the **qjson/build/lib** directory and **qwt** library is in the **qwt-6.0/lib** directory.

Then the **qt-google-shopping** project will be compiled OK. To run it you need to adjust **LD\_LIBRARY\_PATH** on Linux (or **PATH** on Windows) so that variable points to the **qjson/build/lib** directory and **qwt-6.0/lib** directory.