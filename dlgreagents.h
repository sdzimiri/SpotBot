#ifndef DLGREAGENTS_H
#define DLGREAGENTS_H

#include <QDialog>

namespace Ui {
class dlgReagents;
}

class dlgReagents : public QDialog
{
    Q_OBJECT

public:
    explicit dlgReagents(QWidget *parent = 0);
    ~dlgReagents();
    void setReagents(QString list[20]);

signals:
    void txUpdatedReagents(QStringList);

private slots:
    void on_bttn_Accept_clicked();
    void on_bttn_Cancel_clicked();

private:
    Ui::dlgReagents *ui;
    QString reagList[20];
};

#endif // DLGREAGENTS_H
