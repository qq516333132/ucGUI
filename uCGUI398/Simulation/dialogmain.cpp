#include "dialogmain.h"
#include "ui_dialogmain.h"

#include <QThread>
#include "GUI.h"
#include "LCDConf.h"

#include <QPushButton>
#include <QWidget>
#include <QTimer>
#include <QImage>
#include <QDebug>
#include <QMouseEvent>

unsigned int LCD_Buffer[LCD_YSIZE][LCD_XSIZE];





#ifdef __cplusplus
extern "C" {
#endif

	void LCDSIM_Init(void)
	{
	}

	void LCDSIM_SetPixelIndex(int x, int y, int Index, int LayerIndex)
	{
#if 0
		int R = (Index & 0xF800) >> 11;
		int G = (Index & 0x07E0) >> 5;
		int B = Index & 0x001F;

		LCD_Buffer[y][x] = R << 3 << 16 | G << 2 << 8 | B << 3;
#endif
		//return;
#if (LCD_BITSPERPIXEL == 8)
		unsigned char R = 1.164 * (Index - 16);
		LCD_Buffer[y][x] = R << 16 | R << 8 | R;
#elif (LCD_BITSPERPIXEL == 16) && (LCD_FIXEDPALETTE == 565)
		LCD_Buffer[y][x] = ((Index & 0xF800) << 3 >> 11 << 16) | ((Index & 0x07E0) << 2 >> 5 << 8) | (Index & 0x1F) << 3;
#elif (LCD_BITSPERPIXEL == 1)
		if (Index == 0)
		{
			LCD_Buffer[y][x] = 0x00000000;
		}
		else
		{
			LCD_Buffer[y][x] = 0x00FFFFFF;
		}
#else
		LCD_Buffer[y][x] = Index;
#endif
	}


	void LCDSIM_FillRect(int x0, int y0, int x1, int y1, int Index, int LayerIndex)
	{

	}


	int LCDSIM_GetPixelIndex(int x, int y, int LayerIndex)
	{
		return LCD_Buffer[y][x];
	}

	void LCDSIM_SetLUTEntry(U8 Pos, LCD_COLOR color, int LayerIndex)
	{

	}

#ifdef __cplusplus
}
#endif


GUI_PID_STATE Touch_Status;
void Touch_Pressed(int x, int y)  //触摸按下  x,y 为按下是的鼠标在窗体中的坐标
{
	Touch_Status.Pressed = 1;
	Touch_Status.x = x;
	Touch_Status.y = y;
	GUI_TOUCH_StoreStateEx(&Touch_Status);
}

void Touch_Release(void)  //触摸释放
{
	Touch_Status.Pressed = 0;
	Touch_Status.x = -1;
	Touch_Status.y = -1;
	GUI_TOUCH_StoreStateEx(&Touch_Status);
}


class MainForm : public QWidget
{
private:


public:

	MainForm()
	{

	}
};

#ifdef __cplusplus
extern "C" {
	extern void MainTask(void);
}
#endif


class GUIThread : public QThread
{
public:
	void run()
	{
		MainTask();  //GUI Demo
	}
};


void DialogMain::labelPressEvent(QMouseEvent *ev)
{
    Touch_Status.Pressed = 1;
    Touch_Status.x = ev->x();
    Touch_Status.y = ev->y();
    GUI_TOUCH_StoreStateEx(&Touch_Status);
}

void DialogMain::timerAction()
{
	//qDebug() << "timer action";
	//QByteArray imageByteArray = QByteArray((const char*)LCD_Buffer, LCD_XSIZE*LCD_YSIZE*3);
	//uchar*  transData = (unsigned char*)imageByteArray.data();

	QImage image(LCD_XSIZE, LCD_YSIZE, QImage::Format_RGB32);
	for (int i = 0; i < LCD_XSIZE; i++)
	{
		for (int j = 0; j < LCD_YSIZE; j++)
		{
			unsigned int value = (uint)(LCD_Buffer[j][i]) ;
			image.setPixel(i, j, value);
		}
	}

	ui->label->setPixmap(QPixmap::fromImage(image));

	this->update();
}


DialogMain::DialogMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMain)
{
    ui->setupUi(this);

	GUIThread* guithread = new GUIThread();
	guithread->start();

	QImage myImage;
	myImage.load("C:\\Android\\Android\\sdk\\docs\\images\\home\\androidn-hero-n5.png");

	ui->label->setPixmap(QPixmap::fromImage(myImage));

	
	//image->
	timer = new QTimer(this);
	timer->setInterval(30);
	connect(timer, &QTimer::timeout, this, &DialogMain::timerAction);

	timer->start();

    connect(ui->label, SIGNAL(mousePressEvent(QMouseEvent*)), this, SLOT(labelPressEvent()));
}

DialogMain::~DialogMain()
{
    delete ui;
}
