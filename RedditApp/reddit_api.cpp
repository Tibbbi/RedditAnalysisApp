#include "reddit_api.h"


QStringList fetchHotPosts(const QString &subreddit)
{
    QStringList posts;
    QNetworkAccessManager manager;

    // Construiește URL-ul
    QString url = QString("https://www.reddit.com/r/%1/hot.json").arg(subreddit);
    qDebug() << "Generated URL:" << url; // Debug URL

    QNetworkRequest request((QUrl(url)));
    request.setHeader(QNetworkRequest::UserAgentHeader, "RedditApp/1.0");

    // Inițializează bucla de evenimente pentru cererea HTTP
    QEventLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, [&loop]() { loop.quit(); });
    loop.exec();

    // Procesarea răspunsului
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(responseData);

        QJsonArray children = json.object()["data"].toObject()["children"].toArray();
        for (const QJsonValue &child : children) {
            QJsonObject data = child.toObject()["data"].toObject();
            QString title = data["title"].toString();
            QString author = data["author"].toString();
            int score = data["score"].toInt();
            int comments = data["num_comments"].toInt();
            QString imageUrl = data["url_overridden_by_dest"].toString();
            QString link = "https://reddit.com" + data["permalink"].toString();

            posts.append(QString("%1||%2||%3||%4||%5||%6")
                             .arg(title)
                             .arg(author)
                             .arg(score)
                             .arg(comments)
                             .arg(imageUrl)
                             .arg(link));
        }
    } else {
        qDebug() << "Network Error:" << reply->errorString();
        posts.append("Error: Unable to fetch data.");
    }

    reply->deleteLater();
    return posts;
}

