#ifdef QT_AVAILABLE
#include "filenameedit.h"

#include <QPushButton>
#include <QEvent>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QFileDialog>
#include <QCompleter>
#include <QFileSystemModel>

#include "pixmapwidget.h"

FileNameEdit::FileNameEdit(QWidget* parent)
    : QLineEdit(parent)
    , __pb(new QPushButton(this))
    , __warning(([](QWidget* parent)->QWidget*{
                PixmapWidget* w(new PixmapWidget(parent));
                const int a(15);
                w->setFixedSize(a, a);
                w->setPixmap(QPixmap(":/img/warning.png"));
                return w;
                   })(this))
    , __onClick([](FileNameEdit* parent)->QString{ return QFileDialog::getOpenFileName(parent); })
{
    __pb->setIcon(QIcon(":/img/open.png"));
    __pb->setToolTip(tr("Chose file"));
    __pb->setFlat(true);
    __pb->setFixedHeight(this->height());
    __pb->setFixedSize(19,19);
    connect(__pb, SIGNAL(clicked()), SLOT(callDialod()));
    resizeEvent(0);

    __pb->installEventFilter(this);

    QMargins m(this->textMargins());
    m.setRight(m.right() + 35);
    this->setTextMargins(m);
    this->setCompleter(new QCompleter(this));
    QFileSystemModel* const model(new QFileSystemModel(this));
    model->setRootPath("");
    this->completer()->setMaxVisibleItems(10);
    this->completer()->setModel(model);
    __warning->setToolTip(tr("file still not selected"));
    this->connect(this, SIGNAL(textChanged(QString)), this, SLOT(valueTest()));
}

void FileNameEdit::resizeEvent(QResizeEvent * e) {
    if (e) {
        QLineEdit::resizeEvent(e);
    }
    this->__pb->move(QPoint(this->width() - 20, (this->height() - __pb->height()) >> 1));
    this->__warning->move(this->width() - 35, (this->height() - __warning->height()) >> 1);
}

void FileNameEdit::valueTest() {
    QFileInfo checkFile(this->text());
    // check if file exists and if yes: Is it really a file and no directory?
    if (checkFile.exists() && checkFile.isFile()) {
        if (__val != this->text()) {
            __val = this->text();
            emit fileNameChanged(this->text());
        }
        if (!__warning->isHidden()) {
            __warning->hide();
            QMargins m(this->textMargins());
            m.setRight(m.right() - 18);
            this->setTextMargins(m);
        }
    } else {
        if (__warning->isHidden()) {
            __warning->show();
            QMargins m(this->textMargins());
            m.setRight(m.right() + 18);
            this->setTextMargins(m);
        }
    }
}

bool FileNameEdit::eventFilter(QObject* o, QEvent* e) {
    Q_ASSERT(o == __pb);
    if (e->type() == QEvent::Enter) {
        this->setCursor(QCursor(Qt::ArrowCursor));
        return true;
    } else if (e->type() == QEvent::Leave) {
        this->setCursor(QCursor(Qt::IBeamCursor));
        return true;
    }
    return false;
}

void FileNameEdit::callDialod() {
    const QString s(__onClick(this));
    if (!QFile::exists(s)) {
        return;
    }
    this->setText(s);
    valueTest();
}
#endif
