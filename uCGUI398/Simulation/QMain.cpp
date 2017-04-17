#include <QApplication>

#include "dialogmain.h"

int main(int argc, char* argv[])
{
	QApplication* app = new QApplication(argc, argv);
	

	//MainForm* form = new MainForm();
	//form->show();

	DialogMain* dialog = new DialogMain();
	dialog->show();

    app->exec();
}
