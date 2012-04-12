#include <QDebug>
#include <QApplication>
#include <QSettings>
#include "oauth2.h"
#include "logindialog.h"

OAuth2::OAuth2(QObject *parent) :
    QObject(parent)
{
    m_strEndPoint = "https://accounts.google.com/o/oauth2/auth";
    m_strScope = "https://www.googleapis.com/auth/shoppingapi"; //Access to Shopping API Service service
//    m_strClientID = "476807742904-1e2qesck2a151t6envel2756heus7ddt.apps.googleusercontent.com";
//    m_strRedirectURI = "http://www.ics.com/oauth2callback";
    m_strClientID = "27044754895.apps.googleusercontent.com";
    m_strRedirectURI = "urn:ietf:wg:oauth:2.0:oob";
    m_strResponseType = "token";
}
//OAuth2::OAuth2() :
//{
//}
QString OAuth2::loginUrl()
{
    QString str = QString("%1?client_id=%2&redirect_uri=%3&response_type=%4&scope=%5").arg(m_strEndPoint).arg(m_strClientID).
            arg(m_strRedirectURI).arg(m_strResponseType).arg(m_strScope);
    qDebug() << "Login URL" << str;
    return str;
}

QString OAuth2::accessToken()
{
    return m_strAccessToken;
}

bool OAuth2::isAuthorized()
{
    return m_strAccessToken != "";
}

void OAuth2::startLogin(QWidget* parent, bool bForce)
{
    qDebug() << "OAuth2::startLogin";
    QSettings settings("ICS", "Google API Shopping Client");
    QString str = settings.value("access_token", "").toString();

    qDebug() << "OAuth2::startLogin, token from Settings" << str;

    if(str.isEmpty() || bForce)
    {
        LoginDialog dlg(parent);
        dlg.setLoginUrl(loginUrl());
        int res = dlg.exec();
        if(res == QDialog::Accepted)
        {
            m_strAccessToken = dlg.accessToken();
            settings.setValue("access_token", m_strAccessToken);
            emit loginDone();
        }
    }
    else
    {
        m_strAccessToken = str;
        emit loginDone();
    }
}

