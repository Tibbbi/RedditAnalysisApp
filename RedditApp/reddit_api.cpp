#include "reddit_api.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QProcess>
#include <iostream>

std::string getBearerToken() {
    QProcess process;
    process.start("py", QStringList() << "get_reddit_token.py");
    if (!process.waitForFinished()) {
        std::cerr << "Error: Failed to execute Python script." << std::endl;
        return "";
    }

    QByteArray output = process.readAllStandardOutput();
    QByteArray error = process.readAllStandardError();

    if (!error.isEmpty()) {
        std::cerr << "Script Error: " << error.toStdString() << std::endl;
        return "";
    }

    std::string token = output.toStdString();
    token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());

    if (token.empty()) {
        std::cerr << "Error: Token retrieval failed." << std::endl;
    } else {
        std::cout << "Bearer Token: " << token << std::endl;
    }

    return token;
}

QStringList fetchHotPosts(const QString &subreddit, const QString &sortBy, int limit, const QString &timeFrame) {

    QStringList posts;
    QNetworkAccessManager manager;

    QString url = QString("https://www.reddit.com/r/%1/%2.json?limit=%3&t=%4")
                      .arg(subreddit)
                      .arg(sortBy)
                      .arg(limit)
                      .arg(timeFrame);
    QNetworkRequest request((QUrl(url)));
    request.setHeader(QNetworkRequest::UserAgentHeader, "RedditApp/1.0");

    QEventLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
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
    } else {
        posts.append("Error: Unable to fetch data.");
    }

    reply->deleteLater();
    return posts;
}


QStringList fetchPrivateMessages(const QString &token)
{
    QStringList messages;
    QNetworkAccessManager manager;

    QUrl url("https://oauth.reddit.com/message/inbox");
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());
    request.setHeader(QNetworkRequest::UserAgentHeader, "RedditApp/1.0");

    QEventLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(responseData);

        QJsonArray children = json.object()["data"].toObject()["children"].toArray();
        for (const QJsonValue &child : children) {
            QString author = child.toObject()["data"].toObject()["author"].toString();
            QString subject = child.toObject()["data"].toObject()["subject"].toString();
            QString body = child.toObject()["data"].toObject()["body"].toString();

            messages.append(QString("From: %1\nSubject: %2\n\n%3")
                                .arg(author)
                                .arg(subject)
                                .arg(body));
        }
    } else {
        messages.append("Error: Unable to fetch messages.");
    }

    reply->deleteLater();
    return messages;
}
