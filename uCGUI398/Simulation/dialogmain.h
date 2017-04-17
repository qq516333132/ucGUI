#ifndef DIALOGMAIN_H
#define DIALOGMAIN_H

#include <QDialog>

namespace Ui {
class DialogMain;
}

class DialogMain : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMain(QWidget *parent = 0);
    ~DialogMain();

    void timerAction();
    void labelPressEvent(QMouseEvent *ev);

private:
    Ui::DialogMain *ui;

	QImage* image;
	QTimer* timer;
};

#endif // DIALOGMAIN_H
