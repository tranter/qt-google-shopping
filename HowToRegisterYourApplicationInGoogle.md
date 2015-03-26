# Introduction #

The qt-google-shopping example uses Google API for Shopping. To use API you need to register your own application on Google. Do not worry: the procedure is very simple.


# Details #

You need to login to Google, so first you need to create simple Google account. Then you can visit the page

**https://code.google.com/apis/console**

there you can create your application. You need to check access to **Search API for Shoping** in tab "Services" (set Status On as shown in the next figure).

![http://dl.dropbox.com/u/72326178/SetServiceShoping.jpg](http://dl.dropbox.com/u/72326178/SetServiceShoping.jpg)

Then you need create Simple API Access key, not Client ID because qt-google-shopping application not use private user data.

Then you need change source code:

1) you need to open the file **shopping\_data\_manager.cpp**.

2) find the line m\_key = "YOUR\_SIMPLE\_API\_ACCESS\_KEY\_HERE";

3) replace **YOUR\_SIMPLE\_API\_ACCESS\_KEY\_HERE** value with **your Simple API Access key**.


```


ShoppingDataManager::ShoppingDataManager()
{
    m_pNetworkAccessManager = new QNetworkAccessManager(this);
    m_key = "YOUR_SIMPLE_API_ACCESS_KEY_HERE";
    m_startIndex = 1;
    m_maxResults = 25;
    m_facetsEnabled = true;

    connect(m_pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}


```

After that you can compile and run **qt-google-shopping**.