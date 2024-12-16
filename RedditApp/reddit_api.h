#ifndef REDDIT_API_H
#define REDDIT_API_H

#include <QString>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QStringList>

QStringList fetchHotPosts(const QString &subreddit);

#endif // REDDIT_API_H
