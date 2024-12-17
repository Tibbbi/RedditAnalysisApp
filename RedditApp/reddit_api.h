// reddit_api.h
#ifndef REDDIT_API_H
#define REDDIT_API_H

#include <QString>
#include <QStringList>

QStringList fetchHotPosts(const QString &subreddit, const QString &sortBy = "hot", int limit = 10, const QString &timeFrame = "all");
QStringList fetchPrivateMessages(const QString &token);
std::string getBearerToken();

#endif // REDDIT_API_H
