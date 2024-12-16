#include "reddit_api.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>

QStringList fetchHotPosts(const QString &subreddit)
{
    QStringList posts;
    QNetworkAccessManager manager;

    QString url = QString("https://www.reddit.com/r/%1/hot.json").arg(subreddit);
    QNetworkRequest request((QUrl(url)));
    request.setHeader(QNetworkRequest::UserAgentHeader, "RedditApp/1.0");

    QEventLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, [&loop]() { loop.quit(); });
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(responseData);

        QJsonArray children = json.object()["data"].toObject()["children"].toArray();
        for (const QJsonValue &child : children) {
            QString title = child.toObject()["data"].toObject()["title"].toString();
            QString author = child.toObject()["data"].toObject()["author"].toString();
            int score = child.toObject()["data"].toObject()["score"].toInt();
            int comments = child.toObject()["data"].toObject()["num_comments"].toInt();

            posts.append(QString("%1||%2||%3||%4")
                             .arg(title)
                             .arg(author)
                             .arg(score)
                             .arg(comments));
        }
    } else {
        posts.append("Error: Unable to fetch data.");
    }

    reply->deleteLater();
    return posts;
}
