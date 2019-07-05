#include "dlghelp.h"
#include "ui_dlghelp.h"
#include <QPixmap>
#include <QDir>

dlgHelp::dlgHelp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgHelp)
{
    ui->setupUi(this);

    QDir dir;
    QString imPath = dir.absolutePath();
    QPixmap map("");
}

dlgHelp::~dlgHelp()
{
    delete ui;
}

void dlgHelp::on_bttn_Close_clicked()
{
    this->close();
}
