#include "browserCache.h"

const QString BrowserCache::metadataFileName = "cache_metadata.json";

BrowserCache::BrowserCache(const QString& path, int maxMemorySizeMB, int maxDiskSizeMB,
                           QObject* parent)
    : QObject(parent)
    , cachePath(path)
    , maxDiskSize(maxDiskSizeMB * 1024 * 1024)
{
    // Initialize memory cache with max size in bytes
    memoryCache.setMaxCost(maxMemorySizeMB * 1024 * 1024);

    ensureCacheDirectory();
    loadMetadata();
}

BrowserCache::~BrowserCache()
{
    saveMetadata();
}

bool BrowserCache::store(const QUrl& url, const QByteArray& data,
                         const QString& contentType, const QDateTime& expiry,
                         const QString& etag, const QString& lastModified)
{
    QString cacheKey = generateCacheKey(url);

    // Create cache entry
    auto* entry = new CacheEntry{
        data,
        contentType,
        expiry.isValid() ? expiry : QDateTime::currentDateTime().addDays(7), // 1 week default
        etag,
        lastModified
    };

    // Store in memory cache
    if (data.size() <= memoryCache.maxCost()) {
        memoryCache.insert(cacheKey, entry, data.size());
    }

    // Store on disk
    QString filePath = generateFilePath(cacheKey);
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly)) {
        // Write cache entry data
        QDataStream stream(&file);
        stream << entry->data;
        stream << entry->contentType;
        stream << entry->expiryTime;
        stream << entry->etag;
        stream << entry->lastModified;

        file.close();

        // Update metadata
        urlToFileMap[url.toString()] = filePath;
        saveMetadata();

        // Check disk cache size and cleanup if needed
        if (calculateDiskCacheSize() > maxDiskSize) {
            cleanupOldEntries();
        }

        return true;
    }

    return false;
}

CacheEntry* BrowserCache::retrieve(const QUrl& url)
{
    QString cacheKey = generateCacheKey(url);

    // Try memory cache first
    if (auto* entry = memoryCache.object(cacheKey)) {
        if (!isExpired(url)) {
            return entry;
        }
    }

    // Try disk cache
    QString filePath = urlToFileMap.value(url.toString());
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream stream(&file);
            auto* entry = new CacheEntry;

            stream >> entry->data;
            stream >> entry->contentType;
            stream >> entry->expiryTime;
            stream >> entry->etag;
            stream >> entry->lastModified;

            file.close();

            // Store in memory cache for future use
            if (entry->data.size() <= memoryCache.maxCost()) {
                memoryCache.insert(cacheKey, entry, entry->data.size());
            }

            if (!isExpired(url)) {
                return entry;
            }

            delete entry;
        }
    }

    return nullptr;
}

bool BrowserCache::remove(const QUrl& url)
{
    QString cacheKey = generateCacheKey(url);

    // Remove from memory cache
    memoryCache.remove(cacheKey);

    // Remove from disk cache
    QString filePath = urlToFileMap.value(url.toString());
    if (!filePath.isEmpty()) {
        QFile::remove(filePath);
        urlToFileMap.remove(url.toString());
        saveMetadata();
        return true;
    }

    return false;
}

void BrowserCache::clear()
{
    // Clear memory cache
    memoryCache.clear();

    // Clear disk cache
    QDir cacheDir(cachePath);
    cacheDir.removeRecursively();
    ensureCacheDirectory();

    urlToFileMap.clear();
    saveMetadata();
}

bool BrowserCache::isValid(const QUrl& url) const
{
    return !isExpired(url);
}

bool BrowserCache::isExpired(const QUrl& url) const
{
    QString cacheKey = generateCacheKey(url);

    // Check memory cache
    if (auto* entry = memoryCache.object(cacheKey)) {
        return entry->expiryTime < QDateTime::currentDateTime();
    }

    // Check disk cache
    QString filePath = urlToFileMap.value(url.toString());
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream stream(&file);
            QByteArray data;
            QString contentType;
            QDateTime expiryTime;
            QString etag;
            QString lastModified;

            stream >> data >> contentType >> expiryTime >> etag >> lastModified;
            file.close();

            return expiryTime < QDateTime::currentDateTime();
        }
    }

    return true;
}

QString BrowserCache::generateCacheKey(const QUrl& url) const
{
    return QCryptographicHash::hash(url.toString().toUtf8(),
                                    QCryptographicHash::Sha256).toHex();
}

QString BrowserCache::generateFilePath(const QString& cacheKey) const
{
    return cachePath + "/" + cacheKey;
}

void BrowserCache::ensureCacheDirectory()
{
    QDir dir(cachePath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

bool BrowserCache::saveMetadata()
{
    QFile file(cachePath + "/" + metadataFileName);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject metadata;
        for (auto it = urlToFileMap.begin(); it != urlToFileMap.end(); ++it) {
            metadata[it.key()] = it.value();
        }

        QJsonDocument doc(metadata);
        file.write(doc.toJson());
        file.close();
        return true;
    }
    return false;
}

bool BrowserCache::loadMetadata()
{
    QFile file(cachePath + "/" + metadataFileName);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject metadata = doc.object();

        urlToFileMap.clear();
        for (auto it = metadata.begin(); it != metadata.end(); ++it) {
            urlToFileMap[it.key()] = it.value().toString();
        }

        file.close();
        return true;
    }
    return false;
}

void BrowserCache::cleanupOldEntries()
{
    QList<QPair<QString, QDateTime>> entries;

    // Collect all entries with their last modified times
    for (auto it = urlToFileMap.begin(); it != urlToFileMap.end(); ++it) {
        QFileInfo fileInfo(it.value());
        if (fileInfo.exists()) {
            entries.append({it.key(), fileInfo.lastModified()});
        }
    }

    // Sort by last modified time (oldest first)
    std::sort(entries.begin(), entries.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    // Remove oldest entries until we're under the size limit
    while (calculateDiskCacheSize() > maxDiskSize && !entries.isEmpty()) {
        QString url = entries.first().first;
        remove(QUrl(url));
        entries.removeFirst();
    }
}

qint64 BrowserCache::calculateDiskCacheSize() const
{
    qint64 totalSize = 0;
    QDir dir(cachePath);

    for (const QFileInfo& file : dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)) {
        totalSize += file.size();
    }

    return totalSize;
}
