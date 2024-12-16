#ifndef REDDIT_API_H
#define REDDIT_API_H

#include <QString>
#include <QList>
#include <QStringList>

QStringList fetchHotPosts(const QString &subreddit, const QString &timeFilter, bool sortByScore, bool sortByComments);

#endif // REDDIT_API_H
