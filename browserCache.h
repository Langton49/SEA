#ifndef BROWSERCACHE_H
#define BROWSERCACHE_H

#include <QString>
#include <QByteArray>
#include <QCache>
#include <QDir>
#include <QCryptographicHash>
#include <QDateTime>
#include <QUrl>
#include <QMap>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

struct CacheEntry {
    QByteArray data;
    QString contentType;
    QDateTime expiryTime;
    QString etag;
    QString lastModified;
};

class BrowserCache : public QObject {
    Q_OBJECT

public:
    explicit BrowserCache(const QString& cachePath, int maxMemorySizeMB = 50,
                          int maxDiskSizeMB = 500, QObject* parent = nullptr);
    ~BrowserCache();

    // Cache operations
    bool store(const QUrl& url, const QByteArray& data,
               const QString& contentType, const QDateTime& expiry = QDateTime(),
               const QString& etag = QString(),
               const QString& lastModified = QString());

    CacheEntry* retrieve(const QUrl& url);
    bool remove(const QUrl& url);
    void clear();

    // Cache validation
    bool isValid(const QUrl& url) const;
    bool isExpired(const QUrl& url) const;
    QString getETag(const QUrl& url) const;
    QString getLastModified(const QUrl& url) const;

private:
    // Memory cache
    QCache<QString, CacheEntry> memoryCache;

    // Disk cache
    QString cachePath;
    qint64 maxDiskSize;
    QMap<QString, QString> urlToFileMap;

    // Helper functions
    QString generateCacheKey(const QUrl& url) const;
    QString generateFilePath(const QString& cacheKey) const;
    bool saveMetadata();
    bool loadMetadata();
    void ensureCacheDirectory();
    void cleanupOldEntries();
    qint64 calculateDiskCacheSize() const;

    // Metadata file handling
    static const QString metadataFileName;
};

#endif // BROWSERCACHE_H
