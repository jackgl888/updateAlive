#include "qiplinediter.h"

QIPlinEditer::QIPlinEditer(QWidget *parent) : QLineEdit(parent)
{
    QRegExp rx("([0-1][0-9]{2}|[0-9]{2}|2[0-5]{2})");
    QHBoxLayout *pHBox = new QHBoxLayout(this);
    pHBox->setSpacing(10);
    pHBox->setContentsMargins(0,0,0,0);

    for(int i = 0;i<4;i++)
    {
        this->m_lineEdit[i] = new QLineEdit(this);
        this->m_lineEdit[i]->setFrame(false);
        this->m_lineEdit[i]->setMaxLength(3);
        this->m_lineEdit[i]->setAlignment(Qt::AlignCenter);
        m_lineEdit[i]->installEventFilter(this);
        m_lineEdit[i]->setValidator(new QRegExpValidator(rx, this));
        this->m_lineEdit[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        pHBox->addWidget(this->m_lineEdit[i]);
    }
    this->setReadOnly(true);
}


QIPlinEditer::~QIPlinEditer()
{

}

void QIPlinEditer::setText(const QString &strIP)
{
    if (!isTextValid(strIP))
        {
            QMessageBox::warning(this, "Attention", "Your IP Address is Invalid!", QMessageBox::StandardButton::Ok);
            return;
        }
        else
        {
            int i = 0;
            QStringList ipList = strIP.split(".");
            foreach (QString ip , ipList)
            {
                m_lineEdit[i]->setText(ip);
                i++;
            }
        }
}


QString QIPlinEditer::text() const
{
    QString strIP;
        for (int i = 0; i < 4; i++)
        {    if(i<3)
             strIP.append(m_lineEdit[i]->text()+".");
             else
              strIP.append(m_lineEdit[i]->text());
        }

        return strIP;
}

void QIPlinEditer::paintEvent(QPaintEvent *event)
{
    QPainter   painter(this);
    QBrush  brush;
    brush.setStyle(Qt::BrushStyle:: SolidPattern);
    brush.setColor(Qt::black);
    painter.setBrush(brush);

    int width =0;
    for(int i =0; i<3;i++)
    {
        width += this->m_lineEdit[i]->width()+ (i == 0 ? 3: 10);//布局的间隔
        painter.drawEllipse(width, height() / 2 - 2, 4, 4);
    }
}


bool QIPlinEditer::eventFilter(QObject *obj, QEvent *ev)
{
    if (children().contains(obj) && QEvent::KeyPress == ev->type())
        {
            QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(ev);
            QLineEdit *pEdit = qobject_cast<QLineEdit *>(obj);
            switch (keyEvent->key())
            {
            case Qt::Key_0:
            case Qt::Key_1:
            case Qt::Key_2:
            case Qt::Key_3:
            case Qt::Key_4:
            case Qt::Key_5:
            case Qt::Key_6:
            case Qt::Key_7:
            case Qt::Key_8:
            case Qt::Key_9:
            {
                QString strText = pEdit->text();
                uchar len = pEdit->selectedText().length();
                if (pEdit->selectedText().length())
                {
                    pEdit->text().replace(pEdit->selectedText(), QChar(keyEvent->key()));
                }
                else if (strText.length() == 3 || strText.length() < 3 && strText.toInt() * 10 > 255)
                {
                    int index = getIndex(pEdit);
                    if (index != -1 && index != 3)
                    {
                        m_lineEdit[index + 1]->setFocus();
                        m_lineEdit[index + 1]->selectAll();
                    }
                }
                else if (strText.length() == 2 && strText.toInt() * 10 < 255)
                {
                    if (Qt::Key_0 == keyEvent->key() && strText.toInt())
                    {
                        pEdit->setText(strText.insert(pEdit->cursorPosition(), QChar(Qt::Key_0)));
                    }
                }
                return QLineEdit:: eventFilter(obj, ev);
            }
            break;
            case Qt::Key_Backspace:
            {
                QString strText = pEdit->text();
                if (!strText.length() || strText.length() && !pEdit->cursorPosition())//211
                {
                    int index = getIndex(pEdit);
                    if (index != -1 && index != 0)
                    {
                        m_lineEdit[index - 1]->setFocus();
                        int length = m_lineEdit[index - 1]->text().length();
                        m_lineEdit[index - 1]->setCursorPosition(length ? length : 0);
                    }
                }
                return QLineEdit::eventFilter(obj, ev);
            }
            case Qt::Key_Left:
            {
                if (!pEdit->cursorPosition())
                {
                    int index = getIndex(pEdit);
                    if (index != -1 && index != 0)
                    {
                        m_lineEdit[index - 1]->setFocus();
                        int length =m_lineEdit[index - 1]->text().length();
                        m_lineEdit[index - 1]->setCursorPosition(length ? length : 0);
                    }
                }
                return QLineEdit::eventFilter(obj, ev);
            }
            case Qt::Key_Right:
            {
                if (pEdit->cursorPosition() == pEdit->text().length())
                {
                    int index = getIndex(pEdit);
                    if (index != -1 && index != 3)
                    {
                        m_lineEdit[index + 1]->setFocus();
                        m_lineEdit[index + 1]->setCursorPosition(0);
                    }
                }
                return QLineEdit::eventFilter(obj, ev);
            }
            default:
                break;
            }
        }
        return false;

}

int QIPlinEditer::getIndex(QLineEdit *pEdit)
{
    int index = -1;
        for (int i = 0; i < 4; i++)
        {
            if (pEdit == m_lineEdit[i])
                index = i;
        }
        return index;
}







bool QIPlinEditer::isTextValid(const QString &strIP)
{
    QRegExp rx2("\\b(?:(?:25[0-9|2[0-9][0-9]|[012]?[0-9][0-9]?)\\.){3}(?:25[0-9]|2[0-9][0-9]|[012]?[0-9][0-9]?)\\b");
        if (!rx2.exactMatch(strIP))
            return false;
        return true;
}
