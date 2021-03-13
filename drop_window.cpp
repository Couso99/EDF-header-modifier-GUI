#include "drop_window.h"
#include "ui_drop_window.h"
/* Author: Pablo Couso */
//#include <QDir>
DropWindow::DropWindow(QWidget *parent)
:QMainWindow(parent), ui(new Ui::DropWindow)
{
  ui->setupUi(this);
  setAcceptDrops(true);
  ui->createFilesButton->setEnabled(0);

  //ui->listWidget->addItem(QDir().absolutePath());
}

DropWindow::~DropWindow()
{
  delete ui;
}

void DropWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void DropWindow::dragMoveEvent(QDragMoveEvent *e)
{
  e->acceptProposedAction();
}

void DropWindow::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();

        if (fileName.endsWith(".edf"))
          addFile(fileName);

        //qDebug() << "Dropped file:" << fileName;
    }
}

void DropWindow::addFile(QString fname)
{
  QList<QListWidgetItem *> items;
  items = ui->listWidget->findItems(fname, Qt::MatchExactly);
  if (!items.empty())
  {
    ui->listWidget->setCurrentItem(items.at(0));
    return;
  }

  filenames.push_back(fname);
  ui->listWidget->addItem(fname);
}

void DropWindow::on_createFilesButton_clicked()
{
  int i;
  /*qDebug() << "Filenames stored are:";
  for (i=0;i<(int)filenames.size();i++)
  {
    qDebug() << '\t' << filenames[i];
  }*/

  int fileNumber;
  fileNumber = (int) filenames.size();
  char **fnames = (char **) malloc(fileNumber*sizeof(char *));
  for (i=0;i<fileNumber;i++)
  {
    fnames[i] = (char *) malloc(256*sizeof(char));
    strcpy(fnames[i],filenames[i].toStdString().c_str());
  }

  char *folder = (char *) malloc(256*sizeof(char));
  strcpy(folder, folderName.toStdString().c_str());

  //if (!QDir("RESULTS").exists())
  //  QDir().mkdir("RESULTS");

  int retval;
  retval = create_new_files(fileNumber, folder, fnames);
  qDebug() << "El resultado es: "<<retval << '\n';

  filenames.clear();
  ui->listWidget->clear();

}

void DropWindow::on_chooseFolderButton_clicked()
{
  QFileDialog *dialog;
  dialog = new QFileDialog(this);
  dialog->setFileMode(QFileDialog::Directory);
  
  if (dialog->exec())
  {
    QStringList l;
    l = dialog->selectedFiles();
    folderName = l.at(0);
    ui->createFilesButton->setEnabled(1);
  }
}

void DropWindow::on_selectFilesButton_clicked()
{
  QFileDialog *dialog;
  dialog = new QFileDialog(this);
  dialog->setNameFilter(tr("EDF (*.edf)"));

  if (dialog->exec())
  {
    QStringList l;
    l = dialog->selectedFiles();
    for (int i=0;i<l.size();i++)
      addFile(l.at(i));
  }
}

void DropWindow::on_deleteFileButton_clicked()
{
  int index;
  if (ui->listWidget->count())
  {
    index = ui->listWidget->currentRow();
    filenames.erase(filenames.begin()+index);
    QListWidgetItem *it = ui->listWidget->takeItem(index);
    delete it;
  }
}
