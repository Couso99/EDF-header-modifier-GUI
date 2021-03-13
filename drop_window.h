#ifndef DRP_WND_H
#define DRP_WND_H
/* Author: Pablo Couso */

#include <QMainWindow>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>
#include <QDir>

#include "create_anon_file.h"

namespace Ui {
  class DropWindow;
}

class DropWindow : public QMainWindow
{
  Q_OBJECT

private:
  Ui::DropWindow *ui;
  std::vector<QString> filenames;
  QString folderName;
  void addFile(QString);

public:
  DropWindow(QWidget *parent=nullptr);
  ~DropWindow();
  void dragEnterEvent(QDragEnterEvent *);
  void dragMoveEvent(QDragMoveEvent *e);
  void dropEvent(QDropEvent *);

public slots:
  void on_createFilesButton_clicked();
  void on_chooseFolderButton_clicked();
  void on_selectFilesButton_clicked();
  void on_deleteFileButton_clicked();
};


#endif // DRP_WND_H
