#include "ControlUI.h"
#include<QMessageBox>
ControlUI::ControlUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::WindowCloseButtonHint | Qt::FramelessWindowHint);
	ui.CloseBtn->setToolTipName(QString::fromLocal8Bit("关闭"));
	ui.MinBtn->setToolTipName(QString::fromLocal8Bit("最小化"));
	connect(ui.MinBtn, SIGNAL(clicked()), this, SLOT(slot_minWindow()));
	connect(ui.CloseBtn, SIGNAL(clicked()), this, SLOT(close()));
}

ControlUI::~ControlUI()
{
}

void ControlUI::closeEvent(QCloseEvent *event)
{
	QWidget::close();
	*isActive = false;
	delete this;
}

void ControlUI::slot_minWindow()
{
	this->showMinimized();
}

void ControlUI::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		m_Drag = true;
		m_DragPosition = event->globalPos() - this->pos();
		event->accept();
	}
}

void ControlUI::mouseMoveEvent(QMouseEvent *event)
{
	if (m_Drag && (event->buttons() && Qt::LeftButton)) {
		move(event->globalPos() - m_DragPosition);
		event->accept();
	}
}

void ControlUI::mouseReleaseEvent(QMouseEvent *event)
{
	m_Drag = false;
}

void ControlUI::on_RIDBtn_clicked()
{
	cmmIndex = 2301;
	//读取ID
	QString Comm = "ID\r\n";
	QByteArray ba = Comm.toLatin1();
	LPCSTR ch = ba.data();
	int len = Comm.length();
	::send(Socket, ch, len, 0);
}

void ControlUI::on_WIDBtn_clicked()
{
	cmmIndex = 2302;
	//设置ID
	QString Comm = "ID,"+ui.DevicelineEdit->text().trimmed()+"\r\n";
	QByteArray ba = Comm.toLatin1();
	LPCSTR ch = ba.data();
	int len = Comm.length();
	::send(Socket, ch, len, 0);
}

void ControlUI::on_RTimeBtn_clicked()
{
	cmmIndex = 2303;
	//读取时钟
	QString Comm = "DATETIME\r\n";
	QByteArray ba = Comm.toLatin1();
	LPCSTR ch = ba.data();
	int len = Comm.length();
	::send(Socket, ch, len, 0);
}

void ControlUI::on_WTimeBtn_clicked()
{
	cmmIndex = 2304;
	//设置时钟
	QString Time;
	Time = ui.RdateTimeEdit->dateTime().toString("yyyy-MM-dd,HH:mm:ss");
	QString Comm = "DATETIME," + Time + "\r\n";
	QByteArray ba = Comm.toLatin1();
	LPCSTR ch = ba.data();
	int len = Comm.length();
	::send(Socket, ch, len, 0);
}

void ControlUI::on_RALTBtn_clicked()
{
	cmmIndex = 2305;
	//读取高度
	QString Comm = "QZ\r\n";
	QByteArray ba = Comm.toLatin1();
	LPCSTR ch = ba.data();
	int len = Comm.length();
	::send(Socket, ch, len, 0);
}

void ControlUI::on_WALTBtn_clicked()
{
	cmmIndex = 2306;
	//设置台站
	QString Comm = "QZ," + ui.LineEdit_ALT->text().trimmed() + "\r\n";
	QByteArray ba = Comm.toLatin1();
	LPCSTR ch = ba.data();
	int len = Comm.length();
	::send(Socket, ch, len, 0);
}

void ControlUI::on_RRESETBtn_clicked()
{
	cmmIndex = 2311;
	//重启采集器
	QString Comm = "RESET\r\n";
	QByteArray ba = Comm.toLatin1();
	LPCSTR ch = ba.data();
	int len = Comm.length();
	::send(Socket, ch, len, 0);
}

void ControlUI::on_RUPDATEBtn_clicked()
{
	cmmIndex = 2312;
	//远程升级
	QString Comm = "UPDATE\r\n";
	QByteArray ba = Comm.toLatin1();
	LPCSTR ch = ba.data();
	int len = Comm.length();
	::send(Socket, ch, len, 0);
}

void ControlUI::on_DownBtn_clicked()
{
	cmmIndex = 2313;
	//补抄
	QString Time_B;
	Time_B = ui.BdateTimeEdit->dateTime().toString("yyyy-MM-dd,HH:mm");
	QString Comm = "DMTD," + Time_B + ",1\r\n";
	QByteArray ba = Comm.toLatin1();
	LPCSTR ch = ba.data();
	int len = Comm.length();
	::send(Socket, ch, len, 0);
}

//设备返回值
void ControlUI::setValue(QStringList list)
{
	switch (cmmIndex)
	{
	case 2301://设备号
	{
		if (list.count() < 1)
			break;
		ui.DevicelineEdit->setText(list.at(0));
		break;
	}
	case 2303://设备时间
	{
		if (list.count() < 2)
			break;
		QString datetime;
		datetime = list.at(0) + list.at(1);
		QDateTime time = QDateTime::fromString(datetime, "yyyy-MM-ddhh:mm:ss");
		ui.RdateTimeEdit->setDateTime(time);
		break;
	}
	case 2305://海拔
	{
		if (list.count() < 1)
			break;
		ui.LineEdit_ALT->setText(list.at(0));
		break;
	}
	default:
	{
		if (list.count() < 1)
			break;
		QString r = list.at(0);
		if (r.toUpper().compare("T") == 0)
		{
			QMessageBox::about(this, QString::fromLocal8Bit("设备状态"), QString::fromLocal8Bit("操作成功！"));
		}
		else
		{
			QMessageBox::about(this, QString::fromLocal8Bit("设备状态"), QString::fromLocal8Bit("操作失败！"));
		}
	}
	break;
	}
}

