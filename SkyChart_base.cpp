#include "SkyChart_base.h"

SkyChart_base::SkyChart_base(QWidget *parent):
    QGraphicsView(new QGraphicsScene(parent), parent),
    m_rubberBand(nullptr),
    pressMouse(false)
{
    //fill all parent widget
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    setRenderHint(QPainter::Antialiasing, true);
    setAcceptDrops(true);
    baseChart = new Base2DChart();
    scene()->addItem(baseChart->getChart());
    this->setMouseTracking(true);
    //initialaze visible mode
    setVisible(true);
}

SkyChart_base::~SkyChart_base()
{

}

bool SkyChart_base::createOrAdd2DChart(std::vector<float> *_x, std::vector<float> *_y, QString _titleName)
{
    baseChart->setPoints(_x,_y);
    baseChart->setTitleName(_titleName);
    this->setFocus();
    return true;
}

QWidget *SkyChart_base::skyChart()
{
    return this;
}

void SkyChart_base::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        baseChart->getChart()->resize(event->size());
    }
    QGraphicsView::resizeEvent(event);
}
#ifndef QT_NO_WHEELEVENT
void SkyChart_base::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
        baseChart->getChart()->zoomIn();
    if (event->angleDelta().y() < 0)
        baseChart->getChart()->zoomOut();
    event->accept();
    QGraphicsView::wheelEvent(event);
}
#endif

void SkyChart_base::screenshot()
{
    //first need make more resolution
    /*
        As one of work  method, send chart to new widget and set him more resolution
        QGridLayout(chart), QGridLayout().setRect(), QWidget::grab()
        QGridLayout* layout = new QGridLayout();
        QWidget* widget = new QWidget(_chart);
        layout->addWidget(widget);
        layout->setGeometry(QRect(0,0,1920,1080));
    */
    QPixmap screenshot = QWidget::grab();
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr(""), "",
                                                    tr("(*.png)"));
    screenshot.save(fileName, "png");
}

void SkyChart_base::copy()
{
    //copy function
    QClipboard* clipBoard = QApplication::clipboard();
    QPixmap screenshot = QWidget::grab();
    clipBoard->setPixmap(screenshot);
}

void SkyChart_base::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_S:
        if (QGuiApplication::keyboardModifiers() == Qt::ControlModifier)
            screenshot();
        break;
    case Qt::Key_C:
        if (QGuiApplication::keyboardModifiers() == Qt::ControlModifier)
            copy();
        break;
    default:
        break;
    }
}

void SkyChart_base::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    if(pressMouse){
        if (m_rubberBand != nullptr){
            m_rubberBand->setGeometry(QRect(m_pressButtonPosition, event->pos()).normalized());
        }
    }
}

void SkyChart_base::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    pressMouse = true;
    m_pressButtonPosition = event->pos();
    if (!m_rubberBand){
        m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        QPalette _pal;
        _pal.setBrush(QPalette::Highlight, QBrush(Qt::darkBlue));
        m_rubberBand->setPalette(_pal);
        m_rubberBand->setVisible(true);
        m_rubberBand->show();
    }
    m_rubberBand->setGeometry(QRect(m_pressButtonPosition, QSize()));
    m_rubberBand->show();
}

void SkyChart_base::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    if (m_rubberBand) {
        m_rubberBand->hide();
        if(m_pressButtonPosition.x() > event->pos().x()){
            baseChart->getChart()->zoomReset();
        }
        else{
            QRectF rect = m_rubberBand->geometry();
            baseChart->getChart()->zoomIn(rect);
        }
    }
    pressMouse = false;
}


