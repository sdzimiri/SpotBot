#ifndef DLGHELP_H
#define DLGHELP_H

#include <QDialog>

namespace Ui {
class dlgHelp;
}

class dlgHelp : public QDialog
{
    Q_OBJECT

public:
    explicit dlgHelp(QWidget *parent = 0);
    ~dlgHelp();

private slots:
    void on_bttn_Close_clicked();

private:
    Ui::dlgHelp *ui;
};

#endif // DLGHELP_H
