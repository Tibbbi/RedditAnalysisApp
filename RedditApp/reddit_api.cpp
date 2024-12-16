#include "reddit_api.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>

QStringList fetchHotPosts(const QString &subreddit, const QString &timeFilter, bool sortByScore, bool sortByComments)
{
    QStringList posts;
    QNetworkAccessManager manager;

    // Construirea URL-ului pe baza filtrului de timp
    QString url = QString("https://www.reddit.com/r/%1/top.json?t=%2").arg(subreddit, timeFilter);
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
            QString link = "https://reddit.com" + child.toObject()["data"].toObject()["permalink"].toString();

            posts.append(QString("%1||%2||%3||%4||%5")
                             .arg(title)
                             .arg(author)
                             .arg(score)
                             .arg(comments)
                             .arg(link));
        }

        // Sortare postări pe baza criteriilor selectate
        std::sort(posts.begin(), posts.end(), [&](const QString &a, const QString &b) {
            QStringList aDetails = a.split("||");
            QStringList bDetails = b.split("||");
            int aScore = aDetails[2].toInt();
            int bScore = bDetails[2].toInt();
            int aComments = aDetails[3].toInt();
            int bComments = bDetails[3].toInt();

            if (sortByScore && sortByComments) {
                return (aScore == bScore) ? (aComments > bComments) : (aScore > bScore);
            } else if (sortByScore) {
                return aScore > bScore;
            } else if (sortByComments) {
                return aComments > bComments;
            }
            return false;
        });
    } else {
        posts.append("Error: Unable to fetch data.");
    }

    reply->deleteLater();
    return posts;
}
