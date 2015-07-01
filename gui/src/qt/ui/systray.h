#ifndef SYSTRAY_H
#define SYSTRAY_H

#include "model/scanmodel.h"

#include <QObject>
#include <QThread>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>

class WatchThread;

class Systray : public QObject {
    Q_OBJECT

public:
  Systray();
  ~Systray() {}

  void addRecentScan(ScanModel *model);

public slots:
  void scan(const QString &path);
  void scan();
  void about();

private:
  void createActions();
  void createTrayIcon();

  QIcon *getIcon();

  QAction *scanAction;
  QAction *aboutAction;
  QAction *quitAction;
  QSystemTrayIcon *trayIcon;
  QMenu *recentScanMenu;
  WatchThread *_watchThread;
};

class RecentScanAction: public QAction {
    Q_OBJECT

 public:
  RecentScanAction(QObject *parent, ScanModel *model);
  RecentScanAction(const QString &text, QObject *parent, ScanModel *model);
  RecentScanAction(const QIcon &icon, const QString &text, QObject *parent, ScanModel *model);

 public slots:
  void showScan();

 private:
  ScanModel *_model;
};

class WatchThread: public QThread {
  Q_OBJECT

public:
  WatchThread(const QString &path/* , Systray *systray */) : _path(path)/* , _systray(systray) */ {}

signals:
  void watched(const QString &path);

protected:
  void run();

private:
  QString _path;
  /* Systray *_systray; */
};

#endif